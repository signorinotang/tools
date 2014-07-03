//
//  vnTreeData.cpp
//  vnbase
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnTreeData.h"

_vn_begin

void TreeData::save(TreeData *value, DataStream &ds) {
    if (value) {
        ds << (u8)value->type();
        value->_save(ds);
    } else {
        ds << (u8)kNull;
    }
}

bool TreeData::load(DataStream &ds, TreeData *&result) {
    try {
        u8 type = 0;
        ds >> type;
        switch (type) {
            case kNull:
                result = 0;
                return true;
                
            case kBoolean:
                result = vnnew TreeDataBoolean();
                break;
            
            case kInteger:
                result = vnnew TreeDataInteger();
                break;
                
            case kLongInteger:
                result = vnnew TreeDataLongInteger();
                break;
                
            case kFloat:
                result = vnnew TreeDataFloat();
                break;
                
            case kDouble:
                result = vnnew TreeDataDouble();
                break;
                
            case kString:
                result = vnnew TreeDataString();
                break;
                
            case kArray:
                result = vnnew TreeDataArray();
                break;
                
            case kObject:
                result = vnnew TreeDataObject();
                break;
                
            case kNode:
                result = vnnew TreeDataNode();
                break;
            
            default:
                result = 0;
                return false;
        }
        result->_load(ds);
    } catch (DataStream::Exception &e) {
        result = 0;
        throw e;
    }
    return true;
}
//**********************TreeDataBoolean******************************
TreeDataBoolean::TreeDataBoolean()
: m_value(false) {
    
}

TreeDataBoolean::TreeDataBoolean(bool _value)
: m_value(_value) {
    
}

TreeDataBoolean::TreeDataBoolean(const TreeDataBoolean &other)
: m_value(other.m_value) {
    
}

TreeDataBoolean * TreeDataBoolean::clone() const {
    return vnnew TreeDataBoolean(m_value);
}

bool & TreeDataBoolean::value() {
    return m_value;
}

bool TreeDataBoolean::value() const {
    return m_value;
}

void TreeDataBoolean::_save(DataStream &ds) const {
    ds << m_value;
}

void TreeDataBoolean::_load(DataStream &ds) {
    ds >> m_value;
}

bool TreeDataBoolean::getBoolean() const
{
	return m_value;
}
s32 TreeDataBoolean::getInteger() const
{
	return m_value ? 1 : 0;
}
s64 TreeDataBoolean::getLongIntgeger() const
{
	return m_value ? 1 : 0;
}
f32 TreeDataBoolean::getFloat() const
{
	return (f32)(m_value ? 1 : 0);
}
f64 TreeDataBoolean::getDouble() const
{
	return m_value ? 1 : 0;
}
//********************TreeDataInteger***************************************
TreeDataInteger::TreeDataInteger()
: m_value(0)
{

}

TreeDataInteger::TreeDataInteger(s32 _value)
: m_value(_value)
{

}

TreeDataInteger::TreeDataInteger(const TreeDataInteger& other)
: m_value(other.m_value)
{

}

TreeDataInteger* TreeDataInteger::clone() const
{
	return vnnew TreeDataInteger(m_value);
}

s32 & TreeDataInteger::value()
{
	return m_value;
}

s32 TreeDataInteger::value() const
{
	return m_value;
}


void TreeDataInteger::_save(DataStream &ds) const {
	ds << m_value;
}

void TreeDataInteger::_load(DataStream &ds) {
	ds >> m_value;
}

bool TreeDataInteger::getBoolean() const
{
	return m_value != 0;
}
s32 TreeDataInteger::getInteger() const
{
	return (s32)m_value;
}
s64 TreeDataInteger::getLongIntgeger() const
{
	return (s64)m_value;
}
f32 TreeDataInteger::getFloat() const
{
	return (f32)m_value;
}
f64 TreeDataInteger::getDouble() const
{
	return (f64)m_value;
}

//***************************TreeDataLongInteger*******************************
TreeDataLongInteger::TreeDataLongInteger()
: m_value(0){

}

TreeDataLongInteger::TreeDataLongInteger(s64 _value)
: m_value(_value){

}

TreeDataLongInteger::TreeDataLongInteger(const TreeDataLongInteger& other)
: m_value(other.m_value){

}

TreeDataLongInteger* TreeDataLongInteger::clone() const { 
	return vnnew TreeDataLongInteger(m_value);
}

s64 & TreeDataLongInteger::value() {
	return m_value;
}

