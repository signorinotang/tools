//
//  vnVariable.cpp
//  vnbase
//
//  Created by Wayne on 13-10-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnVariable.h"

_vn_begin

bool Variable::numberic() const {
    switch (type()) {
        case kVT_null:
        case kVT_bool:
        case kVT_int32:
        case kVT_int64:
        case kVT_float:
        case kVT_double:
            return true;
        case kVT_reference:
            return static_cast<const Variable_reference *>(this)->get()->numberic();
        default:
            break;
    }
    return false;
}

void Variable::save(const Variable *value, DataStream &stream) {
	vnassert(value);
	stream << (u8)value->type();
	value->_save(stream);
}

void Variable::load(DataStream &stream, Variable *&value) {
	try {
		u8 t = 0;
		stream >> t;
		switch (t) {
			case kVT_null:
				value = Variable_null::instance();
				value->grab();
				return ;
				
			case kVT_bool:
				value = vnnew Variable_bool();
				break;
				
			case kVT_int32:
				value = vnnew Variable_int32();
				break;
				
			case kVT_int64:
				value = vnnew Variable_int64();
				break;
			
			case kVT_float:
				value = vnnew Variable_float32();
				break;
		
			case kVT_double:
				value = vnnew Variable_float64();
				break;
				
			case kVT_string:
				value = vnnew Variable_string();
				break;
				
			case kVT_reference:
				value = vnnew Variable_reference();
				break;
				
			case kVT_array:
				value = vnnew Variable_array();
				break;
				
			case kVT_object:
				value = vnnew Variable_object();
				break;
				
			default:
				value = 0;
				throw LoadException();
		}
		value->_load(stream);
	} catch (DataStream::Exception &e) {
		VN_SAFE_DROP(value);
		throw e;
	}
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_null
#endif

Variable::Type Variable_null::type() const {
	return kVT_null;
}

bool Variable_null::boolean() const {
	return false;
}

s32 Variable_null::int32() const {
	return 0;
}

s64 Variable_null::int64() const {
	return 0;
}

f32 Variable_null::float32() const {
	return 0;
}

f64 Variable_null::float64() const {
	return 0;
}

str8 Variable_null::string() const {
	return str8();
}

str8 & Variable_null::string(str8 &ret) const {
	ret.clear();
	return ret;
}

size_t Variable_null::size() const {
	return 0;
}

Variable * Variable_null::at(size_t index) const {
	throw NullReferenceException();
	return 0;
}

Variable * Variable_null::at(const str8 &index) const {
	throw NullReferenceException();
	return 0;
}

Variable_null * Variable_null::instance() {
	static Variable_null s_ins;
	return &s_ins;
}

void Variable_null::_save(DataStream &stream) const {
	
}

void Variable_null::_load(DataStream &stream) {
	
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_bool
#endif

Variable_bool::Variable_bool()
: value(false) {
	
}

Variable_bool::Variable_bool(bool _v)
: value(_v) {
	
}

Variable::Type Variable_bool::type() const {
	return kVT_bool;
}

bool Variable_bool::boolean() const {
	return value;
}

s32 Variable_bool::int32() const {
	return (value ? 1 : 0);
}

s64 Variable_bool::int64() const {
	return (value ? 1 : 0);
}

f32 Variable_bool::float32() const {
	return (value ? 1.f : 0.f);
}

f64 Variable_bool::float64() const {
	return (value ? 1.0 : 0.0);
}

str8 Variable_bool::string() const {
	if (value) {
		return "true";
	}
	return "false";
}

str8 & Variable_bool::string(str8 &ret) const {
	ret = (value ? "true" : "false");
	return ret;
}

size_t Variable_bool::size() const {
	return 1;
}

Variable * Variable_bool::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_bool *>(this);
}

Variable * Variable_bool::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_bool::_save(DataStream &stream) const {
	stream << value;
}

void Variable_bool::_load(DataStream &stream) {
	stream >> value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_int32
#endif

Variable_int32::Variable_int32()
: value(0) {
	
}

Variable_int32::Variable_int32(s32 _v)
: value(_v) {
	
}

Variable::Type Variable_int32::type() const {
	return kVT_int32;
}

bool Variable_int32::boolean() const {
	return value != 0;
}

s32 Variable_int32::int32() const {
	return value;
}

s64 Variable_int32::int64() const {
	return value;
}

f32 Variable_int32::float32() const {
	return (f32)value;
}

f64 Variable_int32::float64() const {
	return value;
}

str8 Variable_int32::string() const {
	c8 buf[16];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%d", value);
#else
	sprintf(buf, "%d", value);
#endif
	return buf;
}

str8 & Variable_int32::string(str8 &ret) const {
	c8 buf[16];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%d", value);
#else
	sprintf(buf, "%d", value);
#endif
	return ret = buf;
}

size_t Variable_int32::size() const {
	return 1;
}

Variable * Variable_int32::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_int32 *>(this);
}

Variable * Variable_int32::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_int32::_save(DataStream &stream) const {
	stream << value;
}

void Variable_int32::_load(DataStream &stream) {
	stream >> value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_int64
#endif

Variable_int64::Variable_int64()
: value(0) {
	
}

Variable_int64::Variable_int64(s64 _v)
: value(_v) {
	
}

Variable::Type Variable_int64::type() const {
	return kVT_int64;
}

bool Variable_int64::boolean() const {
	return value != 0;
}

s32 Variable_int64::int32() const {
	return (s32)value;
}

s64 Variable_int64::int64() const {
	return value;
}

f32 Variable_int64::float32() const {
	return (f32)value;
}

f64 Variable_int64::float64() const {
	return (f64)value;
}

str8 Variable_int64::string() const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%lld", value);
#else
	sprintf(buf, "%lld", value);
#endif
	return buf;
}

str8 & Variable_int64::string(str8 &ret) const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%lld", value);
#else
	sprintf(buf, "%lld", value);
#endif
	return ret = buf;
}

