//
//  vnUIList.cpp
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIList.h"
#include "vnUIListItem.h"
#include "vnUIRenderer.h"
#include "vnUIRoot.h"
#include "vnLog.h"

_vn_begin

UIList::UIList()
: m_style(kST_Default)
, m_itemPrototype(0)
, m_scrollBar(0)
, m_scrollBarSize(0)
, m_viewDirty(false)
, m_viewLength(0)
, m_viewOffset(0)
, m_selectedItem(0)
, m_mouseDown(false)
, m_mouseDownItem(0) {
    
}

UIList::UIList(const UIList &other)
: UIElement(other)
, m_style(other.m_style)
, m_itemPrototype(other.m_itemPrototype ? other.m_itemPrototype->clone() : 0)
, m_scrollBar(other.m_scrollBar ? other.m_scrollBar->clone() : 0)
, m_scrollBarSize(other.m_scrollBarSize)
, m_viewDirty(other.m_viewDirty)
, m_viewLength(other.m_viewLength)
, m_viewOffset(other.m_viewOffset)
, m_selectedItem(0)
, m_mouseDown(false)
, m_mouseDownItem(0) {
    if (m_scrollBar) {
        m_scrollBar->bindAction_Scrolling(make_memfn(this, &UIList::_onScrolling));
    }
    m_items.reserve(other.m_items.size());
    for (Items::const_iterator it = other.m_items.begin(); it != other.m_items.end(); ++it) {
        UIListItem *item = (*it)->clone();
        item->startup();
        item->setList(this);
        m_items.push_back(item);
        if (*it == other.m_selectedItem) {
            m_selectedItem = item;
        }
    }
}

UIList * UIList::clone() const {
    return vnnew UIList(*this);
}

UIList::~UIList() {
    clearItems();
    VN_SAFE_DROP(m_scrollBar);
    VN_SAFE_DROP(m_itemPrototype);
}

void UIList::setScrollBar(vn::UIScrollBar *scrollBar, bool grab) {
    if (m_scrollBar) {
        m_scrollBar->drop();
    }
    if ((m_scrollBar = scrollBar)) {
        if (grab) {
            m_scrollBar->grab();
        }
        m_scrollBar->setVisible(false);
        m_scrollBar->setStyle(m_style);
        m_scrollBar->bindAction_Scrolling(make_memfn(this, &UIList::_onScrolling));
        _updateScrollBarRect();
    }
}

void UIList::setScrollBarSize(f32 size) {
    if (m_scrollBarSize != size) {
        m_scrollBarSize = size;
        _updateScrollBarRect();
    }
}

size_t UIList::itemCount() const {
    return m_items.size();
}

UIListItem * UIList::addItem() {
    if (!m_itemPrototype) {
        return 0;
    }
    UIListItem *item = m_itemPrototype->clone();
    item->startup();
    item->setList(this);
    m_items.push_back(item);
    m_viewDirty = true;
    return item;
}

UIListItem * UIList::insertItem(size_t index) {
    vnassert(index <= m_items.size());
    if (!m_itemPrototype) {
        return 0;
    }

    
    UIListItem *item = m_itemPrototype->clone();
    item->startup();
    item->setList(this);
    m_items.insert(m_items.begin() + index, item);
    m_viewDirty = true;
    return item;
}

size_t UIList::addItem(UIListItem *item, bool grab) {
    vnassert(item);
    if (grab) {
        item->grab();
    }
    size_t idx = m_items.size();
    item->setList(this);
    m_items.push_back(item);
    m_viewDirty = true;
    return idx;
}

void UIList::insertItem(size_t index, UIListItem *item, bool grab) {
    vnassert(item && index <= m_items.size());
    if (grab) {
        item->grab();
    }
    item->setList(this);
    m_items.insert(m_items.begin() + index, item);
    m_viewDirty = true;
}

void UIList::eraseItem(size_t index) {
    if (index < m_items.size()) {
        Items::iterator it = m_items.begin() + index;
        if (*it == m_selectedItem) {
            setSelectedItem(0);
        }
        (*it)->setList(0);
        (*it)->drop();
        m_items.erase(it);
        m_viewDirty = true;
    }
}

void UIList::clearItems() {
    m_viewOffset = 0;
    setSelectedItem(0);
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        (*it)->setList(0);
        (*it)->drop();
    }
    m_items.clear();
    m_viewDirty = true;
}

UIListItem * UIList::item(size_t index) {
    vnassert(index < m_items.size());
    return m_items[index];
}

size_t UIList::itemIndex(UIListItem *item) const {
    for (size_t i = 0, c = m_items.size(); i < c; ++i) {
        if (m_items[i] == item) {
            return i;
        }
    }
    return -1;
}

void UIList::setSelectedItem(UIListItem *item) {
    if (m_selectedItem == item) {
        return ;
    }
    if (m_selectedItem) {
        m_selectedItem->onUnselected();
    }
    if ((m_selectedItem = item)) {
        m_selectedItem->onSelected();
    }
    onSelectedChanged();
}

