//
//  BrushDecoration.cpp
//  TDMapEditor
//
//  Created by Wayne on 3/31/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BrushDecoration.h"
#include "vnUILocatorRect.h"
#include "EditorApp.h"
#include "UIListItemDecorationImage.h"
#include "UIListItemDecorationAnimation.h"
#include "Renderable.h"
#include "vnPictureManager.h"


Brush_Decoration::Brush_Decoration()
: m_btnCreate0(0), m_btnCreate1(0)
, m_lstImages(0), m_lstAnimations(0)
, m_btnFlipX(0), m_btnFlipY(0)
, m_btnLightMode(0)
, m_btnDelete(0)
, m_sbOpacity(0), m_sbRed(0), m_sbGreen(0), m_sbBlue(0)
, m_node(vnnew SceneNode())
, m_decoration(0)
, m_flipX(false), m_flipY(false)
, m_lightMode(false)
, m_color(vn::color_t::kWhite)
, m_state(kEdit)
, m_edit(0)
, m_editMoving(false)
, m_editSizing(false)
, m_animController(vnnew AnimationController(this)) {
    m_animController->setLoop(true);
}

Brush_Decoration::~Brush_Decoration() {

}

void Brush_Decoration::init() {
    m_uiWindow = vnnew vn::UIWindow();
    //m_uiWindow->setSizeable(false);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderLeft, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderRight, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderTop, 0);
    m_uiWindow->setBorderSize(vn::UIWindow::kBorderBottom, 4);
    m_uiWindow->setMinSize(vn::vector2f(384, 424));
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow.ptr());
    locator->setSize(vn::vector2f(384, 552));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow.ptr(), "1:/TDMapEditor/ui_brush_decoration.tdl");
    
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);
    
    m_btnCreate0 = dynamic_cast<vn::UIButton *>(namedElements["create0"]);
    m_btnCreate1 = dynamic_cast<vn::UIButton *>(namedElements["create1"]);
    if (m_btnCreate0) {
        m_btnCreate0->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onCreateClicked));
    }
    if (m_btnCreate1) {
        m_btnCreate1->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onCreateClicked));
    }
    
    m_lstImages = dynamic_cast<vn::UIList *>(namedElements["image_list"]);
    m_lstAnimations = dynamic_cast<vn::UIList *>(namedElements["anim_list"]);
    if (m_lstImages) {
        m_lstImages->bindAction_ItemClicked(vn::make_memfn(this, &Brush_Decoration::_onImageItemClicked));
    }
    if (m_lstAnimations) {
        m_lstAnimations->bindAction_ItemClicked(vn::make_memfn(this, &Brush_Decoration::_onAnimItemClicked));
    }
    
    m_btnDelete = dynamic_cast<vn::UIButton *>(namedElements["delete"]);
    if (m_btnDelete) {
        m_btnDelete->setEnabled(false);
        m_btnDelete->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onDeleteClicked));
    }
    
    m_btnFlipX = dynamic_cast<vn::UIButton *>(namedElements["flip_x"]);
    m_btnFlipY = dynamic_cast<vn::UIButton *>(namedElements["flip_y"]);
    m_btnLightMode = dynamic_cast<vn::UIButton *>(namedElements["light"]);
    if (m_btnFlipX) {
        m_btnFlipX->setEnabled(false);
        m_btnFlipX->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onFlipXClicked));
    }
    if (m_btnFlipY) {
        m_btnFlipY->setEnabled(false);
        m_btnFlipY->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onFlipYClicked));
    }
    if (m_btnLightMode) {
        m_btnLightMode->setEnabled(false);
        m_btnLightMode->bindAction_Clicked(vn::make_memfn(this, &Brush_Decoration::_onLightModeClicked));
    }
    
    m_sbOpacity = dynamic_cast<vn::UIScrollBar *>(namedElements["opacity"]);
    if (m_sbOpacity) {
        m_sbOpacity->setVisible(false);
        m_sbOpacity->setSliderPosition((vn::f32)m_color.alpha);
        m_sbOpacity->bindAction_Scrolling(vn::make_memfn(this, &Brush_Decoration::_onOpacityScrolling));
    }
    
    m_sbRed = dynamic_cast<vn::UIScrollBar *>(namedElements["red"]);
    if (m_sbRed) {
        m_sbRed->setVisible(false);
        m_sbRed->setSliderPosition((vn::f32)m_color.red);
        m_sbRed->bindAction_Scrolling(vn::make_memfn(this, &Brush_Decoration::_onRedScrolling));
    }
    
    m_sbGreen = dynamic_cast<vn::UIScrollBar *>(namedElements["green"]);
    if (m_sbGreen) {
        m_sbGreen->setVisible(false);
        m_sbGreen->setSliderPosition((vn::f32)m_color.red);
        m_sbGreen->bindAction_Scrolling(vn::make_memfn(this, &Brush_Decoration::_onGreenScrolling));
    }
    
    m_sbBlue = dynamic_cast<vn::UIScrollBar *>(namedElements["blue"]);
    if (m_sbBlue) {
        m_sbBlue->setVisible(false);
        m_sbBlue->setSliderPosition((vn::f32)m_color.red);
        m_sbBlue->bindAction_Scrolling(vn::make_memfn(this, &Brush_Decoration::_onBlueScrolling));
    }
    
}

