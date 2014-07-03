//
//  vnUIFactory.cpp
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIFactory.h"
#include "vnLog.h"
#include "vnFileManager.h"
#include "vnTreeDataScript.h"

#include "vnUIElement.h"
#include "vnUIFrame.h"
#include "vnUIPicture.h"
#include "vnUIButton.h"
#include "vnUIButtonDefault.h"
#include "vnUILabel.h"
#include "vnUITextField.h"
#include "vnUIScrollBar.h"
#include "vnUIList.h"
#include "vnUIListItem.h"
#include "vnUIWindow.h"

#include "vnUILocator.h"
#include "vnUILocatorRect.h"
#include "vnUILocatorBorder.h"
#include "vnUILocatorBox.h"

#include "vnUIRenderPicture.h"

_vn_begin

////////////////////////////////////////
VN_IMPL_UI_ELEMENT(UIElement, "Element")
VN_IMPL_UI_ELEMENT(UIFrame, "Frame")
VN_IMPL_UI_ELEMENT(UIPicture, "Picture")
VN_IMPL_UI_ELEMENT(UIButton, "Button")
VN_IMPL_UI_ELEMENT(UIButtonDefault, "ButtonDefault")
VN_IMPL_UI_ELEMENT(UILabel, "Label")
VN_IMPL_UI_ELEMENT(UITextField, "TextField")
VN_IMPL_UI_ELEMENT(UIScrollBar, "ScrollBar")
VN_IMPL_UI_ELEMENT(UIList, "List")
VN_IMPL_UI_ELEMENT(UIWindow, "Window")
////////////////////////////////////////
VN_IMPL_UI_LOCATOR(UILocatorRect, "Rect")
VN_IMPL_UI_LOCATOR(UILocatorRectEx, "RectEx")
VN_IMPL_UI_LOCATOR(UILocatorBox, "Box")
VN_IMPL_UI_LOCATOR(UILocatorBoxEx, "BoxEx")
VN_IMPL_UI_LOCATOR(UILocatorBorder, "Border")
////////////////////////////////////////
VN_IMPL_UI_RENDER_PICTURE(UIRenderPictureEmpty, "ImageEmpty")
VN_IMPL_UI_RENDER_PICTURE(UIRenderPicture0, "Image0")
VN_IMPL_UI_RENDER_PICTURE(UIRenderPicture1, "Image1")
VN_IMPL_UI_RENDER_PICTURE(UIRenderPicture3V, "Image3V")
VN_IMPL_UI_RENDER_PICTURE(UIRenderPicture3H, "Image3H")
VN_IMPL_UI_RENDER_PICTURE(UIRenderPicture9, "Image9")
////////////////////////////////////////
VN_IMPL_UI_LOGIC(UIListItemText, "ListItemText")
////////////////////////////////////////

void UIFactory::_register(const c8 *name, UIElement*(*creator)()) {
    m_factoryE[name] = creator;
}

void UIFactory::_register(const c8 *name, UILocator *(*creator)(UIElement *)) {
    m_factoryL[name] = creator;
}

void UIFactory::_register(const c8 *name, UIRenderPicture *(*creator)()) {
	m_factoryRP[name] = creator;
}

void UIFactory::_register(const c8 *name, vn::UILogic *(*creator)()) {
    m_factoryO[name] = creator;
}

UIElement * UIFactory::createElement(const str8 &name) {
    ElementFactory::iterator it = m_factoryE.find(name);
    if (it == m_factoryE.end()) {
        return 0;
    }
    return it->second();
}

UIElement * UIFactory::createElement(const TreeDataObject *object) {
    vnassert(object);
    UIElement *p = createElement(object->name());
    if (!p) {
        VN_LOG_ERROR("UIFactory: unknown element \"" << object->name() << "\"");
        return 0;
    }
    p->init(object);
    return p;
}

UIElement * UIFactory::createElement(const TreeDataNode *node) {
    vnassert(node);
    UIElement *p = createElement((const TreeDataObject *)node);
    if (!p) {
        return 0;
    }
    createElements(p, node->firstChild());
    return p;
}

UILocator * UIFactory::createLocator(const str8 &name, UIElement *owner) {
    vnassert(owner != 0);
    LocatorFactory::iterator it = m_factoryL.find(name);
    if (it == m_factoryL.end()) {
        return 0;
    }
    return it->second(owner);
}

UIRenderPicture * UIFactory::createRenderPicture(const str8 &name) {
	RenderPictureFactory::iterator it = m_factoryRP.find(name);
	if (it == m_factoryRP.end()) {
		return 0;
	}
	return it->second();
}

UIRenderPicture * UIFactory::createRenderPicture(const TreeDataObject *object) {
	vnassert(object);
	UIRenderPicture *pic = createRenderPicture(object->name());
	if (!pic) {
		VN_LOG_ERROR("UIFactory: unknown image \"" << object->name() << "\"");
		return 0;
	}
	pic->init(object);
	return pic;
}

UILogic * UIFactory::createLogic(const str8 &name) {
    LogicFactory::iterator it = m_factoryO.find(name);
    if (it == m_factoryO.end()) {
        return 0;
    }
    return it->second();
}

UILogic * UIFactory::createLogic(const TreeDataObject *object) {
    UILogic *p = createLogic(object->name());
    if (!p) {
        VN_LOG_ERROR("UIFactory: unknown logic \"" << object->name() << "\"");
		return 0;
    }
    p->init(object);
    return p;
}

UILogic * UIFactory::createLogicFromTDL(const vn::FilePath &path) {
    TreeDataScript tds;
    TreeDataNode *node = tds.parse(path);
    if (!node) {
        VN_LOG_ERROR("UIFactory: tdl parser error.");
        VN_LOG_ERROR(tds.error());
        return 0;
    }
    UILogic *logic = createLogic(node);
    node->drop();
    return logic;
}

void UIFactory::createElements(UIElement *root, const TreeDataNode *node) {
    vnassert(root);
    while (node) {
        UIElement *p = createElement((TreeDataObject *)node);
        if (p) {
            root->addChild(p, false);
            createElements(p, node->firstChild());
        }
        node = node->nextNeighbour();
    }
}

bool UIFactory::createElementsFromBinaryTDL(vn::UIElement *root, const vn::FilePath &path) {
    FileStream *fs = FileManager::instance().open(path);
    if (!fs) {
        VN_LOG_ERROR("UIFactory: open file \"" << path.fsId << ":/" << path.fileName << "\" failed.");
        return false;
    }
    TreeData *node = 0;
    if (!TreeData::load(*fs, node) || node->type() != TreeData::kNode) {
        fs->close();
        VN_LOG_ERROR("UIFactory: read file \"" << path.fsId << ":/" << path.fileName << "\" failed.");
        return false;
    }
    fs->close();
    createElements(root, (TreeDataNode *)node);
    return true;
}

bool UIFactory::createElementsFromTDL(vn::UIElement *root, const vn::FilePath &path) {
    TreeDataScript tds;
    TreeDataNode *node = tds.parse(path);
    if (!node) {
        VN_LOG_ERROR("UIFactory: tdl parser error.");
        VN_LOG_ERROR(tds.error());
        return false;
    }
    createElements(root, node);
    node->drop();
    return true;
}

_vn_end