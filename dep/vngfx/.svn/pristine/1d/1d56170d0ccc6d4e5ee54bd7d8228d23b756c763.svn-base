//
//  vnUILocatorRect.h
//  vngfx
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILocatorEx_h
#define vngfx_vnUILocatorEx_h

#include "vnUILocator.h"

_vn_begin

class _vn_gfx_api UILocatorRect : public UILocator {
	VN_DECL_UI_LOCATOR()
public:
	// ctor & dtor
	UILocatorRect(UIElement *owner);
    UILocatorRect(const UILocatorRect &other, UIElement *owner);
	virtual UILocatorRect * clone(UIElement *owner) const;
	
	// properties
	void setPosition(const vector2f &position);
	const vector2f & getPosition() const;
	void setSize(const vector2f &size);
	const vector2f & getSize() const;
	
	// methods
    virtual void init(const TreeDataObject *object);
	virtual void _set(const aabox2f &location);
	
protected:
	virtual void _updateBoundingBox();
	vector2f m_position;
	vector2f m_size;
};

class _vn_gfx_api UILocatorRectEx : public UILocatorRect {
	VN_DECL_UI_LOCATOR()
public:
	enum Alignment {
		kLittle = 0,
		kBig = 1,
		kHalf = 2,
	};
	enum Unit {
		kValue = 0,
		kPercent = 1,
	};
	
	// ctor & dtor
	UILocatorRectEx(UIElement *owner);
	UILocatorRectEx(const UILocatorRectEx &other, UIElement *owner);
	virtual UILocatorRectEx * clone(UIElement *owner) const;
	
	// properties
	Alignment getPositionXAlignment() const;
	void setPositionXAlignment(Alignment xAlignment);
	Alignment getPositionYAlignment() const;
	void setPositionYAlignment(Alignment yAlignment);
	
	Unit getPositionXUnit() const;
	void setPositionXUnit(Unit xUnit);
	Unit getPositionYUnit() const;
	void setPositionYUnit(Unit yUnit);
    
	Unit getWidthUnit() const;
	void setWidthUnit(Unit widthUnit);
	Unit getHeightUnit() const;
	void setHeightUnit(Unit heightUnit);
	
    
    virtual void init(const TreeDataObject *object);
	virtual void _set(const aabox2f &location);
	
protected:
	virtual void _updateBoundingBox();
	
	Alignment m_alignX, m_alignY;
	Unit m_unitX, m_unitY, m_unitWidth, m_unitHeight;
	
};

_vn_end

#endif