UIListItem * UIList::getSelectedItem() const {
    return m_selectedItem;
}

UIListItem * UIList::pickItem(const vector2f &point) {
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        UIListItem *item = *it;
        UIElement *e = item->root();
        if (!e->isVisible()) {
            continue;
        }
        if (point >= e->boundingBox().min_corner && point < e->boundingBox().max_corner) {
            return item;
        }
    }
    return 0;
}

UIElement * UIList::pick(const vector2f &point) {
    UIElement *ret = UIElement::pick(point);
    if (ret != this) {
        return ret;
    }
    if (m_scrollBar && m_scrollBar->isVisible() && m_scrollBar->isEnabled()) {
        ret = m_scrollBar->pick(point);
        if (ret) {
            return ret;
        }
    }
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        UIElement *item = (*it)->root();
        if (item->isVisible() && item->isEnabled()) {
            ret = item->pick(point);
            if (ret) {
                return ret;
            }
        }
    }
    return this;
}

void UIList::init(const TreeDataObject *object) {
    UIElement::init(object);
    
    m_style = (ScrollType)object->queryAttributeInteger("style");
    
    const TreeDataObject *tdobj = object->queryAttributeObject("item_prototype");
    if (tdobj) {
        UILogic *p = UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_itemPrototype = dynamic_cast<UIListItem *>(p);
            if (!m_itemPrototype) {
                p->drop();
            }
        }
    }
    setScrollBarSize(object->queryAttributeFloat("scroll_bar_size", m_scrollBarSize));
    tdobj = object->queryAttributeObject("scroll_bar");
    if (tdobj) {
        UIElement *p = UIFactory::instance().createElement(tdobj);
        if (p) {
            setScrollBar(dynamic_cast<UIScrollBar *>(p));
            p->drop();
        }
    }
    
    const TreeDataArray *tda = object->queryAttributeArray("items");
    if (m_itemPrototype && tda) {
        for (size_t i = 0, c = tda->size(); i < c; ++i) {
            const TreeDataObject *data = dynamic_cast<const TreeDataObject *>(tda->at(i));
            if (!data) {
                continue;
            }
            UIListItem *item = addItem();
            item->set(data);
        }
    }
    
}

void UIList::refresh() {
    m_viewDirty = true;
}

void UIList::makeItemVisible(UIListItem *item) {
    vnassert(item && item->getList() == this);
    if (m_viewDirty) {
        if (m_style == kST_Horizontal) {
            _updateHorizontalItems();
        } else {
            _updateVerticalItems();
        }
    } else {
        m_viewDirty = true;
    }
    const aabox2f &rect = item->root()->boundingBox();
    if (m_style == kST_Horizontal) {
        f32 delta = m_boundingBox.max_corner.x - rect.max_corner.x;
        if (delta < 0) {
            m_viewOffset += delta;
        }
        delta = m_boundingBox.min_corner.x - rect.min_corner.x;
        if (delta > 0) {
            m_viewOffset += delta;
        }
    } else {
        f32 delta = m_boundingBox.max_corner.y - rect.max_corner.y;
        if (delta < 0) {
            m_viewOffset += delta;
        }
        delta = m_boundingBox.min_corner.y - rect.min_corner.y;
        if (delta > 0) {
            m_viewOffset += delta;
        }
    }
}


void UIList::bindAction_SelectedChanged(const function<void(UIElement *, UIListItem *)> &func) {
    m_fnSelectedChanged = func;
}

void UIList::bindAction_ItemClicked(const function<void(UIElement *, UIListItem *)> &func) {
    m_fnItemClicked = func;
}

void UIList::onMouseLeftDown(const vector2f &pos) {
    m_mouseDown = true;
    UIRoot::instance().captureMouse(this);
    m_mouseDownItem = pickItem(pos);
}

void UIList::onMouseLeftUp(const vector2f &pos) {
    if (m_mouseDown) {
        m_mouseDown = false;
        UIRoot::instance().releaseMouse(this);
        UIListItem *item = pickItem(pos);
        if (item == m_mouseDownItem) {
            onItemClicked(item);
        }
        m_mouseDownItem = 0;
    }
}

void UIList::onMouseCaptureCancelled() {
    m_mouseDown = false;
    m_mouseDownItem = 0;
}

void UIList::onMouseWheel(const vector2f &pos, const vector2f &delta) {
    if (m_scrollBar && m_scrollBar->isVisible()) {
        m_scrollBar->onMouseWheel(pos, delta);
    }
}

void UIList::_onBoundingBoxUpdated() {
    UIElement::_onBoundingBoxUpdated();
    m_viewDirty = true;
    _updateScrollBarRect();
}

void UIList::onSelectedChanged() {
    if (!m_fnSelectedChanged.null()) {
        m_fnSelectedChanged(this, m_selectedItem);
    }
}

void UIList::onItemClicked(UIListItem *item) {
    if (!m_fnItemClicked.null()) {
        m_fnItemClicked(this, item);
    }
}

