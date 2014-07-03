//
//  vnUIScrollBar.h
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIScrollBar_h
#define vngfx_vnUIScrollBar_h

#include "vnUIElement.h"
#include "vnUIRenderPicture.h"

_vn_begin

class _vn_gfx_api UIScrollBar : public UIElement {
	VN_DECL_UI_ELEMENT()
public:
	UIScrollBar();
	UIScrollBar(const UIScrollBar &other);
	virtual UIScrollBar * clone() const;
	
    void setStyle(ScrollType st);
	ScrollType getStyle() const;
	
	void setSliderPosition(f32 position);
	f32 getSliderPosition() const;
	void setSliderRange(f32 range);
	f32 getSliderRange() const;
	void setSliderSize(f32 size);
	f32 getSliderSize() const;
	void setSliderUnit(f32 unit);
	f32 getSliderUnit() const;
    
    void setSliderInterval(f32 interval);
    f32 getSliderInterval() const;
	
	void slide(f32 delta);
	
	virtual void init(const TreeDataObject *object);
	virtual void bindAction_Scrolling(const function<void(UIElement *, f32)> &func);
    
    // events
    
    virtual void onMouseLeftDown(const vector2f &pos);
    virtual void onMouseLeftUp(const vector2f &pos);
	virtual void onMouseWheel(const vector2f &pos, const vector2f &delta);
	
    
    virtual void onScrolling();
    
protected:
	virtual void _onBoundingBoxUpdated();
	virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
	virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
	
	ScrollType m_style;
	UIRenderPicturePtr m_bkgPicture;
	color_t m_bkgColor;
	aabox2f m_sliderRect;
	UIRenderPicturePtr m_sliderPicture;
	color_t m_sliderColor;
	bool m_sliderDirty;
	f32 m_sliderUnit;
	f32 m_sliderRange;
	f32 m_sliderPos;
	f32 m_sliderSize;
    f32 m_sliderInterval;
    bool m_sliderLimited;
    
    bool m_mouseDown;
    f32 m_mouseDownOffCenter;
	
    function<void(UIElement *, f32)> m_fnScrolling;
};

_vn_end

#endif
