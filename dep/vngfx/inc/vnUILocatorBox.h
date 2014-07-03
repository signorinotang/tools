//
//  vnUILocatorBox.h
//  vngfx
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILocatorBox_h
#define vngfx_vnUILocatorBox_h

#include "vnUILocator.h"

_vn_begin

class _vn_gfx_api UILocatorBox : public UILocator {
	VN_DECL_UI_LOCATOR()
public:
	// ctor & dtor
	UILocatorBox(UIElement *owner);
    UILocatorBox(const UILocatorBox &other, UIElement *owner);
	virtual UILocatorBox * clone(UIElement *owner) const;
	
	// methods
	virtual void init(const TreeDataObject *object);
	virtual void _set(const aabox2f &location);
	
	void setBox(const aabox2f &box);
	const aabox2f & getBox() const;
	
protected:
	virtual void _updateBoundingBox();
	
	aabox2f m_box;
};

class _vn_gfx_api UILocatorBoxEx : public UILocatorBox {
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
	UILocatorBoxEx(UIElement *owner);
    UILocatorBoxEx(const UILocatorBoxEx &other, UIElement *owner);
	virtual UILocatorBoxEx * clone(UIElement *owner) const;
	
	// methods
	virtual void init(const TreeDataObject *object);
	virtual void _set(const aabox2f &location);
	
	Alignment getLeftAlignement() const;
	Alignment getTopAlignement() const;
	Alignment getRightAlignement() const;
	Alignment getBottomAlignement() const;
	
	void setLeftAlignment(Alignment alignment);
	void setTopAlignment(Alignment alignment);
	void setRightAlignment(Alignment alignment);
	void setBottomAlignment(Alignment alignment);
	
	Unit getLeftUnit() const;
	Unit getTopUnit() const;
	Unit getRightUnit() const;
	Unit getBottomUnit() const;
	
	void setLeftUnit(Unit unit);
	void setTopUnit(Unit unit);
	void setRightUnit(Unit unit);
	void setBottomUnit(Unit unit);
	
	
protected:
	virtual void _updateBoundingBox();
	
	Unit m_unitLeft, m_unitTop, m_unitRight, m_unitBottom;
	Alignment m_alignmentLeft, m_alignmentTop, m_alignmentRight, m_alignmentBottom;
	
	static f32 _convertTo(f32 x, Alignment align, Unit unit, f32 little, f32 big);
	static f32 _convertFrom(f32 x, Alignment align, Unit unit, f32 little, f32 big);
};

_vn_end

#endif
