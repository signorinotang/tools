//
//  BrushSSO.h
//  TDMapEditor
//
//  Created by Wayne on 6/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BrushSSO__
#define __TDMapEditor__BrushSSO__

#include "Brush.h"
#include "vnUIWindow.h"
#include "vnUIButton.h"
#include "vnUIList.h"

#include <map>

class Brush_SSO : public Brush {
public:
    
    Brush_SSO();
    ~Brush_SSO();
    
    virtual void init();
    virtual void fini();
    virtual void begin(const vn::vector2f &pos);
    virtual void end(const vn::vector2f &pos);
    virtual void update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint);
    virtual bool busy() const;
    
    virtual void switchIn();
    virtual void switchOut();
    
    virtual void onKeyDown(vn::c32 chr, bool repeat);
    virtual void onMouseRightDown(const vn::vector2f &pos);
    virtual void onMouseRightUp(const vn::vector2f &pos);
    virtual void onMapSSORemoved(const MapSSOPtr &sso);
    
protected:
    vn::RefCountedPtr<vn::UIWindow> m_uiWindow;
    vn::UIButton *m_btnDelete = 0;
    vn::UIList *m_lstSSO = 0;
    
    void _onDelete(vn::UIElement *);
    void _onItemClicked(vn::UIElement *, vn::UIListItem *item);
    
    MapSSOPtr m_sso;
    SceneNodePtr m_node = vnnew SceneNode();
    
    vn::vector2f m_lastPos, m_originPos;
    bool m_moved = false;
    bool m_moveSSO = false;
    
    enum State {
        kEdit = 0,
        kNew,
    } m_state = kEdit;
    
    void _cancelNew();
    void _cancelEdit();
};

#endif /* defined(__TDMapEditor__BrushSSO__) */
