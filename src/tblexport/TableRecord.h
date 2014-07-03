//
//  TableRecord.h
//  tblexport
//
//

#ifndef tblexport_TableRecord_h
#define tblexport_TableRecord_h

#include "TableField.h"
#include "TableRecordDesc.h"

#include <vector>

class TableRecord : public AllocatedObject {
public:
	TableRecord();
	TableRecord(const TableRecord &other);
	~TableRecord();
	bool init(const tbl::RecordDesc &desc);
	void fini();

	size_t size() const;
	TableFieldBase * field(size_t index);
	bool hasKeys() const;
	bool checkKeys() const;

	bool operator <(const TableRecord &other) const;

	void save(DataStream &stream) const;

protected:

	typedef std::vector<TableFieldBase *> Fields;
	Fields m_fields, m_keys;

};


#endif