s64 TreeDataLongInteger::value() const {
	return m_value;
}

void TreeDataLongInteger::_save(DataStream &ds) const {
	ds << m_value;
}

void TreeDataLongInteger::_load(DataStream &ds) {
	ds >> m_value;
}

bool TreeDataLongInteger::getBoolean() const
{
	return m_value != 0;
}
s32 TreeDataLongInteger::getInteger() const
{
	return (s32)m_value;
}
s64 TreeDataLongInteger::getLongIntgeger() const
{
	return (s64)m_value;
}
f32 TreeDataLongInteger::getFloat() const
{
	return (f32)m_value;
}
f64 TreeDataLongInteger::getDouble() const
{
	return (f64)m_value;
}

//**********************TreeDataFloat*********************************
TreeDataFloat::TreeDataFloat()
:m_value(0){

}

TreeDataFloat::TreeDataFloat(f32 _value)
: m_value(_value){

}

TreeDataFloat::TreeDataFloat(const TreeDataFloat& other)
:m_value(other.m_value){

}

TreeDataFloat* TreeDataFloat::clone() const{
	return vnnew TreeDataFloat(m_value);
}

f32 & TreeDataFloat::value(){
	return m_value;
}

f32 TreeDataFloat::value() const {
	return m_value;
}

void TreeDataFloat::_save(DataStream &ds) const {
	ds << m_value;
}

void TreeDataFloat::_load(DataStream &ds) {
	ds >> m_value;
}

bool TreeDataFloat::getBoolean() const
{
	return m_value != 0;
}
s32 TreeDataFloat::getInteger() const
{
	return (s32)m_value;
}
s64 TreeDataFloat::getLongIntgeger() const
{
	return (s64)m_value;
}
f32 TreeDataFloat::getFloat() const
{
	return (f32)m_value;
}
f64 TreeDataFloat::getDouble() const
{
	return (f64)m_value;
}

//****************************TreeDataDouble***************************
TreeDataDouble::TreeDataDouble()
:m_value(0){

}

TreeDataDouble::TreeDataDouble(f64 _value)
: m_value(_value){

}

TreeDataDouble::TreeDataDouble(const TreeDataDouble& other)
: m_value(other.m_value){

}

TreeDataDouble* TreeDataDouble::clone() const{
	return vnnew TreeDataDouble(m_value);
}

f64 & TreeDataDouble::value(){
	return m_value;
}

f64 TreeDataDouble::value() const {
	return m_value;
}

void TreeDataDouble::_save(DataStream &ds) const {
	ds << m_value;
}

void TreeDataDouble::_load(DataStream &ds) {
	ds >> m_value;
}

bool TreeDataDouble::getBoolean() const
{
	return m_value != 0;
}
s32 TreeDataDouble::getInteger() const
{
	return (s32)m_value;
}
s64 TreeDataDouble::getLongIntgeger() const
{
	return (s64)m_value;
}
f32 TreeDataDouble::getFloat() const
{
	return (f32)m_value;
}
f64 TreeDataDouble::getDouble() const
{
	return (f64)m_value;
}

//**********************TreeDataString*******************************
TreeDataString::TreeDataString() {
	
}

TreeDataString::TreeDataString(const str8 &_value)
: m_value(_value){

}

TreeDataString::TreeDataString(const TreeDataString& other)
: m_value(other.m_value){

}

TreeDataString* TreeDataString::clone() const {
	return vnnew TreeDataString(m_value);
}

str8 & TreeDataString::value(){
	return m_value;
}

const str8 & TreeDataString::value() const {
	return m_value;
}

void TreeDataString::_save(DataStream &ds) const {
	ds << m_value;
}

void TreeDataString::_load(DataStream &ds) {
	ds >> m_value;
}

//********************TreeDataArray*******************************
TreeDataArray::TreeDataArray()
{

}

TreeDataArray::TreeDataArray(const TreeDataArray &other)
: m_values(other.m_values) {
	for (Values::iterator it = m_values.begin(); it != m_values.end(); ++it) {
		*it = (*it)->clone();
	}
}

TreeDataArray::~TreeDataArray() {
	for (Values::const_iterator it = m_values.begin(); it != m_values.end(); ++it) {
		(*it)->drop();
	}
}

TreeDataArray* TreeDataArray::clone() const{
	return vnnew TreeDataArray(*this);
}

size_t TreeDataArray::size() const{
	return m_values.size();
}

const TreeData * TreeDataArray::at(size_t index) const{
	return m_values.at(index);
}

