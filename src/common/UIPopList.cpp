//
//  UIPopList.cpp
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIPopList.h"
#include "vnUIRoot.h"
#include "vnFontManager.h"
#include "vnLog.h"
#include "vnGfxApplication.h"

VN_IMPL_UI_ELEMENT(UIPopList, "PopList")

UIPopList::UIPopList()
: m_dirty(false)
, m_list(vnnew _List(this))
, m_popped(false)
, m_selectedIndex(-1)
, m_hoverIndex(-1)
, m_listBkgColor(vn::color_t::kWhite)
, m_listItemColor(vn::color_t::kBlack)
, m_listHoverBkgColor(vn::color_t::kBlue)
, m_listHoverItemColor(vn::color_t::kWhite) {
	
}

UIPopList::UIPopList(const UIPopList &other)
: UIElement(other)
, m_dirty(other.m_dirty)
, m_list(vnnew _List(this))
, m_popped(false)
, m_selectedIndex(other.m_selectedIndex)
, m_hoverIndex(-1)
, m_font(other.m_font)
, m_listBkgColor(other.m_listBkgColor)
, m_listItemColor(other.m_listItemColor)
, m_listHoverBkgColor(other.m_listHoverBkgColor)
, m_listHoverItemColor(other.m_listHoverItemColor) {
	size_t c = other.m_items.size();
	m_items.resize(c);
	for (size_t i = 0; i < c; ++i) {
		m_items[i] = vnnew Item(*other.m_items[i]);
	}
}

UIPopList * UIPopList::clone() const {
	return vnnew UIPopList(*this);
}

UIPopList::~UIPopList() {
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		delete *it;
	}
	delete m_list;
}

void UIPopList::onMouseLeftUp(const vn::vector2f &position) {
	if (m_popped) {
		m_popped = false;
		vn::UIRoot::instance().releaseMouse(this);
		if (m_hoverIndex != -1) {
			setSelectedItemIndex(m_hoverIndex);
			m_hoverIndex = -1;
		}
		vn::UIElement *root = vn::UIRoot::instance().getRootElement();
		if (root) {
			root->removeChild(m_list);
		}
	}
}

void UIPopList::onMouseCaptureCancelled() {
	m_popped = false;
	m_hoverIndex = -1;
	vn::UIElement *root = vn::UIRoot::instance().getRootElement();
	if (root) {
		root->removeChild(m_list);
	}
}

void UIPopList::init(const vn::TreeDataObject *object) {
	UIElement::init(object);
	const vn::TreeDataString *tdstr = object->queryAttributeString("font");
    if (tdstr) {
        m_font = vn::FontManager::instance().findFont(tdstr->value());
    } else {
        m_font = vn::FontManager::instance().defaultFont();
    }
	m_dirty = true;
	const vn::TreeDataArray * tda = object->queryAttributeArray("items");
	if (tda) {
		for (size_t i = 0, c = tda->size(); i < c; ++i) {
			tdstr = dynamic_cast<const vn::TreeDataString *>(tda->at(i));
			if (tdstr) {
				addItem(tdstr->value());
			}
		}
	}
}

size_t UIPopList::addItem(const vn::str8 &text, void *data) {
	size_t index = m_items.size();
	Item *item = vnnew Item();
	item->text = text;
	item->renderText.setFont(m_font);
	item->renderText.setText(text);
	item->data = data;
	m_items.push_back(item);
	m_dirty = true;
	return index;
}

void UIPopList::clearItems() {
	setSelectedItemIndex(-1);
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		delete *it;
	}
	m_items.clear();
	m_dirty = true;
}

void UIPopList::setItemText(size_t index, const vn::str8 &text) {
	m_items[index]->text = text;
	m_items[index]->renderText.setText(text);
}

void UIPopList::setItemData(size_t index, void *data) {
	m_items[index]->data = data;
}

void * UIPopList::getItemData(size_t index) const {
	return m_items[index]->data;
}

