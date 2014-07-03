//
//  vnUILocatorBorder.h
//  vngfx
//
//  Created by Wayne on 13-12-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILocatorBorder_h
#define vngfx_vnUILocatorBorder_h

#include "vnUILocator.h"

_vn_begin

class _vn_gfx_api UILocatorBorder : public UILocator {
	VN_DECL_UI_LOCATOR()
public:
	UILocatorBorder(UIElement *owner);
	UILocatorBorder(const UILocatorBorder &other, UIElement *owner);
	virtual UILocatorBorder * clone(UIElement *owner) const;
	
	virtual void init(const TreeDataObject *object);
	virtual void _set(const aabox2f &location);
	
	void setBorder(f32 left, f32 top, f32 right, f32 bottom);
	
protected:
	virtual void _updateBoundingBox();
	
	vector2f m_nearDistance;
	vector2f m_farDistance;
};

_vn_end

#endif