void TreeDataArray::push_back(TreeData* value, bool grab)
{	
	if (grab)
	{
		value->grab();
	}
	m_values.push_back(value);
}

void TreeDataArray::insert(size_t index, TreeData* value, bool grab)
{
	if (grab)
	{
		value->grab();
	}
	m_values.insert(m_values.begin()+index, value);
}

void TreeDataArray::erase(size_t index)
{
	m_values.at(index)->drop();
	m_values.erase(m_values.begin() + index);
}

void TreeDataArray::_save(DataStream &ds) const
{
	ds << DataStream::z_u32((u32)m_values.size());
	for (Values::const_iterator it = m_values.begin(); it != m_values.end(); ++it) {
		save(*it, ds);
	}
}

void TreeDataArray::_load(DataStream &ds)
{
	vnassert(m_values.empty());
	DataStream::z_u32 _size;
	ds >> _size;
	m_values.assign(_size.value, 0);
	for (u32 i = 0; i < _size.value; ++i) {
		TreeData *ptr = 0;
		load(ds, ptr);
		vnassert(ptr);
		m_values[i] = ptr;
	}
}
//********************TreeDataObject******************************
TreeDataObject::TreeDataObject()
{

}

TreeDataObject::TreeDataObject(const TreeDataObject& other)
: m_name(other.m_name)
, m_attributes(other.m_attributes)
{
	for (AttributeMap::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
		it->second = it->second->clone();
	}
}

TreeDataObject * TreeDataObject::clone() const
{
	return vnnew TreeDataObject(*this);
}

const str8 & TreeDataObject::name() const
{
	return m_name;
}

void TreeDataObject::_setName(const str8 &name)
{
	m_name = name;
}

const TreeData * TreeDataObject::findAttribute(const str8 &name) const {
    AttributeMap::const_iterator it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return 0;
    }
    return it->second.ptr();
}

TreeData * TreeDataObject::findAttribute(const str8 &name) {
    AttributeMap::const_iterator it = m_attributes.find(name);
    if (it == m_attributes.end()) {
        return 0;
    }
    return it->second.ptr();
}

void TreeDataObject::_setAttribute(const str8 &name, TreeData *value, bool grab) {
	vnassert(value);
	if (grab) {
		value->grab();
	}
	m_attributes[name] = value;
}

bool TreeDataObject::queryAttributeBoolean(const str8 &name, bool errval) const
{
	AttributeMap::const_iterator it = m_attributes.find(name);

	if (it != m_attributes.end())
	{
		TreeDataNumber* tmp = dynamic_cast<TreeDataNumber*>(it->second.ptr());
		if (tmp)
		{
			return tmp->getBoolean();
		}	
	}
	return errval;

}
s32 TreeDataObject::queryAttributeInteger(const str8 &name, s32 errval) const
{
	AttributeMap::const_iterator it = m_attributes.find(name);

	if (it != m_attributes.end())
	{
		TreeDataNumber* tmp = dynamic_cast<TreeDataNumber*>(it->second.ptr());
		if (tmp)
		{
			return tmp->getInteger();
		}
	}
	return errval;
	
}
s64 TreeDataObject::queryAttributeLongInteger(const str8 &name, s64 errval) const
{
	AttributeMap::const_iterator it = m_attributes.find(name);

	if (it != m_attributes.end())
	{
		TreeDataNumber* tmp = dynamic_cast<TreeDataNumber*>(it->second.ptr());
		if (tmp)
		{
			return tmp->getLongIntgeger();
		}
	}
	return errval;
}
f32 TreeDataObject::queryAttributeFloat(const str8 &name, f32 errval) const
{
	AttributeMap::const_iterator it = m_attributes.find(name);

	if (it != m_attributes.end())
	{
		TreeDataNumber* tmp = dynamic_cast<TreeDataNumber*>(it->second.ptr());
		if (tmp)
		{
			return tmp->getFloat();
		}
	}
	return errval;
}
f64 TreeDataObject::queryAttributeDouble(const str8 &name, f64 errval) const
{
	AttributeMap::const_iterator it = m_attributes.find(name);

	if (it != m_attributes.end())
	{
		TreeDataNumber* tmp = dynamic_cast<TreeDataNumber*>(it->second.ptr());
		if (tmp)
		{
			return tmp->getDouble();
		}
	}
	return errval;
}
bool TreeDataObject::queryAttributeString(const str8 &name, str8 &result) const {
    const TreeDataString *tdstr = queryAttributeString(name);
    if (!tdstr) {
        return false;
    }
    result = tdstr->value();
    return true;
}
const TreeDataString * TreeDataObject::queryAttributeString(const str8 &name) const {
	AttributeMap::const_iterator it = m_attributes.find(name);
	if (it == m_attributes.end()) {
        return 0;
    }
	return dynamic_cast<const TreeDataString *>(it->second.ptr());
}

