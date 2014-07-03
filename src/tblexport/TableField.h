//
//  TableField.h
//  tblexport
//
//

#ifndef tblexport_TableField_h
#define tblexport_TableField_h

#include "TableBase.h"

#include "vnAllocatedObject.h"

using namespace vn;

class TableFieldBase : public AllocatedObject {
public:
	TableFieldBase();
	virtual TableFieldBase * clone() const = 0;
	virtual ~TableFieldBase();
	virtual tbl::FieldType type() const = 0;
	virtual int compare(const TableFieldBase &other) const = 0;
	virtual bool isNull() const = 0;
	virtual void save(DataStream &stream) const = 0;
	void setKey(bool key);
	bool isKey() const;
protected:
	bool m_key;
};

template <class _Value>
class TableField : public TableFieldBase {
public:

	virtual TableField<_Value> * clone() const {
		return vnnew TableField<_Value>(*this);
	}

	virtual tbl::FieldType type() const {
		return (tbl::FieldType)_Value::type;
	}
	
	virtual int compare(const TableFieldBase &other) const {
		vnassert(_Value::type == other.type());
		return field.compare(static_cast<const TableField<_Value> &>(other).field);
	}

	virtual bool isNull() const {
		return field.null;
	}

	virtual void save(DataStream &stream) const {
		stream << field;
	}

	_Value field;
};

typedef TableField<tbl::Field_string> TableField_string;
typedef TableField<tbl::Field_int> TableField_int;
typedef TableField<tbl::Field_uint> TableField_uint;
typedef TableField<tbl::Field_float> TableField_float;

#endif