const vn::str8 & UIPopList::getItemText(size_t index) const {
	return m_items[index]->text;
}

size_t UIPopList::getItemIndexByData(void *data) const {
	for (size_t i = 0, c = m_items.size(); i < c; ++i) {
		if (m_items[i]->data == data) {
			return i;
		}
	}
	return -1;
}

size_t UIPopList::itemCount() const {
	return m_items.size();
}

void UIPopList::setSelectedItemIndex(size_t index) {
	if (m_selectedIndex != index) {
		m_selectedIndex = index;
		if (!m_fnSelectedChanged.null()) {
			m_fnSelectedChanged(this, index);
		}
	}
}

size_t UIPopList::getSelectedItemIndex() const {
	return m_selectedIndex;
}

void UIPopList::setFont(const vn::FontPtr &font) {
	m_font = font;
	m_dirty = true;
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->renderText.setFont(m_font);
	}
}

const vn::FontPtr & UIPopList::getFont() const {
	return m_font;
}

void UIPopList::pop() {
	if (m_popped) {
		return ;
	}
	m_popped = true;
	vn::UIRoot::instance().captureMouse(this);
	vn::UIElement *root = vn::UIRoot::instance().getRootElement();
	if (root) {
		root->addChild(m_list);
	}
}

void UIPopList::bindAction_SelectedChanged(const vn::function<void(vn::UIElement *, size_t)> &func) {
	m_fnSelectedChanged = func;
}

void UIPopList::_onUpdate(vn::f32 deltaTime) {
	if (m_dirty) {
		m_dirty = false;
		m_listBox.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y);
		m_listBox.max_corner.x = m_boundingBox.max_corner.x;
		
		vn::f32 height = 0;
		if (!m_font.null()) {
			height = m_font->height();
		}
		vn::f32 top = m_listBox.min_corner.y;
		for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
			Item &item = **it;
			item.rect.min_corner.set(m_listBox.min_corner.x, top);
			item.rect.max_corner.set(m_listBox.max_corner.x, top += height);
		}
		m_listBox.max_corner.y = top;
	}
	
	if (!m_popped) {
		return ;
	}
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->renderText.update();
	}
	vn::vector2f pt = vn::GfxApplication::instance().getMousePosition();
	if (!m_font.null() && pt >= m_listBox.min_corner && pt < m_listBox.max_corner) {
		m_hoverIndex = (size_t)((pt.y - m_listBox.min_corner.y) / m_font->height());
		if (m_hoverIndex >= m_items.size()) {
			m_hoverIndex = -1;
		}
	} else {
		m_hoverIndex = -1;
	}
}

void UIPopList::_onListRender(vn::UIRenderer *renderer) {
	renderer->drawRect(m_listBox, m_listBkgColor);
	renderer->pushClippingBox(m_listBox);
	for (size_t i = 0, c = m_items.size(); i < c; ++i) {
		Item &item = *m_items[i];
		if (i == m_hoverIndex) {
			renderer->drawRect(item.rect, m_listHoverBkgColor);
			item.renderText.render(renderer, item.rect.min_corner, m_listHoverItemColor);
		} else {
			item.renderText.render(renderer, item.rect.min_corner, m_listItemColor);
		}
	}
	renderer->popClippingBox();
}

void UIPopList::_onBoundingBoxUpdated() {
	UIElement::_onBoundingBoxUpdated();
	m_dirty = true;
}

bool UIPopList::_bindAction(const vn::c8 *name, vn::RefCounted *func_impl) {
	VN_UI_ELEMENT_BIND_ACTION(UIPopList, "changed", m_fnSelectedChanged, name, func_impl)
	return UIElement::_bindAction(name, func_impl);
}

UIPopList::_List::_List(UIPopList *owner)
: m_owner(owner) {}

void UIPopList::_List::_onRender(vn::UIRenderer *renderer) {
	m_owner->_onListRender(renderer);
}