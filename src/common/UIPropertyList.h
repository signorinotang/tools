//
//  UIPropertyList.h
//  test
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __test__UIPropertyList__
#define __test__UIPropertyList__

#include "UIListItemETBool.h"
#include "UIListItemETInt.h"
#include "UIListItemETFloat.h"
#include "UIListItemETString.h"
#include "UIListItemETEnum.h"

class UIPropertyList : public vn::UILogic {
    VN_DECL_UI_LOGIC()
public:
    UIPropertyList();
    virtual ~UIPropertyList();
    virtual void init(const vn::TreeDataObject *object);
    
    void setTarget(vn::EditValueObject *target);
    
protected:
    virtual bool _startup(vn::UIFactory::ElementMap &namedElements);
    
    UIListItemETBool *m_liBool;
    UIListItemETInt *m_liInt;
    UIListItemETFloat *m_liFloat;
    UIListItemETString *m_liString;
    UIListItemETEnum *m_liEnum;
    
    vn::UIList *m_list;
    
    void _buildItem(const vn::str8 &name, vn::EditValue *value, bool editable);
};

#endif /* defined(__test__UIPropertyList__) */
