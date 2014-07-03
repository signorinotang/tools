//
//  vnUILogic.h
//  vngfx
//
//  Created by Wayne on 13-12-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUILogic_h
#define vngfx_vnUILogic_h

#include "vnUIFactory.h"

_vn_begin

class _vn_gfx_api UILogic : public RefCounted {
public:
    UILogic();
    UILogic(const UILogic &other);
    virtual UILogic * clone() const;
    virtual ~UILogic();
    
    void init(UIElement *root, bool grab = true);
    virtual void init(const TreeDataObject *object);
    
    UIElement * root() const;
    
    bool startup();
    
    virtual void update(f32 deltaTime);
    
protected:
    UIElement *m_root;
    
    virtual bool _startup(UIFactory::ElementMap &namedElements);
};

typedef RefCountedPtr<UILogic> UILogicPtr;

_vn_end

#endif
