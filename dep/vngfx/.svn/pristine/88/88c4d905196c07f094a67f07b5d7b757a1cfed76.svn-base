//
//  vnUILocator.h
//  vngfx
//
//  Created by Wayne on 13-12-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILocator_h
#define vngfx_vnUILocator_h

#include "vnUIFactory.h"
#include "vnAllocatedObject.h"
#include "vnAABox2.h"

_vn_begin

class UIElement;

class _vn_gfx_api UILocator : public AllocatedObject {
public:
	// ctor & dtor
	UILocator(UIElement *owner);
    UILocator(const UILocator &other, UIElement *owner);
	virtual UILocator * clone(UIElement *owner) const = 0;
	virtual ~UILocator();
	
	// methods
	void markDirty();
	void cleanDirty();
	bool isDirty() const;
	void update(f32 deltaTime);
	
    virtual void init(const TreeDataObject *object);
    
	virtual void _set(const aabox2f &location) = 0;
	
protected:
	virtual void _onUpdate(f32 deltaTime);
	virtual void _updateBoundingBox() = 0;
	void _setOwnerBoundingBox(const aabox2f &boundingBox);
	UIElement *m_owner;
	bool m_dirty;
};

_vn_end

#endif