void Brush_Decoration::fini() {

}

void Brush_Decoration::begin(const vn::vector2f &pos) {
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    if (m_state == kEdit) {
        if (m_edit && m_edit->canOperate(_pos)) {
            m_editSizing = true;
            m_edit->setOperating(true);
            m_extent = m_edit->getExtent();
            m_editOffset = _pos;
        } else {
            MapDecoration *deco = EditorApp::instance().pickDecoration(_pos);
            if (deco == m_edit) {
                if (deco) {
                    m_editLastPos = deco->getPosition();
                    m_editOffset = deco->getPosition() - _pos;
                    m_editMoving = true;
                }
            } else {
                _setEditDecoration(deco);
            }
        }
    }
}

void Brush_Decoration::end(const vn::vector2f &pos) {
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    if (m_state == kNewImage) {
        EditorApp::instance().commandList().do_(vnnew Command_NewDecoration(MapDecoration::kImage, vn::vector2i(_pos), m_decorationFilePath, m_flipX, m_flipY, m_lightMode, m_extent, m_color));
    } else if (m_state == kNewAnimation) {
        EditorApp::instance().commandList().do_(vnnew Command_NewDecoration(MapDecoration::kAnimation, vn::vector2i(_pos), m_decorationFilePath, m_flipX, m_flipY, m_lightMode, m_extent, m_color));
    } else {
        if (m_edit) {
            
            if (m_editMoving) {
                m_editMoving = false;
                m_edit->grab();
                EditorApp::instance().commandList().do_(vnnew Command_MoveDecoration(m_edit, m_editLastPos, vn::vector2i(_pos + m_editOffset)));
            } else if (m_editSizing) {
                m_editSizing = false;
                vn::vector2f extent = (_pos - m_editOffset) * 2.f + m_extent;
                if (extent.x < 16) {
                    extent.x = 16;
                }
                if (extent.y < 16) {
                    extent.y = 16;
                }
                m_edit->setOperating(false);
                m_edit->grab();
                EditorApp::instance().commandList().do_(vnnew Command_SizeDecoration(m_edit, m_extent, extent));
            }
        }
    }
}

