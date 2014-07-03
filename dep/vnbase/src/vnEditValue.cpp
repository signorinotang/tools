//
//  vnEditValue.cpp
//  vnbase
//
//  Created by Wayne on 14-1-3.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnEditValue.h"
#include "vnEditType.h"

#include <sstream>

_vn_begin

EditValue::EditValue(const EditType *_type)
: m_type(_type) {
    vnassert(_type);
    m_type->grab();
}

EditValue::EditValue(const EditValue &other)
: m_type(other.m_type) {
    m_type->grab();
}

EditValue::~EditValue() {
	for (Observers::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->ev_destroyed(this);
	}
    m_type->drop();
}

const EditType * EditValue::type() const {
    return m_type;
}

str8 EditValue::desc() const {
    str8 s;
    return desc(s);
}

bool EditValue::verity() const {
    return m_type->verity(this);
}

void EditValue::addObserver(Observer *observer) {
	m_observers.push_back(observer);
}

void EditValue::removeObserver(Observer *observer) {
	for (Observers::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		if (*it == observer) {
			m_observers.erase(it);
			break;
		}
	}
}

void EditValue::_notify_changed_backup(const BackupData *data) {
	for (Observers::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->ev_changed_backup(this, data);
	}
}

void EditValue::_notify_changed() {
	for (Observers::iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
		(*it)->ev_changed(this);
	}
}

EditValue::BackupData::BackupData(const EditType *type)
: m_type(type) {
	m_type->grab();
}

EditValue::BackupData::~BackupData() {
	m_type->drop();
}

const EditType * EditValue::BackupData::type() const {
	return m_type;
}

////////////////////////////////////////////

EditValueObserverProxy::EditValueObserverProxy()
: m_target(0) {
	
}

EditValueObserverProxy::~EditValueObserverProxy() {
	if (m_target) {
		m_target->removeObserver(this);
		m_target = 0;
	}
}

void EditValueObserverProxy::setTarget(EditValue *target) {
	if (m_target) {
		m_target->removeObserver(this);
	}
	if ((m_target = target)) {
		m_target->addObserver(this);
	}
}

void EditValueObserverProxy::ev_changed(EditValue *target) {
	if (!m_fnChanged.null()) {
		m_fnChanged(target);
	}
}

void EditValueObserverProxy::ev_changed_backup(EditValue *target, const EditValue::BackupData *data) {
	if (!m_fnChangedBackup.null()) {
		m_fnChangedBackup(target, data);
	}
}

void EditValueObserverProxy::ev_destroyed(EditValue *target) {
	if (!m_fnDestroyed.null()) {
		m_fnDestroyed(target);
	}
	m_target = 0;
}

void EditValueObserverProxy::setFunction_Changed(const function<void (EditValue *)> &func) {
	m_fnChanged = func;
}

void EditValueObserverProxy::setFunction_ChangedBackup(const function<void(EditValue *, const EditValue::BackupData *)> &func) {
	m_fnChangedBackup = func;
}

void EditValueObserverProxy::setFunction_Destroyed(const function<void (EditValue *)> &func) {
	m_fnDestroyed = func;
}


////////////////////////////////////////////

EditValueBool::EditValueBool(const EditTypeBool *_type)
: EditValue(_type)
, m_value(false) {
    
}

EditValueBool * EditValueBool::clone() const {
    return vnnew EditValueBool(*this);
}

str8 & EditValueBool::desc(str8 &s) const {
    s = (m_value ? "true" : "false");
    return s;
}

const EditValue::BackupData * EditValueBool::backup() const {
	struct _BackupData : BackupData {
		bool m_value;
		_BackupData(const EditType *type, bool value)
		: BackupData(type), m_value(value) {}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			static_cast<EditValueBool *>(value)->setValue(m_value, false);
			return true;
		}
	};
	return vnnew _BackupData(m_type, m_value);
}

const EditTypeBool * EditValueBool::type() const {
    return (const EditTypeBool *)m_type;
}

void EditValueBool::setValue(bool value, bool _backup) {
	if (m_value == value) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		m_value = value;
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_value = value;
	}
	_notify_changed();
}

bool EditValueBool::getValue() const {
    return m_value;
}

////////////////////////////////////////////

EditValueInt::EditValueInt(const EditTypeInt *_type)
: EditValue(_type)
, m_value(0) {
    
}

EditValueInt * EditValueInt::clone() const {
    return vnnew EditValueInt(*this);
}

str8 & EditValueInt::desc(str8 &s) const {
    c8 buf[16];
    s.assign(buf, sprintf(buf, "%d", m_value));
    return s;
}