size_t Variable_int64::size() const {
	return 1;
}

Variable * Variable_int64::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_int64 *>(this);
}

Variable * Variable_int64::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_int64::_save(DataStream &stream) const {
	stream << value;
}

void Variable_int64::_load(DataStream &stream) {
	stream >> value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_float32
#endif

Variable_float32::Variable_float32()
: value(0) {
	
}

Variable_float32::Variable_float32(f32 _v)
: value(_v) {
	
}

Variable::Type Variable_float32::type() const {
	return kVT_float;
}

bool Variable_float32::boolean() const {
	return value != 0;
}

s32 Variable_float32::int32() const {
	return (s32)value;
}

s64 Variable_float32::int64() const {
	return (s64)value;
}

f32 Variable_float32::float32() const {
	return value;
}

f64 Variable_float32::float64() const {
	return (f64)value;
}

str8 Variable_float32::string() const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%g", value);
#else
	sprintf(buf, "%g", value);
#endif
	return buf;
}

str8 & Variable_float32::string(str8 &ret) const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%g", value);
#else
	sprintf(buf, "%g", value);
#endif
	return ret = buf;
}

size_t Variable_float32::size() const {
	return 1;
}

Variable * Variable_float32::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_float32 *>(this);
}

Variable * Variable_float32::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_float32::_save(DataStream &stream) const {
	stream << value;
}

void Variable_float32::_load(DataStream &stream) {
	stream >> value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_float64
#endif

Variable_float64::Variable_float64()
: value(0) {
	
}

Variable_float64::Variable_float64(f64 _v)
: value(_v) {
	
}

Variable::Type Variable_float64::type() const {
	return kVT_double;
}

bool Variable_float64::boolean() const {
	return value != 0;
}

s32 Variable_float64::int32() const {
	return (s32)value;
}

s64 Variable_float64::int64() const {
	return (s64)value;
}

f32 Variable_float64::float32() const {
	return (f32)value;
}

f64 Variable_float64::float64() const {
	return (f64)value;
}

str8 Variable_float64::string() const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%g", value);
#else
	sprintf(buf, "%g", value);
#endif
	return buf;
}

str8 & Variable_float64::string(str8 &ret) const {
	c8 buf[32];
#if VN_COMPILER == VN_COMPILER_MSVC
	sprintf_s(buf, "%g", value);
#else
	sprintf(buf, "%g", value);
#endif
	return ret = buf;
}

size_t Variable_float64::size() const {
	return 1;
}

Variable * Variable_float64::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_float64 *>(this);
}

Variable * Variable_float64::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_float64::_save(DataStream &stream) const {
	stream << value;
}

