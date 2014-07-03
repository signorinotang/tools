//
//  vnTreeData.h
//  vnbase
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnTreeData_h
#define vnbase_vnTreeData_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

#include <map>
#include <vector>

_vn_begin



class _vn_base_api TreeData : public RefCounted {
public:
    enum Type {
        kNull = 0,
        kBoolean,
        kInteger,
        kLongInteger,
        kFloat,
        kDouble,
        kString,
        kArray,
        kObject,
        kNode
    };
    virtual Type type() const = 0;
    virtual TreeData * clone() const = 0;

    
    static void save(TreeData *value, DataStream &ds);
    static bool load(DataStream &ds, TreeData *&result);
    
protected:
    virtual void _save(DataStream &ds) const = 0;
    virtual void _load(DataStream &ds) = 0;
};

typedef RefCountedPtr<TreeData> TreeDataPtr;


class _vn_base_api TreeDataNumber : public TreeData {
public:
    virtual bool getBoolean() const = 0;
    virtual s32 getInteger() const = 0;
    virtual s64 getLongIntgeger() const = 0;
    virtual f32 getFloat() const = 0;
    virtual f64 getDouble() const = 0;
};


class _vn_base_api TreeDataBoolean : public TreeDataNumber {
public:
    TreeDataBoolean();
    TreeDataBoolean(bool _value);
    TreeDataBoolean(const TreeDataBoolean &other);
    virtual TreeDataBoolean * clone() const;
    
    virtual Type type() const { return kBoolean; }
    virtual bool getBoolean() const;
    virtual s32 getInteger() const;
    virtual s64 getLongIntgeger() const;
    virtual f32 getFloat() const;
    virtual f64 getDouble() const;
    
    bool & value();
    bool value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    bool m_value;
};

class _vn_base_api TreeDataInteger : public TreeDataNumber {
public:
    TreeDataInteger();
    TreeDataInteger(s32 _value);
    TreeDataInteger(const TreeDataInteger &other);
    virtual TreeDataInteger * clone() const;
    
	virtual Type type() const { return kInteger; }
    virtual bool getBoolean() const;
    virtual s32 getInteger() const;
    virtual s64 getLongIntgeger() const;
    virtual f32 getFloat() const;
    virtual f64 getDouble() const;
    
    s32 & value();
    s32 value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    s32 m_value;
};

class _vn_base_api TreeDataLongInteger : public TreeDataNumber {
public:
    TreeDataLongInteger();
    TreeDataLongInteger(s64 _value);
    TreeDataLongInteger(const TreeDataLongInteger &other);
    virtual TreeDataLongInteger * clone() const;
    
	virtual Type type() const { return kLongInteger; }
    virtual bool getBoolean() const;
    virtual s32 getInteger() const;
    virtual s64 getLongIntgeger() const;
    virtual f32 getFloat() const;
    virtual f64 getDouble() const;
    
    s64 & value();
    s64 value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    s64 m_value;
};

class _vn_base_api TreeDataFloat : public TreeDataNumber {
public:
    TreeDataFloat();
    TreeDataFloat(f32 _value);
    TreeDataFloat(const TreeDataFloat &other);
    virtual TreeDataFloat * clone() const;
    
	virtual Type type() const { return kFloat; }
    virtual bool getBoolean() const;
    virtual s32 getInteger() const;
    virtual s64 getLongIntgeger() const;
    virtual f32 getFloat() const;
    virtual f64 getDouble() const;
    
    f32 & value();
    f32 value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    f32 m_value;
};

class _vn_base_api TreeDataDouble : public TreeDataNumber {
public:
    TreeDataDouble();
    TreeDataDouble(f64 _value);
    TreeDataDouble(const TreeDataDouble &other);
    virtual TreeDataDouble * clone() const;
    
	virtual Type type() const { return kDouble; }
    virtual bool getBoolean() const;
    virtual s32 getInteger() const;
    virtual s64 getLongIntgeger() const;
    virtual f32 getFloat() const;
    virtual f64 getDouble() const;
    
    f64 & value();
    f64 value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    f64 m_value;
};

class _vn_base_api TreeDataString : public TreeData {
public:
    TreeDataString();
    TreeDataString(const str8 &_value);
    TreeDataString(const TreeDataString &other);
    virtual TreeDataString * clone() const;
    
	virtual Type type() const { return kString; }
    
    str8 & value();
    const str8 & value() const;
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    str8 m_value;
};

class _vn_base_api TreeDataArray : public TreeData {
public:
    TreeDataArray();
    TreeDataArray(const TreeDataArray &other);
    ~TreeDataArray();
    virtual TreeDataArray * clone() const;
    
	virtual Type type() const { return kArray; }
    
    size_t size() const;
    
    const TreeData * at(size_t index) const;
    TreeData * at(size_t index);
    
    void push_back(TreeData *value, bool grab = true);
    void insert(size_t index, TreeData *value, bool grab = true);
    void erase(size_t index);	
    
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    typedef std::vector<TreeData *> Values;
    Values m_values;
};

class TreeDataNode;

class _vn_base_api TreeDataObject : public TreeData {
public:
    TreeDataObject();
    TreeDataObject(const TreeDataObject &other);
    virtual TreeDataObject * clone() const;
    
	virtual Type type() const{ return kObject; }
    
    const str8 & name() const;
    TreeData * findAttribute(const str8 &name);
    const TreeData * findAttribute(const str8 &name) const;
    bool queryAttributeBoolean(const str8 &name, bool errval = false) const;
    s32 queryAttributeInteger(const str8 &name, s32 errval = 0) const;
    s64 queryAttributeLongInteger(const str8 &name, s64 errval = 0) const;
    f32 queryAttributeFloat(const str8 &name, f32 errval = 0) const;
    f64 queryAttributeDouble(const str8 &name, f64 errval = 0) const;
    bool queryAttributeString(const str8 &name, str8 &result) const;
    const TreeDataString * queryAttributeString(const str8 &name) const;
    const TreeDataArray * queryAttributeArray(const str8 &name) const;
    const TreeDataObject * queryAttributeObject(const str8 &name) const;
    const TreeDataNode * queryAttributeNode(const str8 &name) const;
    
    void _setName(const str8 &name);
    void _setAttribute(const str8 &name, TreeData *value, bool grab = true);
    
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    str8 m_name;
    typedef std::map<str8, TreeDataPtr> AttributeMap;
    AttributeMap m_attributes;
};

class _vn_base_api TreeDataNode : public TreeDataObject {
public:
    TreeDataNode();
    TreeDataNode(const TreeDataNode &other);
    ~TreeDataNode();
    virtual TreeDataNode * clone() const;
    
	virtual Type type() const { return kNode; }
    
    TreeDataNode * firstChild() const;
    TreeDataNode * firstChild(const str8 &name) const;
    TreeDataNode * nextNeighbour() const;
    TreeDataNode * nextNeighbour(const str8 &name) const;
    
    void _setFirstChild(TreeDataNode *node, bool grab = true);
    void _setNextNeighbour(TreeDataNode *node, bool grab = true);
    
protected:
    virtual void _save(DataStream &ds) const;
    virtual void _load(DataStream &ds);
    
    TreeDataNode *m_firstChild;
    TreeDataNode *m_nextNeighbour;
};

_vn_end

#endif
