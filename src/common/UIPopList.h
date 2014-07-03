//
//  UIPopList.h
//  test
//
//  Created by Wayne on 14-1-4.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIPopList__
#define __test__UIPopList__

#include "vnUIElement.h"
#include "vnUIRenderText.h"

class UIPopList : public vn::UIElement {
	VN_DECL_UI_ELEMENT()
public:
	UIPopList();
	UIPopList(const UIPopList &other);
	virtual UIPopList * clone() const;
	virtual ~UIPopList();
	
    virtual void onMouseLeftUp(const vn::vector2f &position);
	virtual void onMouseCaptureCancelled();
	
	virtual void init(const vn::TreeDataObject *object);
	
	size_t addItem(const vn::str8 &text, void *data = 0);
	void clearItems();
	void setItemText(size_t index, const vn::str8 &text);
	void setItemData(size_t index, void *data);
	void * getItemData(size_t index) const;
	const vn::str8 & getItemText(size_t index) const;
	size_t getItemIndexByData(void *data) const;
	size_t itemCount() const;
	
	void setSelectedItemIndex(size_t index);
	size_t getSelectedItemIndex() const;
	
	void setFont(const vn::FontPtr &font);
	const vn::FontPtr & getFont() const;
	
	void pop();
	
	void bindAction_SelectedChanged(const vn::function<void(vn::UIElement *, size_t)> &func);
	
protected:
	virtual void _onUpdate(vn::f32 deltaTime);
	virtual void _onBoundingBoxUpdated();
	virtual bool _bindAction(const vn::c8 *name, vn::RefCounted *func_impl);
	
	void _onListRender(vn::UIRenderer *renderer);
	
	vn::aabox2f m_listBox;
	
	struct Item : vn::AllocatedObject {
		vn::UIRenderText renderText;
		vn::str8 text;
		vn::aabox2f rect;
		void *data;
	};
	typedef std::vector<Item *> Items;
	Items m_items;
	bool m_dirty;
	bool m_popped;
	size_t m_selectedIndex;
	size_t m_hoverIndex;
	
	vn::color_t m_listBkgColor, m_listItemColor, m_listHoverBkgColor, m_listHoverItemColor;
	vn::FontPtr m_font;
	
	vn::function<void(vn::UIElement *, size_t)> m_fnSelectedChanged;
	
	class _List : public vn::UIElement {
	public:
		_List(UIPopList *owner);
	protected:
		virtual void _onRender(vn::UIRenderer *renderer);
	private:
		UIPopList *m_owner;
	} *m_list;
	friend class _List;
};

#endif /* defined(__test__UIPopList__) */
