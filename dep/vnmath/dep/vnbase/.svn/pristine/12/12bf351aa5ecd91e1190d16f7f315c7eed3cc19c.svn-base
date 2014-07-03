//
//  vnEditType.h
//  vnbase
//
//  Created by Wayne on 14-1-3.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnbase_vnEditType_h
#define vnbase_vnEditType_h

#include "vnEditValue.h"

#include <map>

_vn_begin


class _vn_base_api EditType : public RefCounted {
public:
    EditType();
    virtual const c8 * name() const = 0;
    virtual EditValue * generate() const = 0;
    virtual bool verity(const EditValue *value) const;
private:
    EditType(const EditType &other);
};

typedef RefCountedPtr<const EditType> EditTypePtr;

class _vn_base_api EditTypeBool : public EditType {
public:
    virtual const c8 * name() const;
    virtual EditValueBool * generate() const;
    static const c8 * _name(); // "bool"
};

class _vn_base_api EditTypeInt : public EditType {
public:
    virtual const c8 * name() const;
    virtual EditValueInt * generate() const;
    static const c8 * _name(); // "int"
};

class _vn_base_api EditTypeRangedInt : public EditTypeInt {
public:
    EditTypeRangedInt(s32 _min = 0x80000000, s32 _max = 0x7FFFFFFF);
    virtual const c8 * name() const;
    virtual EditValueInt * generate() const;
    virtual bool verity(const EditValue *value) const;
    
    s32 minValue() const;
    s32 maxValue() const;
    static const c8 * _name(); // "ranged_int"
protected:
    s32 m_min, m_max;
};

class _vn_base_api EditTypeEnumeration : public EditTypeInt {
public:
    EditTypeEnumeration(const str8 &name);
    
    virtual const c8 * name() const;
    virtual EditValueInt * generate() const;
    virtual bool verity(const EditValue *value) const;
    
    void set(const str8 &name, s32 value);
    const s32 * query(const str8 &name) const;
    const str8 * query(s32 value) const;
    
    typedef std::map<str8, s32> Items;
    
    const Items & items() const;
protected:
    Items m_items;
    str8 m_name;
};

class _vn_base_api EditTypeFloat : public EditType {
public:
    virtual const c8 * name() const;
    virtual EditValueFloat * generate() const;
    static const c8 * _name(); // "float"
};

class _vn_base_api EditTypeRangedFloat : public EditTypeFloat {
public:
    EditTypeRangedFloat(f32 _min = 0, f32 _max = 1.f);
    virtual const c8 * name() const;
    virtual EditValueFloat * generate() const;
    virtual bool verity(const EditValue *value) const;
    
    f32 minValue() const;
    f32 maxValue() const;
    static const c8 * _name(); // "ranged_float"
protected:
    f32 m_min, m_max;
};

class _vn_base_api EditTypeString : public EditType {
public:
    virtual const c8 * name() const;
    virtual EditValueString * generate() const;
    static const c8 * _name(); // "string"
};

class _vn_base_api EditTypeArray : public EditType {
public:
    EditTypeArray(const EditType *itemType, bool grab = true);
    virtual ~EditTypeArray();
    virtual const c8 * name() const;
    virtual EditValueArray * generate() const;
    EditValue * generateItem() const;
    
    const EditType * itemType() const;
    static const c8 * _name(); // "array"
protected:
    const EditType *m_itemType;
};

class _vn_base_api EditTypeObject : public EditType {
public:
    EditTypeObject(const str8 &_name);
    virtual ~EditTypeObject();
    virtual const c8 * name() const;
    virtual EditValueObject * generate() const;
    
    size_t numAttributes() const;
    bool isAttributeReadOnly(size_t index) const;
    const EditType * getAttributeType(size_t index) const;
    const str8 & getAttributeName(size_t index) const;
    size_t queryAttributeIndex(const str8 &name) const;
    const EditType * queryAttributeType(const str8 &name) const;
    
    size_t addAttribute(const str8 &name, bool readOnly, const EditType *_type, bool grab = true);
protected:
    str8 m_name;
    struct AttributeInfo {
        str8 name;
        bool readOnly;
        const EditType *type;
    };
    typedef std::vector<AttributeInfo> Attributes;
    typedef std::map<str8, size_t> AttributeMap;
    Attributes m_attributes;
    AttributeMap m_indices;
};

_vn_end

#endif
