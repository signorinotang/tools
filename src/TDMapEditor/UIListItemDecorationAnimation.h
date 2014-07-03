//
//  UIListItemDecorationAnimation.h
//  TDMapEditor
//
//  Created by Wayne on 5/21/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__UIListItemDecorationAnimation__
#define __TDMapEditor__UIListItemDecorationAnimation__

#include "vnUIListItem.h"

class UIListItemDecorationAnimation : public vn::UIListItemText {
    VN_DECL_UI_LOGIC()
public:
    //UIListItemDecorationAnimation();
    //UIListItemDecorationAnimation(const UIListItemDecorationAnimation &other);
    virtual UIListItemDecorationAnimation * clone() const;
    
    
    virtual void set(const vn::TreeDataObject *object);
    
    const vn::str8 & getAnimationPath() const;
    
protected:
    
    vn::str8 m_animationPath;

};

#endif /* defined(__TDMapEditor__UIListItemDecorationAnimation__) */
