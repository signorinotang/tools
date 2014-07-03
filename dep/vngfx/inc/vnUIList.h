//
//  vnUIList.h
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIList_h
#define vngfx_vnUIList_h

#include "vnUIScrollBar.h"
#include <vector>

_vn_begin

class UIListItem;

class _vn_gfx_api UIList : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    UIList();
    UIList(const UIList &other);
    virtual UIList * clone() const;
    virtual ~UIList();
    
    void setScrollBar(UIScrollBar *scrollBar, bool grab = true);
    void setScrollBarSize(f32 size);
    
    size_t itemCount() const;
    
    UIListItem * addItem();
    size_t addItem(UIListItem *item, bool grab = true);
    
    UIListItem * insertItem(size_t index);
    void insertItem(size_t index, UIListItem *item, bool grab = true);
    
    void eraseItem(size_t index);
    void clearItems();
    
    UIListItem * item(size_t index);
    size_t itemIndex(UIListItem *item) const;
    
    UIListItem * getSelectedItem() const;
    void setSelectedItem(UIListItem *item);
    
    UIListItem * pickItem(const vector2f &point);
    
    virtual UIElement * pick(const vector2f &point);
    virtual void init(const TreeDataObject *object);
    
    void refresh();
    void makeItemVisible(UIListItem *item);
    
    // named "changed"
    void bindAction_SelectedChanged(const function<void(UIElement *, UIListItem *)> &func);
    // named "clicked"
    void bindAction_ItemClicked(const function<void(UIElement *, UIListItem *)> &func);
    
    virtual void onMouseLeftDown(const vector2f &pos);
    virtual void onMouseLeftUp(const vector2f &pos);
    virtual void onMouseCaptureCancelled();
    virtual void onMouseWheel(const vector2f &pos, const vector2f &delta);
    
    
    virtual void onSelectedChanged();
    virtual void onItemClicked(UIListItem *item);
protected:
    virtual void _onBoundingBoxUpdated();
	virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
    virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
    
    UIScrollBar *m_scrollBar;
    f32 m_scrollBarSize;
    ScrollType m_style;
    
    typedef std::vector<UIListItem *> Items;
    Items m_items;
    bool m_viewDirty;
    UIListItem *m_itemPrototype;
    f32 m_viewOffset, m_viewLength;
    aabox2f m_viewRect;
    UIListItem *m_selectedItem, *m_mouseDownItem;
    bool m_mouseDown;
    
    function<void(UIElement *, UIListItem *)> m_fnSelectedChanged;
    function<void(UIElement *, UIListItem *)> m_fnItemClicked;
    
    void _updateHorizontalItems();
    void _updateVerticalItems();
    void _updateScrollBarRect();
    
    void _onScrolling(UIElement *, f32);
};

_vn_end


#endif
