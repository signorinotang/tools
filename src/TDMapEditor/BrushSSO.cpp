//
//  BrushSSO.cpp
//  TDMapEditor
//
//  Created by Wayne on 6/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BrushSSO.h"
#include "vnUILocatorRect.h"
#include "EditorApp.h"
#include "vnFileManager.h"
#include "vnUIListItem.h"

#include "vnCommandList.h"

/////////////////////////////////////////

class Command_CreateSSO : public vn::Command, public vn::AllocatedObject {
public:
    Command_CreateSSO(const vn::FilePath &scriptFile, const vector2f &position)
    : m_sso(vnnew MapSSO()) {
        if (!m_sso->init(scriptFile, position)) {
            m_sso.release();
        }
    }
    
    virtual void _free() {
        delete this;
    }
    
	virtual void _do() {
        EditorApp::instance().addSSO(m_sso);
    }
    
	virtual void _undo() {
        EditorApp::instance().removeSSO(m_sso);
    }
    
	virtual void _redo() {
        EditorApp::instance().addSSO(m_sso);
        m_sso->sso_reset();
    }
    
private:
    MapSSOPtr m_sso;
};

class Command_DeleteSSO : public vn::Command, public vn::AllocatedObject {
public:
    Command_DeleteSSO(const MapSSOPtr &sso)
    : m_sso(sso) {}
    
    virtual void _free() {
        delete this;
    }
    
	virtual void _do() {
        EditorApp::instance().removeSSO(m_sso);
    }
    
	virtual void _undo() {
        EditorApp::instance().addSSO(m_sso);
        m_sso->sso_reset();
    }
    
	virtual void _redo() {
        EditorApp::instance().removeSSO(m_sso);
    }
    
private:
    MapSSOPtr m_sso;
};

class Command_MoveSSO : public vn::Command, public vn::AllocatedObject {
public:
    Command_MoveSSO(const MapSSOPtr &sso, const vector2f &old, const vector2f &pos)
    : m_sso(sso), m_oldPos(old), m_newPos(pos) {}
    
    virtual void _free() {
        delete this;
    }
    
	virtual void _do() {
        m_sso->setPosition(m_newPos);
    }
    
	virtual void _undo() {
        m_sso->setPosition(m_oldPos);
    }
    
	virtual void _redo() {
        m_sso->setPosition(m_newPos);
    }
    
private:
    MapSSOPtr m_sso;
    vector2f m_oldPos, m_newPos;
};


/////////////////////////////////////////

Brush_SSO::Brush_SSO() {
    
}

Brush_SSO::~Brush_SSO() {
    
}

void Brush_SSO::init() {
    m_uiWindow = vnnew vn::UIWindow();
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderLeft, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderRight, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderTop, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderBottom, 8);
    m_uiWindow->setMinSize(vn::vector2f(256, 256));
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow.ptr());
    locator->setSize(vn::vector2f(256, 256));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow.ptr(), "1:/TDMapEditor/ui_brush_sso.tdl");
    
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);

    m_btnDelete = dynamic_cast<vn::UIButton *>(namedElements["delete"]);
    if (m_btnDelete) {
        m_btnDelete->setEnabled(false);
        m_btnDelete->bindAction_Clicked(vn::make_memfn(this, &Brush_SSO::_onDelete));
    }
    
    m_lstSSO = dynamic_cast<vn::UIList *>(namedElements["list"]);
    if (m_lstSSO) {
        m_lstSSO->bindAction_ItemClicked(vn::make_memfn(this, &Brush_SSO::_onItemClicked));
        
        std::list<vn::str8> result;
        vn::FileManager::instance().list("1001:/sso/", result);
        for (auto &str : result) {
            static_cast<vn::UIListItemText *>(m_lstSSO->addItem())->setText(str);
        }
    }
}

void Brush_SSO::fini() {
    
}

void Brush_SSO::begin(const vn::vector2f &pos) {
    m_lastPos = pos;
    m_moved = false;
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    if (m_state == kEdit && !m_sso.null()) {
        m_moveSSO = m_sso->contains(_pos);
    } else {
        m_moveSSO = false;
    }
}

