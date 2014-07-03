//
//  TableReader.cpp
//  tblexport
//
//

#include "TableReader.h"

#if VN_PLATFORM == VN_PLATFORM_WIN
#include "win\TableReaderImpl.h"
#else

#endif

TableReader * TableReader::ms_instance = 0;

TableReader & TableReader::instance() {
	if (!ms_instance) {
		ms_instance = vnnew TableReaderImpl();
		::atexit(&_destroyInstance);
	}
	return *ms_instance;
}

TableReader::~TableReader() {

}

void TableReader::_destroyInstance() {
	VN_SAFE_DELETE(ms_instance);
}

const TableRecord & TableReader::record() {
	return m_record;
}