const EditValue::BackupData * EditValueInt::backup() const {
	struct _BackupData : BackupData {
		s32 m_value;
		_BackupData(const EditType *type, s32 value)
		: BackupData(type), m_value(value) {}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			static_cast<EditValueInt *>(value)->setValue(m_value, false);
			return true;
		}
	};
	return vnnew _BackupData(m_type, m_value);
}


const EditTypeInt * EditValueInt::type() const {
    return (const EditTypeInt *)m_type;
}

void EditValueInt::setValue(s32 value, bool _backup) {
	if (m_value == value) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		m_value = value;
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_value = value;
	}
	_notify_changed();
}

s32 EditValueInt::getValue() const {
    return m_value;
}

////////////////////////////////////////////

EditValueFloat::EditValueFloat(const EditTypeFloat *_type)
: EditValue(_type)
, m_value(0) {
    
}

EditValueFloat * EditValueFloat::clone() const {
    return vnnew EditValueFloat(*this);
}

str8 & EditValueFloat::desc(str8 &s) const {
    c8 buf[16];
    s.assign(buf, sprintf(buf, "%g", m_value));
    return s;
}

const EditValue::BackupData * EditValueFloat::backup() const {
	struct _BackupData : BackupData {
		f32 m_value;
		_BackupData(const EditType *type, f32 value)
		: BackupData(type), m_value(value) {}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			static_cast<EditValueFloat *>(value)->setValue(m_value, false);
			return true;
		}
	};
	return vnnew _BackupData(m_type, m_value);
}

const EditTypeFloat * EditValueFloat::type() const {
    return (const EditTypeFloat *)m_type;
}

void EditValueFloat::setValue(f32 value, bool _backup) {
	if (m_value == value) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		m_value = value;
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_value = value;
	}
	_notify_changed();
}

f32 EditValueFloat::getValue() const {
    return m_value;
}

////////////////////////////////////////////

EditValueString::EditValueString(const EditTypeString *_type)
: EditValue(_type) {
    
}

EditValueString * EditValueString::clone() const {
    return vnnew EditValueString(*this);
}

str8 & EditValueString::desc(str8 &s) const {
    return s = m_value;
}

const EditValue::BackupData * EditValueString::backup() const {
	struct _BackupData : BackupData {
		str8 m_value;
		_BackupData(const EditType *type, const str8 &value)
		: BackupData(type), m_value(value) {}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			static_cast<EditValueString *>(value)->setValue(m_value, false);
			return true;
		}
	};
	return vnnew _BackupData(m_type, m_value);
}

const EditTypeString * EditValueString::type() const {
    return (const EditTypeString *)m_type;
}

void EditValueString::setValue(const str8 &value, bool _backup) {
	if (m_value == value) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		m_value = value;
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_value = value;
	}
	_notify_changed();
}

const str8 & EditValueString::getValue() const {
    return m_value;
}

////////////////////////////////////////////

EditValueArray::EditValueArray(const EditTypeArray *_type)
: EditValue(_type) {
    
}

EditValueArray::EditValueArray(const EditValueArray &other)
: EditValue(other) {
    m_items.reserve(other.m_items.size());
    for (Items::const_iterator it = other.m_items.begin(); it != other.m_items.end(); ++it) {
        m_items.push_back((*it)->clone());
    }
}

EditValueArray::~EditValueArray() {
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        (*it)->drop();
    }
}

EditValueArray * EditValueArray::clone() const {
    return vnnew EditValueArray(*this);
}

str8 & EditValueArray::desc(str8 &s) const {
    c8 buf[32];
    return s.assign(buf, sprintf(buf, "[%ld]", m_items.size()));
}

const EditValue::BackupData * EditValueArray::backup() const {
	struct _BackupData : BackupData {
		Items m_items;
		_BackupData(const EditType *type, const Items &items)
		: BackupData(type), m_items(items) {
			for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
				(*it)->grab();
			}
		}
		~_BackupData() {
			for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
				(*it)->drop();
			}
		}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			static_cast<EditValueArray *>(value)->_restore(m_items);
			return true;
		}
	};
	return vnnew _BackupData(m_type, m_items);
}

const EditTypeArray * EditValueArray::type() const {
    return (const EditTypeArray *)m_type;
}

size_t EditValueArray::size() const {
    return m_items.size();
}

EditValue * EditValueArray::at(size_t index) {
    return m_items[index];
}

const EditValue * EditValueArray::at(size_t index) const {
    return m_items[index];
}

EditValue * EditValueArray::back() {
    return m_items.back();
}

const EditValue * EditValueArray::back() const {
    return m_items.back();
}

