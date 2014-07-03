//
//  vnEditTypeFactory.cpp
//  vnbase
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnEditTypeFactory.h"

_vn_begin

EditTypeFactory::EditTypeFactory()
: m_etBool(vnnew EditTypeBool())
, m_etInt(vnnew EditTypeInt())
, m_etFloat(vnnew EditTypeFloat())
, m_etString(vnnew EditTypeString()) {
    
}

const EditTypeBool * EditTypeFactory::etBool() const {
    return m_etBool.ptr();
}

const EditTypeInt * EditTypeFactory::etInt() const {
    return m_etInt.ptr();
}

const EditTypeFloat * EditTypeFactory::etFloat() const {
    return m_etFloat.ptr();
}

const EditTypeString * EditTypeFactory::etString() const {
    return m_etString.ptr();
}

const EditTypeEnumeration * EditTypeFactory::etEnumeration(const str8 &name) {
    TypeMap::iterator it = m_namedTypes.find(name);
    if (it == m_namedTypes.end()) {
        return 0;
    }
    return dynamic_cast<const EditTypeEnumeration  *>(it->second.ptr());
}

const EditTypeObject * EditTypeFactory::etObject(const str8 &name) {
    TypeMap::iterator it = m_namedTypes.find(name);
    if (it == m_namedTypes.end()) {
        return 0;
    }
    return dynamic_cast<const EditTypeObject  *>(it->second.ptr());
}

const EditTypeArray * EditTypeFactory::etArray(const str8 &name) {
    TypeMap::iterator it = m_namedTypes.find(name);
    if (it == m_namedTypes.end()) {
        return 0;
    }
    return dynamic_cast<const EditTypeArray  *>(it->second.ptr());
}

const EditType * EditTypeFactory::findType(const str8 &name) {
    TypeMap::iterator it = m_namedTypes.find(name);
    if (it == m_namedTypes.end()) {
        return 0;
    }
    return it->second.ptr();
}

void EditTypeFactory::_register(const str8 &name, const EditType *type, bool grab) {
    vnassert(type);
    if (grab) {
        type->grab();
    }
    m_namedTypes[name] = type;
}


_vn_end