const TreeDataArray * TreeDataObject::queryAttributeArray(const str8 &name) const {
    AttributeMap::const_iterator it = m_attributes.find(name);
	if (it == m_attributes.end()) {
        return 0;
    }
	return dynamic_cast<const TreeDataArray *>(it->second.ptr());
}

const TreeDataObject * TreeDataObject::queryAttributeObject(const str8 &name) const {
    AttributeMap::const_iterator it = m_attributes.find(name);
	if (it == m_attributes.end()) {
        return 0;
    }
	return dynamic_cast<const TreeDataObject *>(it->second.ptr());
}

const TreeDataNode * TreeDataObject::queryAttributeNode(const str8 &name) const {
	AttributeMap::const_iterator it = m_attributes.find(name);
	if (it == m_attributes.end()) {
        return 0;
    }
	return dynamic_cast<const TreeDataNode *>(it->second.ptr());
}

void TreeDataObject::_save(DataStream &ds) const
{
	ds << m_name;
	ds << DataStream::z_u32((u32)m_attributes.size());

	for (AttributeMap::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
		ds << it->first;
		save(it->second.ptr(), ds);
	}
}

void TreeDataObject::_load(DataStream &ds)
{
	vnassert(m_attributes.empty());
	DataStream::z_u32 _size;
	ds >> m_name;
	ds >> _size;
	for (u32 i = 0; i < _size.value; ++i) {
		TreeData *ptr = 0;
		str8 name;
		ds >> name;
		load(ds, ptr);
		vnassert(ptr);
		m_attributes.insert(std::make_pair(name, ptr));
	}
}




//********************TreeDataNode********************************
TreeDataNode::TreeDataNode()
: m_firstChild(0)
, m_nextNeighbour(0)
{

}

TreeDataNode::TreeDataNode(const TreeDataNode& other)
: m_firstChild(other.m_firstChild->clone())
, m_nextNeighbour(other.m_nextNeighbour->clone()) {
    
}

TreeDataNode::~TreeDataNode() {
    VN_SAFE_DROP(m_firstChild);
    VN_SAFE_DROP(m_nextNeighbour);
}

TreeDataNode* TreeDataNode::clone() const
{
	return vnnew TreeDataNode(*this);
}

TreeDataNode* TreeDataNode::firstChild() const
{
	return m_firstChild;
}

TreeDataNode* TreeDataNode::firstChild(const str8& name) const 
{
	if (!m_firstChild)
	{
		return 0;
	}

	if (m_firstChild->m_name == name)
	{
		return m_firstChild;
	}
	
	return 	m_nextNeighbour->firstChild(name);
}

TreeDataNode* TreeDataNode::nextNeighbour() const
{	
	return m_nextNeighbour;
}

TreeDataNode* TreeDataNode::nextNeighbour(const str8& name) const
{
	if (!m_nextNeighbour)
	{
		return 0;
	}

	if (m_nextNeighbour->m_name == name)
	{
		return m_nextNeighbour;
	}
	
	return 	m_nextNeighbour->nextNeighbour(name);
	
}

void TreeDataNode::_setFirstChild(TreeDataNode *node, bool grab)
{
	if (m_firstChild) {
		m_firstChild->drop();
	}
	if ((m_firstChild = node) && grab) {
		m_firstChild->grab();
	}
}
void TreeDataNode::_setNextNeighbour(TreeDataNode *node, bool grab)
{

	if (m_nextNeighbour) {
		m_nextNeighbour->drop();
	}
	if ((m_nextNeighbour = node) && grab) {
		m_nextNeighbour->grab();
	}

}

void TreeDataNode::_save(DataStream &ds) const
{
	TreeDataObject::_save(ds);
	save(m_firstChild, ds);
	save(m_nextNeighbour, ds);
}
void TreeDataNode::_load(DataStream &ds)
{
	TreeDataObject::_load(ds);
	TreeData *ptr = 0;
	load(ds, ptr);
	m_firstChild = (TreeDataNode *)ptr;
	load(ds, ptr);
	m_nextNeighbour = (TreeDataNode *)ptr;
}


_vn_end