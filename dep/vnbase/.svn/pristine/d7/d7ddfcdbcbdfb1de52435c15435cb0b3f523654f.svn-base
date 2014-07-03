//
//  vnEditTypeFactory.h
//  vnbase
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnbase_vnEditTypeFactory_h
#define vnbase_vnEditTypeFactory_h

#include "vnEditType.h"
#include "vnSingleton.h"

_vn_begin

class _vn_base_api EditTypeFactory : public Singleton<EditTypeFactory> {
public:
    EditTypeFactory();
    
    const EditTypeBool * etBool() const;
    const EditTypeInt * etInt() const;
    const EditTypeFloat * etFloat() const;
    const EditTypeString * etString() const;
    
    const EditTypeEnumeration * etEnumeration(const str8 &name);
    const EditTypeObject * etObject(const str8 &name);
    const EditTypeArray * etArray(const str8 &name);
    
    const EditType * findType(const str8 &name);
    
    void _register(const str8 &name, const EditType *type, bool grab = true);
    
private:
    
    RefCountedPtr<const EditTypeBool> m_etBool;
    RefCountedPtr<const EditTypeInt> m_etInt;
    RefCountedPtr<const EditTypeFloat> m_etFloat;
    RefCountedPtr<const EditTypeString> m_etString;
    
    typedef std::map<str8, EditTypePtr> TypeMap;
    TypeMap m_namedTypes;
};

_vn_end

#define VN_DEFINE_ET_ENUMERATION_BEGIN(_id, _name) \
    struct _auto_reg_et_enum_##_id {\
        _auto_reg_et_enum_##_id() {\
            _vn_ns::EditTypeEnumeration *et = vnnew _vn_ns::EditTypeEnumeration(_name);

#define VN_DEFINE_ET_ENUMERATION_ITEM(_name, _value) \
            et->set(_name, _value);

#define VN_DEFINE_ET_ENUMERATION_END(_id) \
            _vn_ns::EditTypeFactory::instance()._register(et->name(), et, false);\
        };\
    } _auto_reg_et_enum_##_id##_ins;


#define VN_DEFINE_ET_OBJECT_BEGIN(_id, _name) \
    struct _auto_reg_et_obj_##_id {\
        _auto_reg_et_obj_##_id() {\
            _vn_ns::EditTypeObject *et = vnnew _vn_ns::EditTypeObject(_name);

#define VN_DEFINE_ET_OBJECT_ATTRIB_BOOL(_name, _readonly) \
            et->addAttribute(_name, _readonly, _vn_ns::EditTypeFactory::instance().etBool(), true);

#define VN_DEFINE_ET_OBJECT_ATTRIB_INT(_name, _readonly) \
            et->addAttribute(_name, _readonly, _vn_ns::EditTypeFactory::instance().etInt(), true);

#define VN_DEFINE_ET_OBJECT_ATTRIB_FLOAT(_name, _readonly) \
            et->addAttribute(_name, _readonly, _vn_ns::EditTypeFactory::instance().etFloat(), true);

#define VN_DEFINE_ET_OBJECT_ATTRIB_STRING(_name, _readonly) \
            et->addAttribute(_name, _readonly, _vn_ns::EditTypeFactory::instance().etString(), true);

#define VN_DEFINE_ET_OBJECT_ATTRIB_NAMED_TYPE(_name, _readonly, _type_name) \
            et->addAttribute(_name, _readonly, _vn_ns::EditTypeFactory::instance().findType(_type_name), true);

#define VN_DEFINE_ET_OBJECT_ATTRIB(_name, _readonly, _type, _grab) \
            et->addAttribute(_name, _readonly, _type, _grab);

#define VN_DEFINE_ET_OBJECT_END(_id) \
            _vn_ns::EditTypeFactory::instance()._register(et->name(), et, false);\
        };\
    } _auto_reg_et_obj_##_id##_ins;

#endif
