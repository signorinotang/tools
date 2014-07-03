//
//  vnUIElement.h
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIElement_h
#define vngfx_vnUIElement_h


#include "vnUIFactory.h"
#include "vnRefCounted.h"
#include "vnAABox2.h"
#include "vnFunction.h"

#include <vector>

_vn_begin

class UIRenderer;

class _vn_gfx_api UIElement : public RefCounted {
    VN_DECL_UI_ELEMENT()
    friend class UILocator;
public:
    // ctor & dtor
    UIElement();
    UIElement(const UIElement &other);
    virtual UIElement * clone() const;
    virtual ~UIElement();
    
    // properties
	UIElement * parent() const;
	const aabox2f & boundingBox() const;
	UILocator * locator() const;
	
    void setName(const str8 &name);
    const str8 & getName() const;
    
	virtual void setVisible(bool visible);
	bool isVisible() const;
	
	virtual void setEnabled(bool enabled);
	bool isEnabled() const;
	
	bool isClipping() const;
	void setClipping(bool clipping);
	
	void setTag(u32 tag);
	u32 getTag() const;
    
    // methods
	void setLocation(const aabox2f &location);
	void update(f32 deltaTime);
	void render(UIRenderer *renderer);
	void bringToTop();
	u32 addChild(UIElement *child, bool grab = true);
	void removeChild(UIElement *child);
	void removeChildByIndex(u32 index);
    void removeAllChildren();
	void bringChildToTop(u32 index);
	void bringChildToTop(UIElement *child);
	u32 getChildIndex(UIElement *child);
	UIElement * getChildByIndex(u32 index);
	UIElement * getChildByTag(u32 tag);
	u32 getNumChildren() const;
    
    virtual bool contains(const vector2f &point) const;
	virtual UIElement * pick(const vector2f &point);
    virtual void init(const TreeDataObject *object);
    virtual void buildElementMap(std::map<str8, UIElement *> &namedElements);
    
    // events
    virtual void onMouseLeftDown(const vector2f &position);
    virtual void onMouseLeftUp(const vector2f &position);
    
    virtual void onMouseRightDown(const vector2f &position);
    virtual void onMouseRightUp(const vector2f &position);
    
    virtual void onMouseWheel(const vector2f &position, const vector2f &delta);
    
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseCaptureCancelled();
    virtual void onFocusIn();
    virtual void onFocusOut();
    
    virtual void onKeyDown(c32 code, bool repeat);
    virtual void onKeyUp(c32 code);
    
    virtual void onTextInsert(const str32 &text);
    virtual void onTextMark(const str32 &text, u32 caretPos);
    virtual void onTextUnmark();
    virtual void onTextNewLine();
    virtual void onTextDeleteBackward();
    virtual void onTextDeleteForward();
	virtual void onTextMoveUp();
	virtual void onTextMoveDown();
	virtual void onTextMoveLeft();
	virtual void onTextMoveRight();
    
    template <typename T>
    bool bindAction(const c8 *name, const function<T> &func) {
        if (!this) {
            return false;
        }
        return _bindAction(name, func.impl_ptr());
    }
    
protected:
	virtual void _onBoundingBoxUpdated();
	virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
	
    virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
    
    str8 m_name;
	bool m_visible;
	bool m_enabled;
	bool m_clipping;
	u32 m_tag;
	
	
	aabox2f m_boundingBox;
	UIElement *m_parent;
	UILocator *m_locator;
	
	typedef std::vector<UIElement *> Elements;
	Elements m_children;
};

typedef RefCountedPtr<UIElement> UIElementPtr;

_vn_end

#define VN_UI_ELEMENT_BIND_ACTION(cls, name, dest, src_name, src_ptr) \
	if (!strcmp(name, src_name)) {\
		dest.set(src_ptr);\
		if (src_ptr && dest.null()) {\
			VN_LOG_ERROR(#cls << ": bind action \"" << name << "\" failed.");\
			return false;\
		}\
		return true;\
	}

#endif