//
//  vnEditValue.h
//  vnbase
//
//  Created by Wayne on 14-1-3.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnbase_vnEditValue_h
#define vnbase_vnEditValue_h

#include "vnRefCounted.h"
#include "vnFunction.h"

#include <vector>

_vn_begin

class EditType;
class _vn_base_api EditValue : public RefCounted {
    friend class EditType;
public:
	class _vn_base_api BackupData : public RefCounted {
	public:
		BackupData(const EditType *type);
		virtual ~BackupData();
		const EditType * type() const;
		virtual bool restore(EditValue *value) const = 0;
	protected:
		const EditType *m_type;
	};
	
	class _vn_base_api Observer {
	public:
		virtual void ev_destroyed(EditValue *) {}
		virtual void ev_changed(EditValue *) {}
		virtual void ev_changed_backup(EditValue *, const BackupData *) {}
	};
	
    EditValue(const EditValue &other);
    virtual EditValue * clone() const = 0;
    virtual ~EditValue();
    virtual str8 & desc(str8 &s) const = 0;
	virtual const BackupData * backup() const = 0;
    
    const EditType * type() const;
    str8 desc() const;
    bool verity() const;
	
	void addObserver(Observer *observer);
	void removeObserver(Observer *observer);
	
protected:
    EditValue(const EditType *_type);
	void _notify_changed();
	void _notify_changed_backup(const BackupData *);
    const EditType *m_type;
	typedef std::vector<Observer *> Observers;
	Observers m_observers;
};

class _vn_base_api EditValueObserverProxy : public EditValue::Observer, public AllocatedObject {
public:
	EditValueObserverProxy();
	~EditValueObserverProxy();
	
	void setTarget(EditValue *target);
	EditValue * getTarget() const;
	
	void setFunction_Destroyed(const function<void(EditValue *)> &func);
	void setFunction_Changed(const function<void(EditValue *)> &func);
	void setFunction_ChangedBackup(const function<void(EditValue *, const EditValue::BackupData *)> &func);
	
	virtual void ev_destroyed(EditValue *);
	virtual void ev_changed(EditValue *);
	virtual void ev_changed_backup(EditValue *, const EditValue::BackupData *);
	
private:
	EditValue *m_target;
	function<void(EditValue *)> m_fnDestroyed;
	function<void(EditValue *)> m_fnChanged;
	function<void(EditValue *, const EditValue::BackupData *)> m_fnChangedBackup;
};

class EditTypeBool;
class _vn_base_api EditValueBool : public EditValue {
    friend class EditTypeBool;
public:
    virtual EditValueBool * clone() const;
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeBool * type() const;
    bool getValue() const;
	void setValue(bool value, bool _backup = false);
protected:
    EditValueBool(const EditTypeBool *_type);
    bool m_value;
};

class EditTypeInt;
class EditTypeRangedInt;
class EditTypeEnumeration;
class _vn_base_api EditValueInt : public EditValue {
    friend class EditTypeInt;
    friend class EditTypeRangedInt;
    friend class EditTypeEnumeration;
public:
    virtual EditValueInt * clone() const;
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeInt * type() const;
    void setValue(s32 value, bool _backup = false);
    s32 getValue() const;
protected:
    EditValueInt(const EditTypeInt *_type);
    s32 m_value;
};

class EditTypeFloat;
class EditTypeRangedFloat;
class _vn_base_api EditValueFloat : public EditValue {
    friend class EditTypeFloat;
    friend class EditTypeRangedFloat;
public:
    virtual EditValueFloat * clone() const;
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeFloat * type() const;
    void setValue(f32 value, bool _backup = false);
    f32 getValue() const;
protected:
    EditValueFloat(const EditTypeFloat *_type);
    f32 m_value;
};

class EditTypeString;
class _vn_base_api EditValueString : public EditValue {
    friend class EditTypeString;
public:
    virtual EditValueString * clone() const;
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeString * type() const;
    void setValue(const str8 &value, bool _backup = false);
    const str8 & getValue() const;
protected:
    EditValueString(const EditTypeString *_type);
    str8 m_value;
};

class EditTypeArray;
class _vn_base_api EditValueArray : public EditValue {
    friend class EditTypeArray;
public:
    EditValueArray(const EditValueArray &other);
    virtual EditValueArray * clone() const;
    virtual ~EditValueArray();
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeArray * type() const;
    
    size_t size() const;
    EditValue * at(size_t index);
    const EditValue * at(size_t index) const;
    
    EditValue * back();
    const EditValue * back() const;
    
    EditValue * front();
    const EditValue * front() const;
    
    EditValue * push_back(bool _backup = false);
    EditValue * insert(size_t position, bool _backup = false);
    void erase(size_t position, bool _backup = false);
    void clear(bool _backup = false);
    
    void assign(size_t _size, bool _backup = false);
    void resize(size_t _size, bool _backup = false);
protected:
    EditValueArray(const EditTypeArray *_type);
    
    typedef std::vector<EditValue *> Items;
    Items m_items;
	
	void _restore(const Items &items);

private:
	void _assign(size_t _size);
	void _resize(size_t _size);
};

class EditTypeObject;
class _vn_base_api EditValueObject : public EditValue {
    friend class EditTypeObject;
public:
    EditValueObject(const EditValueObject &other);
    virtual EditValueObject * clone() const;
    virtual ~EditValueObject();
    virtual str8 & desc(str8 &s) const;
	virtual const BackupData * backup() const;
    const EditTypeObject * type() const;
    
    EditValue * attribute(size_t index);
    const EditValue * attribute(size_t index) const;
    
protected:
    EditValueObject(const EditTypeObject *_type);
    
    typedef std::vector<EditValue *> Attributes;
    Attributes m_attributes;
};

_vn_end


#endif