EditValue * EditValueArray::front() {
    return m_items.front();
}

const EditValue * EditValueArray::front() const {
    return m_items.front();
}

EditValue * EditValueArray::push_back(bool _backup) {
    EditValue *item = EditValueArray::type()->generateItem();
	if (_backup) {
		const BackupData *data = backup();
		m_items.push_back(item);
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_items.push_back(item);
	}
	_notify_changed();
    return item;
}

EditValue * EditValueArray::insert(size_t position, bool _backup) {
	EditValue *item = EditValueArray::type()->generateItem();
	if (_backup) {
		const BackupData *data = backup();
		m_items.insert(m_items.begin() + position, item);
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_items.insert(m_items.begin() + position, item);
	}
	_notify_changed();
	return item;
}

void EditValueArray::erase(size_t position, bool _backup) {
	if (_backup) {
		const BackupData *data = backup();
		m_items[position]->drop();
		m_items.erase(m_items.begin() + position);
		_notify_changed_backup(data);
		data->drop();
	} else {
		m_items[position]->drop();
		m_items.erase(m_items.begin() + position);
	}
	_notify_changed();
}

void EditValueArray::clear(bool _backup) {
	if (m_items.empty()) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
			(*it)->drop();
		}
		m_items.clear();
		_notify_changed_backup(data);
		data->drop();
	} else {
		for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
			(*it)->drop();
		}
		m_items.clear();
	}
	_notify_changed();
}

void EditValueArray::assign(size_t _size, bool _backup) {
	if (_backup) {
		const BackupData *data = backup();
		_assign(_size);
		_notify_changed_backup(data);
		data->drop();
	} else {
		_assign(_size);
	}
	_notify_changed();
}

void EditValueArray::_assign(size_t _size) {
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->drop();
	}
	m_items.clear();
	m_items.assign(_size, 0);
	for (size_t i = 0; i < _size; ++i) {
		m_items[i] = EditValueArray::type()->generateItem();
	}
}

void EditValueArray::resize(size_t _size, bool _backup) {
	if (_size == m_items.size()) {
		return ;
	}
	if (_backup) {
		const BackupData *data = backup();
		_resize(_size);
		_notify_changed_backup(data);
		data->drop();
	} else {
		_resize(_size);
	}
	_notify_changed();
}

void EditValueArray::_resize(size_t _size) {
    if (_size > m_items.size()) {
        size_t i = m_items.size();
        m_items.resize(_size);
        while (i < _size) {
            m_items[i++] = EditValueArray::type()->generateItem();
        }
    } else if (_size < m_items.size()) {
        for (size_t i = _size, c = m_items.size(); i < c; ++i) {
            m_items[i]->drop();
        }
        m_items.resize(_size);
    }
}

void EditValueArray::_restore(const Items &items) {
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->drop();
	}
	m_items = items;
	for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		(*it)->grab();
	}
	_notify_changed();
}

////////////////////////////////////////////

EditValueObject::EditValueObject(const EditTypeObject *_type)
: EditValue(_type) {
    size_t c = _type->numAttributes();
    m_attributes.assign(c, 0);
    for (size_t i = 0; i < c; ++i) {
        m_attributes[i] = _type->getAttributeType(i)->generate();
    }
}

EditValueObject::EditValueObject(const EditValueObject &other)
: EditValue(other) {
    size_t c = other.m_attributes.size();
    m_attributes.assign(c, 0);
    for (size_t i = 0; i < c; ++i) {
        m_attributes[i] = other.m_attributes[i]->clone();
    }
}

EditValueObject * EditValueObject::clone() const {
    return vnnew EditValueObject(*this);
}

EditValueObject::~EditValueObject() {
    for (Attributes::iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
        (*it)->drop();
    }
}

str8 & EditValueObject::desc(str8 &s) const {
    c8 buf[32];
    return s.assign(buf, sprintf(buf, "{%ld}", EditValueObject::type()->numAttributes()));
}

const EditValue::BackupData * EditValueObject::backup() const {
	struct _BackupData : BackupData {
		_BackupData(const EditType *type) : BackupData(type) {}
		virtual bool restore(EditValue *value) const {
			if (value->type() != m_type) {
				return false;
			}
			return true;
		}
	};
	return vnnew _BackupData(m_type);
}

const EditTypeObject * EditValueObject::type() const {
    return (const EditTypeObject *)m_type;
}

EditValue * EditValueObject::attribute(size_t index) {
    return m_attributes[index];
}

const EditValue * EditValueObject::attribute(size_t index) const {
    return m_attributes[index];
}


_vn_end