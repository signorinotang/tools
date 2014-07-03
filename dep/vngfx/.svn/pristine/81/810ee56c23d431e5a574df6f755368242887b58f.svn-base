//
//  vnUIListItem.h
//  vngfx
//
//  Created by Wayne on 13-12-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIListItem_h
#define vngfx_vnUIListItem_h

#include "vnUIElement.h"
#include "vnUIRenderText.h"
#include "vnUILogic.h"

_vn_begin

class UIList;

class _vn_gfx_api UIListItem : public UILogic {
public:
    UIListItem();
    UIListItem(const UIListItem &other);
    virtual UIListItem * clone() const = 0;
    virtual ~UIListItem();
    
    void setList(UIList *list);
    UIList * getList() const;

    virtual f32 calculateWidth() = 0;
    virtual void setPositionAndHeight(const vector2f &pos, f32 height) = 0;
    virtual f32 calculateHeight() = 0;
    virtual void setPositionAndWidth(const vector2f &pos, f32 width) = 0;
    
    virtual void set(const TreeDataObject *object);
    
    virtual void setText(const str8 &text);
    virtual void setText(const str32 &text);
    virtual void getText(str8 &text);
    
    virtual void onSelected();
    virtual void onUnselected();
    
    void setUserData(RefCounted *object, bool grab);
    RefCounted * getUserData() const;
    
protected:
    
    RefCounted * m_userData;
    UIList *m_list;
};

class UILabel;
class UIPicture;

class _vn_gfx_api UIListItemText : public UIListItem {
    VN_DECL_UI_LOGIC()
public:
    UIListItemText();
    UIListItemText(const UIListItemText &other);
    virtual UIListItemText * clone() const;
    
    virtual void init(const TreeDataObject *object);
    
    virtual void set(const TreeDataObject *object);
    
    virtual f32 calculateWidth();
    virtual void setPositionAndHeight(const vector2f &pos, f32 height);
    virtual f32 calculateHeight();
    virtual void setPositionAndWidth(const vector2f &pos, f32 width);
    
    virtual void setText(const str8 &text);
    virtual void setText(const str32 &text);
    virtual void getText(str8 &text);
    
    void setTextNormalColor(const color_t &color);
    void setTextSelectedColor(const color_t &color);
    
    virtual void onSelected();
    virtual void onUnselected();
    
protected:
    virtual bool _startup(UIFactory::ElementMap &namedElements);
    UILabel *m_text; // named "text"
    UIPicture * m_bkg_selected; // named "bkg_selected"
    vector2f m_size;
    color_t m_textNormalColor;
    color_t m_textSelectedColor;
    
};


_vn_end

#endif
