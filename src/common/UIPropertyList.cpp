//
//  UIPropertyList.cpp
//  test
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "UIPropertyList.h"
#include "vnUIList.h"

VN_IMPL_UI_LOGIC(UIPropertyList, "PropertyList")

UIPropertyList::UIPropertyList()
: m_liBool(0)
, m_liInt(0)
, m_liFloat(0)
, m_liString(0)
, m_liEnum(0) {
    
}

UIPropertyList::~UIPropertyList() {
    VN_SAFE_DROP(m_liBool);
    VN_SAFE_DROP(m_liInt);
    VN_SAFE_DROP(m_liFloat);
    VN_SAFE_DROP(m_liString);
    VN_SAFE_DROP(m_liEnum);
}

void UIPropertyList::init(const vn::TreeDataObject *object) {
    UILogic::init(object);
    const vn::TreeDataObject *tdobj = object->queryAttributeObject("li_bool");
    if (tdobj) {
        vn::UILogic *p = vn::UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_liBool = dynamic_cast<UIListItemETBool *>(p);
            if (!m_liBool) {
                p->drop();
            }
        }
    }
    
    tdobj = object->queryAttributeObject("li_int");
    if (tdobj) {
        vn::UILogic *p = vn::UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_liInt = dynamic_cast<UIListItemETInt *>(p);
            if (!m_liInt) {
                p->drop();
            }
        }
    }
    
    tdobj = object->queryAttributeObject("li_float");
    if (tdobj) {
        vn::UILogic *p = vn::UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_liFloat = dynamic_cast<UIListItemETFloat *>(p);
            if (!m_liFloat) {
                p->drop();
            }
        }
    }
    
    tdobj = object->queryAttributeObject("li_string");
    if (tdobj) {
        vn::UILogic *p = vn::UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_liString = dynamic_cast<UIListItemETString *>(p);
            if (!m_liString) {
                p->drop();
            }
        }
    }
    
    tdobj = object->queryAttributeObject("li_enum");
    if (tdobj) {
        vn::UILogic *p = vn::UIFactory::instance().createLogic(tdobj);
        if (p) {
            m_liEnum = dynamic_cast<UIListItemETEnum *>(p);
            if (!m_liEnum) {
                p->drop();
            }
        }
    }
}

void UIPropertyList::setTarget(vn::EditValueObject *target) {
    m_list->clearItems();
    if (!target) {
        return ;
    }
    const vn::EditTypeObject *type = target->type();
    for (size_t i = 0, c = type->numAttributes(); i < c; ++i) {
        _buildItem(type->getAttributeName(i), target->attribute(i), !type->isAttributeReadOnly(i));
    }
}


bool UIPropertyList::_startup(vn::UIFactory::ElementMap &namedElements) {
    m_list = dynamic_cast<vn::UIList *>(namedElements["list"]);
    if (!m_list) {
        return false;
    }
    return true;
}

void UIPropertyList::_buildItem(const vn::str8 &name, vn::EditValue *value, bool editable) {
    const vn::EditTypeBool *t_bool = dynamic_cast<const vn::EditTypeBool *>(value->type());
    if (t_bool && m_liBool) {
        UIListItemETBool *item = m_liBool->clone();
        item->startup();
        m_list->addItem(item, false);
        item->setText(name);
        item->setEditable(editable);
        item->setTarget((vn::EditValueBool *)value);
        return ;
    }
    
    const vn::EditTypeEnumeration *t_enum = dynamic_cast<const vn::EditTypeEnumeration *>(value->type());
    if (t_enum && m_liEnum) {
        UIListItemETEnum *item = m_liEnum->clone();
        item->startup();
        m_list->addItem(item, false);
        item->setText(name);
        item->setEditable(editable);
        item->setTarget((vn::EditValueInt *)value);
        return ;
    }
    
    const vn::EditTypeInt *t_int = dynamic_cast<const vn::EditTypeInt *>(value->type());
    if (t_int && m_liInt) {
        UIListItemETInt *item = m_liInt->clone();
        item->startup();
        m_list->addItem(item, false);
        item->setText(name);
        item->setEditable(editable);
        item->setTarget((vn::EditValueInt *)value);
        return ;
    }
    
    const vn::EditTypeFloat *t_float = dynamic_cast<const vn::EditTypeFloat *>(value->type());
    if (t_float && m_liFloat) {
        UIListItemETFloat *item = m_liFloat->clone();
        item->startup();
        m_list->addItem(item, false);
        item->setText(name);
        item->setEditable(editable);
        item->setTarget((vn::EditValueFloat *)value);
        return ;
    }
    
    const vn::EditTypeString *t_string = dynamic_cast<const vn::EditTypeString *>(value->type());
    if (t_string && m_liString) {
        UIListItemETString *item = m_liString->clone();
        item->startup();
        m_list->addItem(item, false);
        item->setText(name);
        item->setEditable(editable);
        item->setTarget((vn::EditValueString *)value);
        return ;
    }
}