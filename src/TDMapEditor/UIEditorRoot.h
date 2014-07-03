//
//  UIEditorRoot.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__UIEditorRoot__
#define __TDMapEditor__UIEditorRoot__

#include "vnUIElement.h"

class UIEditorRoot : public vn::UIElement {
public:
    virtual void onMouseLeftDown(const vn::vector2f &position);
    virtual void onMouseLeftUp(const vn::vector2f &position);
    virtual void onMouseRightDown(const vn::vector2f &position);
    virtual void onMouseRightUp(const vn::vector2f &position);
    virtual void onMouseCaptureCancelled();
    
    virtual void onKeyDown(vn::c32 chr, bool repeat);
    virtual void onKeyUp(vn::c32 chr);
    
    virtual void onFocusIn();
    virtual void onFocusOut();
};

#endif /* defined(__TDMapEditor__UIEditorRoot__) */