void Brush_Decoration::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    vn::vector2f _pos = EditorApp::instance().camera().convertFromView(pos);
    m_node->setPosition(vn::vector2i(_pos));
    if (m_state == kEdit) {
        if (m_edit) {
            if (m_editMoving) {
                m_edit->setPosition(vn::vector2i(_pos + m_editOffset));
            } else if (m_editSizing) {
                vn::vector2f extent = (_pos - m_editOffset) * 2.f + m_extent;
                if (extent.x < 16) {
                    extent.x = 16;
                }
                if (extent.y < 16) {
                    extent.y = 16;
                }
                m_edit->setExtent(vn::vector2i(extent));
            } else {
                bool flag = m_edit->canOperate(_pos);
                m_edit->setOperating(flag);
            }
        }
    }
    
    if (m_state == kNewAnimation) {
        m_animController->advance(deltaTime);
    }
}

bool Brush_Decoration::busy() const {
    return m_editMoving;
}

void Brush_Decoration::switchIn() {
    EditorApp::instance().uiRootElement()->addChild(m_uiWindow.ptr());
    EditorApp::instance().layer_brush()->addChild(m_node.ptr());
}

void Brush_Decoration::switchOut() {
    EditorApp::instance().uiRootElement()->removeChild(m_uiWindow.ptr());
    EditorApp::instance().layer_brush()->removeChild(m_node.ptr());
    if (m_state == kEdit) {
        _setEditDecoration(0);
    } else {
        _cancelNew();
    }
}

void Brush_Decoration::onKeyDown(vn::c32 chr, bool repeat) {
    switch (chr) {
        case 'J':
        case 'j':
            if (m_btnFlipX->isEnabled()) {
                _onFlipXClicked(m_btnFlipX);
            }
            break;
            
        case 'K':
        case 'k':
            if (m_btnFlipY->isEnabled()) {
                _onFlipYClicked(m_btnFlipY);
            }
            break;
            
        case 'L':
        case 'l':
            if (m_btnLightMode->isEnabled()) {
                _onLightModeClicked(m_btnLightMode);
            }
            break;
            
        case 'D':
        case 'd':
            if (m_btnDelete->isEnabled()) {
                _onDeleteClicked(m_btnDelete);
            }
            break;
    }
}

void Brush_Decoration::onMouseRightDown(const vn::vector2f &pos) {
    if (m_state != kEdit) {
        _cancelNew();
    }
}

void Brush_Decoration::onMouseRightUp(const vn::vector2f &pos) {

}

void Brush_Decoration::onMapObjectRemoved(MapObject *mobj) {

}

void Brush_Decoration::onMapDecorationRemoved(MapDecoration *deco) {
    if (m_edit == deco) {
        _setEditDecoration(0);
    }
}

void Brush_Decoration::anim_frame(const aabox2f &rect, const PicturePtr &image) {
    if (m_quad) {
        *m_quad << rect << image.ptr();
    }
}

void Brush_Decoration::_onCreateClicked(vn::UIElement *element) {
    if (!m_btnCreate0->isPushed() && element == m_btnCreate0) {
        m_btnCreate0->setPushed(true);
        m_btnCreate1->setPushed(false);
        if (m_lstImages) {
            m_lstImages->setVisible(true);
        }
        if (m_lstAnimations) {
            m_lstAnimations->setVisible(false);
        }
    } else if (!m_btnCreate1->isPushed() && element == m_btnCreate1) {
        m_btnCreate0->setPushed(false);
        m_btnCreate1->setPushed(true);
        if (m_lstImages) {
            m_lstImages->setVisible(false);
        }
        if (m_lstAnimations) {
            m_lstAnimations->setVisible(true);
        }
    }
    if (m_state != kEdit) {
        _cancelNew();
    }
}

void Brush_Decoration::_onImageItemClicked(vn::UIElement *, vn::UIListItem *item) {
    if (item) {
        UIListItemDecorationImage *p = dynamic_cast<UIListItemDecorationImage *>(item);
        if (p) {
            m_lstImages->setSelectedItem(item);
            _beginNewImage(p->getImagePath());
        }
    } else if (m_state != kEdit) {
        _cancelNew();
    }
}

