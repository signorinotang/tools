//
//  vnEditType.cpp
//  vnbase
//
//  Created by Wayne on 14-1-3.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnEditType.h"
#include "vnEditValue.h"

_vn_begin

EditType::EditType() {
	
}

bool EditType::verity(const vn::EditValue *value) const {
    vnassert(value->m_type == this);
    return true;
}

//////////////////////////////////////////////////////////

const c8 * EditTypeBool::_name() {
    return "bool";
}

const c8 * EditTypeBool::name() const {
    return _name();
}

EditValueBool * EditTypeBool::generate() const {
    return vnnew EditValueBool(this);
}

//////////////////////////////////////////////////////////

const c8 * EditTypeInt::_name() {
    return "int";
}

const c8 * EditTypeInt::name() const {
    return _name();
}

EditValueInt * EditTypeInt::generate() const {
    return vnnew EditValueInt(this);
}

//////////////////////////////////////////////////////////

EditTypeRangedInt::EditTypeRangedInt(s32 _min, s32 _max)
: m_min(_min)
, m_max(_max) {
    vnassert(_min <= _max);
}

const c8 * EditTypeRangedInt::_name() {
    return "ranged_int";
}

const c8 * EditTypeRangedInt::name() const {
    return _name();
}

EditValueInt * EditTypeRangedInt::generate() const {
    return vnnew EditValueInt(this);
}

bool EditTypeRangedInt::verity(const EditValue *value) const {
    vnassert(value->type() == this);
    const EditValueInt *_val = static_cast<const EditValueInt *>(value);
    return _val->m_value >= m_min && _val->m_value <= m_max;
}

s32 EditTypeRangedInt::minValue() const {
    return m_min;
}

s32 EditTypeRangedInt::maxValue() const {
    return m_max;
}

//////////////////////////////////////////////////////////
EditTypeEnumeration::EditTypeEnumeration(const str8 &name)
: m_name(name) {
    
}

const c8 * EditTypeEnumeration::name() const {
    return m_name.c_str();
}
EditValueInt * EditTypeEnumeration::generate() const {
    return vnnew EditValueInt(this);
}

bool EditTypeEnumeration::verity(const EditValue *value) const {
    vnassert(value->type() == this);
    const EditValueInt *_val = static_cast<const EditValueInt *>(value);
    for (Items::const_iterator it = m_items.begin(); it != m_items.end(); ++it) {
        if (it->second == _val->m_value) {
            return true;
        }
    }
    return false;
}

void EditTypeEnumeration::set(const str8 &name, s32 value) {
    m_items[name] = value;
}

const s32 * EditTypeEnumeration::query(const str8 &name) const {
    Items::const_iterator it = m_items.find(name);
    if (it == m_items.end()) {
        return 0;
    }
    return &it->second;
}

const str8 * EditTypeEnumeration::query(s32 value) const {
    for (Items::const_iterator it = m_items.begin(); it != m_items.end(); ++it) {
        if (it->second == value) {
            return &it->first;
        }
    }
    return 0;
}

const EditTypeEnumeration::Items & EditTypeEnumeration::items() const {
    return m_items;
}

//////////////////////////////////////////////////////////

const c8 * EditTypeFloat::_name() {
    return "float";
}

const c8 * EditTypeFloat::name() const {
    return _name();
}

EditValueFloat * EditTypeFloat::generate() const {
    return vnnew EditValueFloat(this);
}

//////////////////////////////////////////////////////////

EditTypeRangedFloat::EditTypeRangedFloat(f32 _min, f32 _max)
: m_min(_min)
, m_max(_max) {
    vnassert(_min <= _max);
}

const c8 * EditTypeRangedFloat::_name() {
    return "ranged_float";
}

const c8 * EditTypeRangedFloat::name() const {
    return _name();
}

EditValueFloat * EditTypeRangedFloat::generate() const {
    return vnnew EditValueFloat(this);
}

bool EditTypeRangedFloat::verity(const EditValue *value) const {
    vnassert(value->type() == this);
    const EditValueFloat *_val = static_cast<const EditValueFloat *>(value);
    return _val->m_value >= m_min && _val->m_value <= m_max;
}

f32 EditTypeRangedFloat::minValue() const {
    return m_min;
}

f32 EditTypeRangedFloat::maxValue() const {
    return m_max;
}

//////////////////////////////////////////////////////////

const c8 * EditTypeString::_name() {
    return "string";
}

const c8 * EditTypeString::name() const {
    return _name();
}

EditValueString * EditTypeString::generate() const {
    return vnnew EditValueString(this);
}

//////////////////////////////////////////////////////////

EditTypeArray::EditTypeArray(const EditType *itemType, bool grab)
: m_itemType(itemType) {
    vnassert(itemType);
    if (grab) {
        m_itemType->grab();
    }
}

EditTypeArray::~EditTypeArray() {
    m_itemType->drop();
}

const c8 * EditTypeArray::name() const {
    return _name();
}

EditValueArray * EditTypeArray::generate() const {
    return vnnew EditValueArray(this);
}

EditValue * EditTypeArray::generateItem() const {
    return m_itemType->generate();
}

const EditType * EditTypeArray::itemType() const {
    return m_itemType;
}

const c8 * EditTypeArray::_name() {
    return "array";
}

//////////////////////////////////////////////////////////

EditTypeObject::EditTypeObject(const str8 &_name)
: m_name(_name) {
    
}

EditTypeObject::~EditTypeObject() {
    for (Attributes::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
        it->type->drop();
    }
}

const c8 * EditTypeObject::name() const {
    return m_name.c_str();
}

EditValueObject * EditTypeObject::generate() const {
    return vnnew EditValueObject(this);
}

size_t EditTypeObject::numAttributes() const {
    return m_attributes.size();
}

bool EditTypeObject::isAttributeReadOnly(size_t index) const {
    return m_attributes[index].readOnly;
}

const str8 & EditTypeObject::getAttributeName(size_t index) const {
    return m_attributes[index].name;
}

const EditType * EditTypeObject::getAttributeType(size_t index) const {
    return m_attributes[index].type;
}

size_t EditTypeObject::queryAttributeIndex(const str8 &name) const {
    AttributeMap::const_iterator it = m_indices.find(name);
    if (it == m_indices.end()) {
        return -1;
    }
    return it->second;
}

const EditType * EditTypeObject::queryAttributeType(const str8 &name) const {
    AttributeMap::const_iterator it = m_indices.find(name);
    if (it == m_indices.end()) {
        return 0;
    }
    return m_attributes[it->second].type;
}

size_t EditTypeObject::addAttribute(const str8 &name, bool readOnly, const EditType *_type, bool grab) {
    vnassert(_type);
    if (grab) {
        _type->grab();
    }
    AttributeMap::iterator it = m_indices.find(name);
    if (it != m_indices.end()) {
        AttributeInfo &info = m_attributes[it->second];
        info.readOnly = readOnly;
        info.type->drop();
        info.type = _type;
        return it->second;
    }
    size_t ret = m_indices[name] = m_attributes.size();
    AttributeInfo info;
    info.name = name;
    info.readOnly = readOnly;
    info.type = _type;
    m_attributes.push_back(info);
    return ret;
}

_vn_end
