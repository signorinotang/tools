//
//	TableReader.h
//	tblexport
//
//

#ifndef tblexport_TableReader_h
#define tblexport_TableReader_h

#include "TableRecord.h"

#include "vnFilePath.h"

class TableReader : public AllocatedObject {
public:
	static TableReader & instance();

	virtual ~TableReader();
	virtual bool open(const tbl::RecordDesc &desc, const FilePath &inputFile) = 0;
	virtual bool fetch() = 0;
	virtual void close() = 0;

	const TableRecord & record();

protected:
	TableRecord m_record;
private:
	static TableReader *ms_instance;

	static void _destroyInstance();
};

#endif