void Brush_Decoration::_onAnimItemClicked(vn::UIElement *, vn::UIListItem *item) {
    if (item) {
        UIListItemDecorationAnimation *p = dynamic_cast<UIListItemDecorationAnimation *>(item);
        if (p) {
            m_lstAnimations->setSelectedItem(item);
            _beginNewAnimation(p->getAnimationPath());
            
        }
    } else if (m_state != kEdit) {
        _cancelNew();
    }
}

void Brush_Decoration::_onDeleteClicked(vn::UIElement *) {
    MapDecoration *deco = m_edit;
    if (deco) {
        deco->grab();
        _setEditDecoration(0);
        EditorApp::instance().commandList().do_(vnnew Command_DeleteDecoration(deco));
    }
}

void Brush_Decoration::_onFlipXClicked(vn::UIElement *) {
    m_flipX = !m_flipX;
    _updateOptions();
    if (m_edit) {
        m_edit->setFlipX(m_flipX);
    }
}

void Brush_Decoration::_onFlipYClicked(vn::UIElement *) {
    m_flipY = !m_flipY;
    _updateOptions();
    if (m_edit) {
        m_edit->setFlipY(m_flipY);
    }
}

void Brush_Decoration::_onLightModeClicked(vn::UIElement *) {
    m_lightMode = !m_lightMode;
    _updateOptions();
    if (m_edit) {
        m_edit->setLightMode(m_lightMode);
    }
}

void Brush_Decoration::_onOpacityScrolling(vn::UIElement *, vn::f32 pos) {
    m_color.alpha = (vn::u8)pos;
    if (m_quad) {
        *m_quad << m_color;
    }
    if (m_edit) {
        m_edit->setColor(m_color);
    }
}

void Brush_Decoration::_onRedScrolling(vn::UIElement *, vn::f32 pos) {
    m_color.red = (vn::u8)pos;
    if (m_quad) {
        *m_quad << m_color;
    }
    if (m_edit) {
        m_edit->setColor(m_color);
    }
}

void Brush_Decoration::_onGreenScrolling(vn::UIElement *, vn::f32 pos) {
    m_color.green = (vn::u8)pos;
    if (m_quad) {
        *m_quad << m_color;
    }
    if (m_edit) {
        m_edit->setColor(m_color);
    }
}

void Brush_Decoration::_onBlueScrolling(vn::UIElement *, vn::f32 pos) {
    m_color.blue = (vn::u8)pos;
    if (m_quad) {
        *m_quad << m_color;
    }
    if (m_edit) {
        m_edit->setColor(m_color);
    }
}

void Brush_Decoration::_beginNewImage(const vn::str8 &imagePath) {
    if (m_state != kEdit) {
        m_node->clearChildren();
    } else {
        _setEditDecoration(0);
    }
    m_state = kNewImage;
    m_decorationFilePath = imagePath;
    m_decoration = vnnew SceneNode();
    m_node->addChild(m_decoration, false);
    m_quad = vnnew Quad();
    m_decoration->addRenderable(m_quad, false);
    vn::PicturePtr pic = vn::PictureManager::instance().createPicture(m_decorationFilePath.c_str());
    if (!pic.null()) {
        m_extent = pic->size;
        vn::vector2f half = pic->size * 0.5f;
        vn::aabox2f rect(-half, half);
        *m_quad << rect << pic.ptr();
    } else {
        m_extent.set(16, 16);
        vn::aabox2f rect(-8, -8, 8, 8);
        *m_quad << rect;
        m_decoration->setLocalBoundingBox(rect);
    }
    m_btnDelete->setEnabled(false);
    m_btnFlipX->setEnabled(true);
    m_btnFlipY->setEnabled(true);
    m_btnLightMode->setEnabled(true);
    m_sbOpacity->setVisible(true);
    m_sbRed->setVisible(true);
    m_sbGreen->setVisible(true);
    m_sbBlue->setVisible(true);
    _updateOptions();
}

