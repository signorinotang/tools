//
//  UIListItemDecorationImage.h
//  TDMapEditor
//
//  Created by Wayne on 3/31/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__UIListItemDecorationImage__
#define __TDMapEditor__UIListItemDecorationImage__

#include "vnUIListItem.h"

class UIListItemDecorationImage : public vn::UIListItemText {
    VN_DECL_UI_LOGIC()
public:
    UIListItemDecorationImage();
    UIListItemDecorationImage(const UIListItemDecorationImage &other);
    virtual UIListItemDecorationImage * clone() const;
    
    
    virtual void set(const vn::TreeDataObject *object);
    
    const vn::str8 & getImagePath() const;
    
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
    
    vn::UIPicture *m_image;
    vn::str8 m_imagePath;
};

#endif /* defined(__TDMapEditor__UIListItemDecorationImage__) */