void Variable_float64::_load(DataStream &stream) {
	stream >> value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_string
#endif

Variable_string::Variable_string() {
	
}

Variable_string::Variable_string(const str8 &_v)
: value(_v) {
	
}

Variable::Type Variable_string::type() const {
	return kVT_string;
}

bool Variable_string::boolean() const {
	return !value.empty();
}

s32 Variable_string::int32() const {
	throw InvalidTypeException();
	return 0;
}

s64 Variable_string::int64() const {
	throw InvalidTypeException();
	return 0;
}

f32 Variable_string::float32() const {
	throw InvalidTypeException();
	return 0;
}

f64 Variable_string::float64() const {
	throw InvalidTypeException();
	return 0;
}

str8 Variable_string::string() const {
	return value;
}

str8 & Variable_string::string(str8 &ret) const {
	return ret = value;
}

size_t Variable_string::size() const {
	return 1;
}

Variable * Variable_string::at(size_t index) const {
	if (index != 0) {
		throw InvalidIndexException();
	}
	return const_cast<Variable_string *>(this);
}

Variable * Variable_string::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

void Variable_string::_save(DataStream &stream) const {
	stream << value;
}

void Variable_string::_load(DataStream &stream) {
	stream >> value;
}

Variable_reference::Variable_reference() {
	Variable *v = Variable_null::instance();
	v->grab();
	m_value = v;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_reference
#endif

Variable_reference::Variable_reference(Variable *value, bool grab) {
	if (value) {
		m_value = value;
		if (grab) {
			value->grab();
		}
	} else {
		value = Variable_null::instance();
		value->grab();
		m_value = value;
	}
}

Variable_reference::Variable_reference(const str8 &name)
: m_name(name) {
	Variable *v = Variable_null::instance();
	v->grab();
	m_value = v;
}

Variable_reference::Variable_reference(const str8 &name, Variable *value, bool grab)
: m_name(name) {
	if (value) {
		m_value = value;
		if (grab) {
			value->grab();
		}
	} else {
		value = Variable_null::instance();
		value->grab();
		m_value = value;
	}
}

Variable::Type Variable_reference::type() const {
	return kVT_reference;
}

bool Variable_reference::boolean() const {
	return m_value->boolean();
}

s32 Variable_reference::int32() const {
	return m_value->int32();
}

s64 Variable_reference::int64() const {
	return m_value->int64();
}

f32 Variable_reference::float32() const {
	return m_value->float32();
}

f64 Variable_reference::float64() const {
	return m_value->float64();
}

str8 Variable_reference::string() const {
	return m_value->string();
}

str8 & Variable_reference::string(str8 &ret) const {
	return m_value->string(ret);
}

size_t Variable_reference::size() const {
	return m_value->size();
}

Variable * Variable_reference::at(size_t index) const {
	return m_value->at(index);
}

Variable * Variable_reference::at(const str8 &index) const {
	return m_value->at(index);
}

const str8 & Variable_reference::name() const {
	return m_name;
}

Variable * Variable_reference::get() const {
	return m_value.ptr();
}

void Variable_reference::set(Variable *value, bool grab) {
	if (value) {
		m_value = value;
		if (grab) {
			value->grab();
		}
	} else {
		m_value = Variable_null::instance();
		m_value->grab();
	}
}

void Variable_reference::_save(DataStream &stream) const {
	stream << m_name;
	save(m_value.ptr(), stream);
}

void Variable_reference::_load(DataStream &stream) {
	stream >> m_name;
	Variable *value = 0;
	load(stream, value);
	m_value = value;
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_array
#endif

Variable_array::Variable_array() {
	
}

Variable_array::~Variable_array() {
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->drop();
	}
}

Variable::Type Variable_array::type() const {
	return kVT_array;
}

bool Variable_array::boolean() const {
	throw InvalidTypeException();
	return false;
}

s32 Variable_array::int32() const {
	throw InvalidTypeException();
	return 0;
}

s64 Variable_array::int64() const {
	throw InvalidTypeException();
	return 0;
}

f32 Variable_array::float32() const {
	throw InvalidTypeException();
	return 0;
}

f64 Variable_array::float64() const {
	throw InvalidTypeException();
	return 0;
}

str8 Variable_array::string() const {
	throw InvalidTypeException();
	return str8();
}

str8 & Variable_array::string(str8 &ret) const {
	throw InvalidTypeException();
	return ret;
}

size_t Variable_array::size() const {
	return m_items.size();
}

Variable * Variable_array::at(size_t index) const {
	if (index < m_items.size()) {
		return m_items[index];
	}
	throw InvalidIndexException();
	return 0;
}

Variable * Variable_array::at(const str8 &index) const {
	throw InvalidIndexException();
	return 0;
}

size_t Variable_array::index_of(Variable *value) const {
	for (size_t i = 0, c = m_items.size(); i < c; ++i) {
		if (value == m_items[i]) {
			return i;
		}
	}
	return -1;
}

void Variable_array::insert(size_t index, Variable *value, bool grab) {
	vnassert(value);
	if (index >= m_items.size()) {
		index = m_items.size();
	}
	if (grab) {
		value->grab();
	}
	m_items.insert(m_items.begin(), value);
}

void Variable_array::push_back(Variable *value, bool grab) {
	vnassert(value);
	if (grab) {
		value->grab();
	}
	m_items.push_back(value);
}

void Variable_array::erase(size_t index) {
	if (index >= m_items.size()) {
		return ;
	}
	m_items[index]->grab();
	m_items.erase(m_items.begin() + index);
}

void Variable_array::clear() {
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->drop();
	}
	m_items.clear();
}

void Variable_array::set(size_t index, Variable *value, bool grab) {
	vnassert(index < m_items.size() && value);
	if (grab) {
		value->grab();
	}
	Variable *&item = m_items[index];
	item->drop();
	item = value;
}

void Variable_array::_save(DataStream &stream) const {
	stream << DataStream::z_u32((u32)m_items.size());
	for (Items::const_iterator it = m_items.begin(); it != m_items.end(); ++it) {
		save(*it, stream);
	}
}

void Variable_array::_load(DataStream &stream) {
	DataStream::z_u32 size;
	stream >> size;
	m_items.reserve(size.value);
	Variable *item = 0;
	for (u32 i = 0; i < size.value; ++i) {
		load(stream, item);
		m_items.push_back(item);
	}
}

#if VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#pragma mark -
#pragma mark Variable_object
#endif

Variable::Type Variable_object::type() const {
	return kVT_object;
}

bool Variable_object::boolean() const {
	throw InvalidTypeException();
	return false;
}

s32 Variable_object::int32() const {
	throw InvalidTypeException();
	return 0;
}

s64 Variable_object::int64() const {
	throw InvalidTypeException();
	return 0;
}

f32 Variable_object::float32() const {
	throw InvalidTypeException();
	return 0;
}

f64 Variable_object::float64() const {
	throw InvalidTypeException();
	return 0;
}

str8 Variable_object::string() const {
	throw InvalidTypeException();
	return str8();
}

str8 & Variable_object::string(str8 &ret) const {
	throw InvalidTypeException();
	return ret;
}

size_t Variable_object::size() const {
	return m_items.size();
}

Variable * Variable_object::at(size_t index) const {
	if (index < m_indices.size()) {
		return m_indices[index]->second.ptr();
	}
	throw InvalidIndexException();
	return 0;
}

Variable * Variable_object::at(const str8 &index) const {
	ItemMap::const_iterator it = m_items.find(index);
	if (it == m_items.end()) {
		throw InvalidIndexException();
	}
	return it->second.ptr();
}

size_t Variable_object::index_of(const str8 &name) const {
	ItemMap::const_iterator it = m_items.find(name);
	if (it == m_items.end()) {
		return -1;
	}
	size_t index = 0;
	while (m_indices[index] != it) {
		++index;
	}
	return index;
}

const str8 & Variable_object::name_of(size_t index) const {
	if (index >= m_indices.size()) {
		throw InvalidIndexException();
	}
	return m_indices[index]->first;
}

void Variable_object::set(const str8 &name, Variable *value, bool grab) {
	vnassert(value);
	if (grab) {
		value->grab();
	}
	std::pair<ItemMap::iterator, bool> ret = m_items.insert(std::make_pair(name, VariablePtr()));
	ret.first->second = value;
	if (ret.second) {
		m_indices.push_back(ret.first);
	}
}

void Variable_object::set(size_t index, Variable *value, bool grab) {
	vnassert(value);
	if (index >= m_indices.size()) {
		throw InvalidIndexException();
	}
	if (grab) {
		value->grab();
	}
	m_indices[index]->second = value;
}

void Variable_object::erase(const str8 &name) {
	ItemMap::iterator it = m_items.find(name);
	if (it == m_items.end()) {
		return ;
	}
	ItemIndices::iterator i = m_indices.begin();
	while (*i != it) {
		++i;
	}
	m_indices.erase(i);
	m_items.erase(it);
}

void Variable_object::erase(size_t index) {
	if (index >= m_items.size()) {
		return ;
	}
	m_items.erase(m_indices[index]);
	m_indices.erase(m_indices.begin() + index);
}

void Variable_object::clear() {
	m_indices.clear();
	m_items.clear();
}

void Variable_object::_save(DataStream &stream) const {
	stream << DataStream::z_u32((u32)m_indices.size());
	for (ItemIndices::const_iterator it = m_indices.begin(); it != m_indices.end(); ++it) {
		stream << (*it)->first;
		save((*it)->second.ptr(), stream);
	}
}

void Variable_object::_load(DataStream &stream) {
	DataStream::z_u32 size;
	stream >> size;
	m_indices.reserve(size.value);
	str8 name;
	Variable *value = 0;
	for (u32 i = 0; i < size.value; ++i) {
		stream >> name;
		load(stream, value);
		m_indices.push_back(m_items.insert(std::make_pair(name, value)).first);
	}
}

_vn_end