void Brush_SSO::end(const vn::vector2f &pos) {
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    if (m_moved) {
        if (m_moveSSO) {
            EditorApp::instance().commandList().do_(vnnew Command_MoveSSO(m_sso, m_originPos, m_sso->getPosition()));
            m_originPos = m_sso->getPosition();
        }
    } else {
        if (m_sso.null()) {
            MapSSOPtr sso = EditorApp::instance().pickSSO(_pos);
            if (sso.ptr()) {
                m_originPos = sso->getPosition();
                sso->setFrameVisible(true);
                m_sso = sso;
                m_btnDelete->setEnabled(true);
            }
        } else {
            if (m_state == kNew) {
                EditorApp::instance().commandList().do_(vnnew Command_CreateSSO(m_sso->desc()->scriptFile, _pos));
            } else if (!m_moveSSO){
                m_sso->setFrameVisible(false);
                MapSSOPtr sso = EditorApp::instance().pickSSO(_pos);
                if (sso.ptr()) {
                    m_sso = sso;
                    sso->setFrameVisible(true);
                } else {
                    m_sso.release();
                    m_btnDelete->setEnabled(false);
                }
            }
        }
    }
}

void Brush_SSO::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    m_node->setPosition(_pos);
    if (paint) {
        if (pos != m_lastPos) {
            m_moved = true;
            if (m_state == kNew || m_sso.null() || !m_moveSSO) {
                EditorApp::instance().camera().move(m_lastPos - pos);
            } else {
                m_sso->setPosition(m_sso->getPosition() + _pos - EditorApp::instance().camera().convertFromView(m_lastPos));
            }
            m_lastPos = pos;
        }
    }
}

bool Brush_SSO::busy() const {
    return false;
}

void Brush_SSO::switchIn() {
    EditorApp::instance().uiRootElement()->addChild(m_uiWindow.ptr());
    EditorApp::instance().layer_brush()->addChild(m_node.ptr());
}

void Brush_SSO::switchOut() {
    EditorApp::instance().uiRootElement()->removeChild(m_uiWindow.ptr());
    EditorApp::instance().layer_brush()->removeChild(m_node.ptr());
}

void Brush_SSO::onKeyDown(vn::c32 chr, bool repeat) {
    switch (chr) {
        case 'D':
        case 'd':
            if (m_btnDelete->isEnabled() && !repeat) {
                _onDelete(m_btnDelete);
            }
            break;
    }
}

void Brush_SSO::onMouseRightDown(const vn::vector2f &pos) {
    _cancelNew();
}

void Brush_SSO::onMouseRightUp(const vn::vector2f &pos) {
    
}

void Brush_SSO::onMapSSORemoved(const MapSSOPtr &sso) {
    if (sso.ptr() == m_sso.ptr()) {
        _cancelEdit();
    }
}

void Brush_SSO::_onDelete(vn::UIElement *) {
    if (m_sso.ptr()) {
        MapSSOPtr sso = m_sso;
        _cancelEdit();
        EditorApp::instance().commandList().do_(vnnew Command_DeleteSSO(sso));
    }
}

void Brush_SSO::_onItemClicked(vn::UIElement *, vn::UIListItem *item) {
    if (item) {
        _cancelEdit();
        if (!m_sso.null()) {
            m_node->removeChild(m_sso.ptr());
        }
        m_lstSSO->setSelectedItem(item);
        vn::str8 file;
        static_cast<vn::UIListItemText *>(item)->getText(file);
        m_sso = vnnew MapSSO();
        if (m_sso->init(FilePath(1001, vn::str8("sso/") + file), vector2f::kZero)) {
            m_node->addChild(m_sso.ptr());
            m_state = kNew;
        } else {
            m_sso.release();
            m_state = kEdit;
        }
    } else {
        _cancelNew();
    }
}


void Brush_SSO::_cancelEdit() {
    if (m_state == kEdit && !m_sso.null()) {
        m_sso->setFrameVisible(false);
        m_sso->setPosition(m_originPos);
        m_sso.release();
        m_btnDelete->setEnabled(false);
    }
}

void Brush_SSO::_cancelNew() {
    if (m_state == kNew) {
        if (!m_sso.null()) {
            m_node->removeChild(m_sso.ptr());
            m_sso.release();
        }
        m_lstSSO->setSelectedItem(0);
        m_state = kEdit;
    }
}