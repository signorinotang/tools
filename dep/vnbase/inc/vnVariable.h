//
//  vnVariable.h
//  vnbase
//
//  Created by Wayne on 13-10-4.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnVariable_h
#define vnbase_vnVariable_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

#include <vector>
#include <map>

_vn_begin

class _vn_base_api Variable : public RefCounted {
public:
	enum Type {
		kVT_null = 0,
		kVT_bool,
		kVT_int32,
		kVT_int64,
		kVT_float,
		kVT_double,
		kVT_string,
		kVT_reference,
		kVT_array,
		kVT_object
	};
	
	struct Exception {
		enum Type {
			kUnknown = 0,
			kLoad,
			kNullReference,
			kInvalidType,
			kInvalidIndex
		} type;
		Exception(Type _t) : type(_t) {}
	};
	
	struct LoadException : Exception {
		LoadException() : Exception(kLoad) {}
	};
	
	struct NullReferenceException : Exception {
		NullReferenceException() : Exception(kNullReference) {}
	};
	
	struct InvalidTypeException : Exception {
		InvalidTypeException() : Exception(kInvalidType) {}
	};
	
	struct InvalidIndexException : Exception {
		InvalidIndexException() : Exception(kInvalidIndex) {}
	};
	
	virtual Type type() const = 0;
	virtual bool boolean() const = 0;
	virtual s32 int32() const = 0;
	virtual s64 int64() const = 0;
	virtual f32 float32() const = 0;
	virtual f64 float64() const = 0;
	virtual str8 string() const = 0;
	virtual str8 & string(str8 &) const = 0;
	
	virtual size_t size() const = 0;
	virtual Variable * at(size_t index) const = 0;
	virtual Variable * at(const str8 &index) const = 0;
	
    bool numberic() const;
    
	static void save(const Variable *value, DataStream &stream);
	static void load(DataStream &stream, Variable *&value);
	
protected:
	virtual void _save(DataStream &stream) const = 0;
	virtual void _load(DataStream &stream) = 0;
};

typedef RefCountedPtr<Variable> VariablePtr;

class _vn_base_api Variable_null : public Variable {
public:
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	static Variable_null * instance();
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_bool : public Variable {
public:
	Variable_bool();
	Variable_bool(bool _v);
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	bool value;
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_int32 : public Variable {
public:
	Variable_int32();
	Variable_int32(s32 _v);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	s32 value;
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_int64 : public Variable {
public:
	Variable_int64();
	Variable_int64(s64 _v);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	s64 value;
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_float32 : public Variable {
public:
	Variable_float32();
	Variable_float32(f32 _v);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	f32 value;

protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_float64 : public Variable {
public:
	Variable_float64();
	Variable_float64(f64 _v);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	f64 value;
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_string : public Variable {
public:
	Variable_string();
	Variable_string(const str8 &_v);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	str8 value;
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
};

class _vn_base_api Variable_reference : public Variable {
public:
	Variable_reference();
	Variable_reference(Variable *value, bool grab = true);
	Variable_reference(const str8 &name);
	Variable_reference(const str8 &name, Variable *value, bool grab = true);
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	const str8 & name() const;
	Variable * get() const;
	void set(Variable *value, bool grab = true);
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
	
private:
	str8 m_name;
	VariablePtr m_value;
};

class _vn_base_api Variable_array : public Variable {
public:
	Variable_array();
	virtual ~Variable_array();
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	size_t index_of(Variable *value) const;
	void insert(size_t index, Variable *value, bool grab = true);
	void push_back(Variable *value, bool grab = true);
	void erase(size_t index);
	void clear();
	
	void set(size_t index, Variable *value, bool grab = true);
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
	
	typedef std::vector<Variable *> Items;
	
	Items m_items;
};

class _vn_base_api Variable_object : public Variable {
public:
	
	virtual Type type() const;
	virtual bool boolean() const;
	virtual s32 int32() const;
	virtual s64 int64() const;
	virtual f32 float32() const;
	virtual f64 float64() const;
	virtual str8 string() const;
	virtual str8 & string(str8 &) const;
	
	virtual size_t size() const;
	virtual Variable * at(size_t index) const;
	virtual Variable * at(const str8 &index) const;
	
	size_t index_of(const str8 &name) const;
	const str8 & name_of(size_t index) const;
	
	void set(const str8 &name, Variable *value, bool grab = true);
	void set(size_t index, Variable *value, bool grab = true);
	void erase(const str8 &name);
	void erase(size_t index);
	void clear();
	
protected:
	virtual void _save(DataStream &stream) const;
	virtual void _load(DataStream &stream);
	
	typedef std::map<str8, VariablePtr> ItemMap;
	typedef std::vector<ItemMap::iterator> ItemIndices;
	
	ItemMap m_items;
	ItemIndices m_indices;
};

_vn_end

#endif