void Brush_Decoration::_beginNewAnimation(const vn::str8 &animationPath) {
    if (m_state != kEdit) {
        m_node->clearChildren();
    } else {
        _setEditDecoration(0);
    }
    m_state = kNewAnimation;
    m_decorationFilePath = animationPath;
    m_decoration = vnnew SceneNode();
    m_node->addChild(m_decoration, false);
    m_quad = vnnew Quad();
    m_decoration->addRenderable(m_quad, false);
    
    vn::RefCountedPtr<const AnimationGroup> group = AnimationManager::instance().loadFromANS(m_decorationFilePath.c_str());
    if (!group.null() && group->size()) {
        const Animation *anim = group->at(0);
        vn::aabox2f box(-8, -8, 8, 8);
        if (!anim->frames.empty()) {
            box.reset(anim->frames[0].rect.min_corner);
            box.add_point(anim->frames[0].rect.max_corner);
            for (size_t i = 1; i < anim->frames.size(); ++i) {
                box.add_box(anim->frames[i].rect);
            }
        }
        m_extent.x = vn::math::max_(box.max_corner.x, vn::math::abs(box.min_corner.x));
        m_extent.y = vn::math::max_(box.max_corner.y, vn::math::abs(box.min_corner.y));
        m_extent *= 2.f;
        m_animController->setAnimation(anim);
    }
    
    m_btnDelete->setEnabled(false);
    m_btnFlipX->setEnabled(true);
    m_btnFlipY->setEnabled(true);
    m_btnLightMode->setEnabled(true);
    m_sbOpacity->setVisible(true);
    m_sbRed->setVisible(true);
    m_sbGreen->setVisible(true);
    m_sbBlue->setVisible(true);
    _updateOptions();
    
}

void Brush_Decoration::_cancelNew() {
    m_state = kEdit;
    m_decorationFilePath.clear();
    m_node->clearChildren();
    m_decoration = 0;
    m_quad = 0;
    m_lstImages->setSelectedItem(0);
    m_lstAnimations->setSelectedItem(0);
    m_btnFlipX->setEnabled(false);
    m_btnFlipY->setEnabled(false);
    m_btnLightMode->setEnabled(false);
    m_sbOpacity->setVisible(false);
    m_sbRed->setVisible(false);
    m_sbGreen->setVisible(false);
    m_sbBlue->setVisible(false);
    m_flipX = false;
    m_flipY = false;
    m_lightMode = false;
    m_color = vn::color_t::kWhite;
    _updateOptions();
}

void Brush_Decoration::_updateOptions() {
    if (m_decoration) {
        m_decoration->setScale(vn::vector2f((m_flipX ? -1 : 1),(m_flipY ? -1 : 1)));
        if (m_quad) {
            *m_quad << (m_lightMode ? vn::Render2DInterface::kAlphaAdditive : vn::Render2DInterface::kAlphaBlend);
        }
    }
    m_btnFlipX->setPushed(m_flipX);
    m_btnFlipY->setPushed(m_flipY);
    m_btnLightMode->setPushed(m_lightMode);
    m_sbOpacity->setSliderPosition((vn::f32)m_color.alpha);
    m_sbRed->setSliderPosition((vn::f32)m_color.red);
    m_sbGreen->setSliderPosition((vn::f32)m_color.green);
    m_sbBlue->setSliderPosition((vn::f32)m_color.blue);
}