void UIList::_onUpdate(f32 deltaTime) {
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        (*it)->update(deltaTime);
    }
    if (m_viewDirty) {
        m_viewDirty = false;
        if (m_style == kST_Horizontal) {
            _updateHorizontalItems();
        } else {
            _updateVerticalItems();
        }
    }
    if (m_scrollBar) {
        m_scrollBar->update(deltaTime);
    }
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        (*it)->root()->update(deltaTime);
    }
}

void UIList::_onRender(UIRenderer *renderer) {
    renderer->pushClippingBox(m_viewRect);
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        UIElement *item = (*it)->root();
        if (item->isVisible()) {
            item->render(renderer);
        }
    }
    renderer->popClippingBox();
    if (m_scrollBar && m_scrollBar->isVisible()) {
        m_scrollBar->render(renderer);
    }
}

bool UIList::_bindAction(const c8 *name, vn::RefCounted *func_impl) {
    VN_UI_ELEMENT_BIND_ACTION(UIList, "changed", m_fnSelectedChanged, name, func_impl)
    VN_UI_ELEMENT_BIND_ACTION(UIList, "clicked", m_fnItemClicked, name, func_impl)
    return UIElement::_bindAction(name, func_impl);
}

void UIList::_updateScrollBarRect() {
    if (!m_scrollBar) {
        return ;
    }
    aabox2f rect;
    rect.max_corner = m_boundingBox.max_corner;
    if (m_style == kST_Horizontal) {
        rect.min_corner.set(m_boundingBox.min_corner.x, m_boundingBox.max_corner.y - m_scrollBarSize);
    } else {
        rect.min_corner.set(m_boundingBox.max_corner.x - m_scrollBarSize, m_boundingBox.min_corner.y);
    }
    m_scrollBar->setLocation(rect);
}

void UIList::_updateHorizontalItems() {
    m_viewRect = m_boundingBox;
    vector2f size = m_boundingBox.size();
    m_viewLength = 0;
    bool scroll = false;
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        m_viewLength += (*it)->calculateWidth();
    }
    if (m_viewLength > size.x) {
        scroll = true;
        if (m_viewLength + m_viewOffset < size.x) {
            m_viewOffset = m_viewLength - size.x;
        }
        size.y -= m_scrollBarSize;
    } else {
        m_viewOffset = 0;
    }
    
    vector2f pos(m_boundingBox.min_corner.x + m_viewOffset, m_boundingBox.min_corner.y);
    
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        UIListItem *item = *it;
        UIElement *e = item->root();
        item->setPositionAndHeight(pos, size.y);
        if (e->boundingBox().max_corner.x < m_boundingBox.min_corner.x || e->boundingBox().min_corner.x >= m_boundingBox.max_corner.x) {
            e->setVisible(false);
        } else {
            e->setVisible(true);
        }
        pos.x = e->boundingBox().max_corner.x;
    }
    
    if (m_scrollBar) {
        if (scroll) {
            m_scrollBar->setVisible(true);
            m_scrollBar->setSliderRange(m_viewLength - size.x);
            m_scrollBar->setSliderSize(size.x);
            m_scrollBar->setSliderPosition(-m_viewOffset);
            m_viewRect.max_corner.y -= m_scrollBarSize;
        } else {
            m_scrollBar->setVisible(false);
        }
    }
}


void UIList::_updateVerticalItems() {
    m_viewRect = m_boundingBox;
    vector2f size = m_boundingBox.size();
    m_viewLength = 0;
    bool scroll = false;
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        m_viewLength += (*it)->calculateHeight();
    }
    if (m_viewLength > size.y) {
        scroll = true;
        size.x -= m_scrollBarSize;
        if (m_viewLength + m_viewOffset < size.y) {
            m_viewOffset = size.y - m_viewLength;
        }
    } else {
        m_viewOffset = 0;
    }
    
    vector2f pos(m_boundingBox.min_corner.x, m_boundingBox.min_corner.y + m_viewOffset);
    
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        UIListItem *item = *it;
        UIElement *e = item->root();
        item->setPositionAndWidth(pos, size.x);
        if (e->boundingBox().max_corner.y < m_boundingBox.min_corner.y || e->boundingBox().min_corner.y >= m_boundingBox.max_corner.y) {
            e->setVisible(false);
        } else {
            e->setVisible(true);
        }
        pos.y = e->boundingBox().max_corner.y;
    }
    
    if (m_scrollBar) {
        if (scroll) {
            m_scrollBar->setVisible(true);
            m_scrollBar->setSliderRange(m_viewLength - size.y);
            m_scrollBar->setSliderSize(size.x);
            m_scrollBar->setSliderPosition(-m_viewOffset);
            m_viewRect.max_corner.x -= m_scrollBarSize;
        } else {
            m_scrollBar->setVisible(false);
        }
    }
}

void UIList::_onScrolling(vn::UIElement *sender, f32 pos) {
    if (sender != m_scrollBar) {
        return ;
    }
    pos = -pos;
    if (pos != m_viewOffset) {
        m_viewOffset = pos;
        m_viewDirty = true;
    }
}


_vn_end