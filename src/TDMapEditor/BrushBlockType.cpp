//
//  BrushBlockType.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-31.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "BrushBlockType.h"
#include "EditorApp.h"
#include "vnUILocatorRect.h"

Brush_BlockType::Brush_BlockType()
: m_type(kMBT_Land)
, m_uiWindow(0)
, m_command(0) {
    
}

Brush_BlockType::~Brush_BlockType() {
    VN_SAFE_DROP(m_uiWindow);
	VN_SAFE_DELETE(m_command);
}

void Brush_BlockType::init() {
    BlockBrush::init();
    m_uiWindow = vnnew vn::UIWindow();
    m_uiWindow->setSizeable(false);
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow);
    locator->setSize(vn::vector2f(140, 206));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow, "1:/TDMapEditor/ui_brush_block_type.tdl");
    
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);
    m_uiTypeList = (vn::UIList *)namedElements["type_list"];
    m_uiTypeList->bindAction_ItemClicked(vn::make_memfn(this, &Brush_BlockType::_onTypeListItemClicked));
    m_uiTypeList->setSelectedItem(m_uiTypeList->item(0));
}

void Brush_BlockType::fini() {
    VN_SAFE_DROP(m_uiWindow);
    BlockBrush::fini();
}

void Brush_BlockType::begin(const vn::vector2f &pos) {
    _updateBrush(pos);
	m_command = vnnew Command_PaintBlockType();
    _paint();
}

void Brush_BlockType::end(const vn::vector2f &pos) {
	_updateBrush(pos);
	_paint();
    if (m_command->empty()) {
        delete m_command;
    } else {
        EditorApp::instance().commandList().do_(m_command);
    }
	m_command = 0;
}

void Brush_BlockType::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    _updateBrush(pos);
    if (paint) {
        _paint();
    }
}

void Brush_BlockType::switchIn() {
    BlockBrush::switchIn();
    EditorApp::instance().uiRootElement()->addChild(m_uiWindow);
}

void Brush_BlockType::switchOut() {
    BlockBrush::switchOut();
    EditorApp::instance().uiRootElement()->removeChild(m_uiWindow);
}

void Brush_BlockType::onKeyDown(vn::c32 chr, bool repeat) {
    if (repeat) {
        return ;
    }
    if (chr > '0' && chr <='9') {
        _setBrushSize(chr - '0');
    } else {
        switch (chr) {
            case 'A':
            case 'a':
                m_type = kMBT_Field_Red;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'S':
            case 's':
                m_type = kMBT_Field_Green;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'D':
            case 'd':
                m_type = kMBT_Field_White;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'F':
            case 'f':
                m_type = kMBT_Field_HighGround;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'G':
            case 'g':
                m_type = kMBT_Land;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'H':
            case 'h':
                m_type = kMBT_Water;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
            case 'J':
            case 'j':
                m_type = kMBT_DeepWater;
                m_uiTypeList->setSelectedItem(m_uiTypeList->item(m_type));
                break;
                
        }
    }
}

void Brush_BlockType::_setBrushSize(vn::s32 _size) {
    if (m_size.x == _size) {
        return ;
    }
    m_size.set(_size, _size);
    _resetBrushNode();
}

void Brush_BlockType::_paint() {
    vn::vector2i pos, last = m_index + m_size;
    for (pos.y = m_index.y; pos.y < last.y; ++pos.y) {
        for (pos.x = m_index.x; pos.x < last.x; ++pos.x) {
			MapBlockType oldType;
			if (EditorApp::instance().setBlockType(pos, m_type, &oldType)) {
				m_command->add(pos, oldType, m_type);
			}
        }
    }
}

void Brush_BlockType::_onTypeListItemClicked(vn::UIElement *, vn::UIListItem *item) {
    if (!item) {
        return ;
    }
    m_uiTypeList->setSelectedItem(item);
    m_type = (MapBlockType)m_uiTypeList->itemIndex(item);
}

//////////////////////////////////////////////////////////

void Command_PaintBlockType::_free() {
	delete this;
}

void Command_PaintBlockType::_do() {
	
}

void Command_PaintBlockType::_undo() {
	EditorApp &r = EditorApp::instance();
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		r.setBlockType(it->index, it->oldType);
	}
}

void Command_PaintBlockType::_redo() {
	EditorApp &r = EditorApp::instance();
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		r.setBlockType(it->index, it->newType);
	}
}

void Command_PaintBlockType::add(const vn::vector2i &index, MapBlockType oldType, MapBlockType newType) {
	Item item = {index, oldType, newType};
	m_items.push_back(item);
}

bool Command_PaintBlockType::empty() const {
    return m_items.empty();
}