void Brush_Decoration::_setEditDecoration(MapDecoration *deco) {
    if (m_edit) {
        m_edit->setSelected(false);
        m_edit = 0;
    }
    if (deco) {
        m_flipX = deco->isFlipX();
        m_flipY = deco->isFlipY();
        m_lightMode = deco->isLightMode();
        m_color = deco->getColor();
        m_btnFlipX->setEnabled(true);
        m_btnFlipY->setEnabled(true);
        m_btnLightMode->setEnabled(true);
        m_sbOpacity->setVisible(true);
        m_sbRed->setVisible(true);
        m_sbGreen->setVisible(true);
        m_sbBlue->setVisible(true);
        m_btnDelete->setEnabled(true);
        _updateOptions();
        m_edit = deco;
        deco->setSelected(true);
    } else {
        m_flipX = false;
        m_flipY = false;
        m_lightMode = false;
        m_color = vn::color_t::kWhite;
        m_btnDelete->setEnabled(false);
        m_btnFlipX->setEnabled(false);
        m_btnFlipY->setEnabled(false);
        m_btnLightMode->setEnabled(false);
        m_sbOpacity->setVisible(false);
        m_sbRed->setVisible(false);
        m_sbGreen->setVisible(false);
        m_sbBlue->setVisible(false);
        _updateOptions();
    }
    
}


Command_NewDecoration::Command_NewDecoration(MapDecoration::Type type, const vn::vector2f &pos, const vn::str8 &file, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color)
: m_type(type)
, m_pos(pos)
, m_extent(extent)
, m_file(file)
, m_flipX(flipX)
, m_flipY(flipY)
, m_lightMode(lightMode)
, m_color(color) {
    
}

void Command_NewDecoration::_free() {
    delete this;
}

void Command_NewDecoration::_do() {
    if (m_type == MapDecoration::kImage) {
        m_deco = EditorApp::instance().createDecorationImage(m_pos, m_file, m_flipX, m_flipY, m_lightMode, m_extent, m_color);
    } else if (m_type == MapDecoration::kAnimation) {
        m_deco = EditorApp::instance().createDecorationAnimation(m_pos, m_file, m_flipX, m_flipY, m_lightMode, m_extent, m_color);
    }
}

void Command_NewDecoration::_undo() {
    EditorApp::instance().removeDecoration(m_deco.ptr());
}

void Command_NewDecoration::_redo() {
    EditorApp::instance().addDecoration(m_deco.ptr(), true);
    m_deco->setFlipX(m_flipX);
    m_deco->setFlipY(m_flipY);
    m_deco->setLightMode(m_lightMode);
    m_deco->setColor(m_color);
}

Command_DeleteDecoration::Command_DeleteDecoration(const MapDecorationPtr &deco)
: m_deco(deco) {
    
}

void Command_DeleteDecoration::_free() {
    delete this;
}

void Command_DeleteDecoration::_do() {
    EditorApp::instance().removeDecoration(m_deco.ptr());
}

void Command_DeleteDecoration::_undo() {
    EditorApp::instance().addDecoration(m_deco.ptr(), true);
}

void Command_DeleteDecoration::_redo() {
    EditorApp::instance().removeDecoration(m_deco.ptr());
}


Command_MoveDecoration::Command_MoveDecoration(const MapDecorationPtr &deco, const vn::vector2f &oldPos, const vn::vector2f &newPos)
: m_deco(deco)
, m_oldPos(oldPos)
, m_newPos(newPos) {
    
}

void Command_MoveDecoration::_free() {
    delete this;
}

void Command_MoveDecoration::_do() {
    m_deco->setPosition(m_newPos);
}

void Command_MoveDecoration::_undo() {
    m_deco->setPosition(m_oldPos);
}

void Command_MoveDecoration::_redo() {
    m_deco->setPosition(m_newPos);
}


Command_SizeDecoration::Command_SizeDecoration(const MapDecorationPtr &deco, const vn::vector2f &oldExtent, const vn::vector2f &newExtent)
: m_deco(deco)
, m_oldExtent(oldExtent)
, m_newExtent(newExtent) {
    
}

void Command_SizeDecoration::_free() {
    delete this;
}

void Command_SizeDecoration::_do() {
    m_deco->setExtent(m_newExtent);
}

void Command_SizeDecoration::_undo() {
    m_deco->setExtent(m_oldExtent);
}

void Command_SizeDecoration::_redo() {
    m_deco->setExtent(m_newExtent);
}