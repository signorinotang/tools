//
//  TableReaderImpl.cpp
//  tblexport
//
//

#include "TableReaderImpl.h"
#include "vnUnicode.h"
#include "vnLog.h"

bool TableReaderImpl::open(const tbl::RecordDesc &desc, const FilePath &inputFile) {
	if (!m_record.init(desc)) {
		VN_LOG_ERROR("TableRecord::init failed.");
		return false;
	}
	
	strw fileName;
	Unicode::convert(inputFile.fileName.c_str(), inputFile.fileName.length(), fileName);
	if (!m_reader.openExcelFile(fileName)) {
		VN_LOG_ERROR("DBReader::openExcelFile \"" << inputFile.fileName << "\"failed.");
		return false;
	}

	std::ostringstream oss;
	tbl::RecordDesc::Fields::const_iterator it = desc.fields.begin();
	oss << "SELECT [" << it->name.c_str() << "]";
	for (++it; it != desc.fields.end(); ++it) {
		oss << ", [" << it->name.c_str() << "]";
	}
	oss << " FROM [" << desc.name.c_str() << "$]";

	strw buf;
	Unicode::convert(oss.str().c_str(), oss.str().length(), buf);
	if (!m_reader.execute(buf.c_str())) {
		VN_LOG_ERROR("DBReader::execute \"" << oss.str() << "\" failed.");
		return false;
	}

	return _bind();
}

bool TableReaderImpl::fetch() {
	return m_reader.fetch();
}

void TableReaderImpl::close() {
	m_reader.close();
}

template <class _FieldType>
bool _bind(DBReader &reader, size_t index, TableFieldBase *field) {
	_FieldType *p = static_cast<_FieldType *>(field);
	return reader.bindColumn((u16)index + 1, p->field.value, &p->field.null);
}

bool TableReaderImpl::_bind() {
	for (size_t i = 0, c = m_record.size(); i < c; ++i) {
		TableFieldBase *field = m_record.field(i);
		switch (field->type()) {
			case tbl::kFT_String:
				if (!::_bind<TableField_string>(m_reader, i, field)) {
					return false;
				}
				break;

			case tbl::kFT_Int:
				if (!::_bind<TableField_int>(m_reader, i, field)) {
					return false;
				}
				break;

			case tbl::kFT_UInt:
				if (!::_bind<TableField_uint>(m_reader, i, field)) {
					return false;
				}
				break;

			case tbl::kFT_Float:
				if (!::_bind<TableField_float>(m_reader, i, field)) {
					return false;
				}
				break;
		}
	}
	return true;
}