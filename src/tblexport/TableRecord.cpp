//
//  TableRecord.cpp
//  tblexport
//
//

#include "TableRecord.h"

TableRecord::TableRecord() {

}

TableRecord::TableRecord(const TableRecord &other)
: m_fields(other.m_fields) {
	for (Fields::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
		*it = (*it)->clone();
		if ((*it)->isKey()) {
			m_keys.push_back(*it);
		}
	}
}

TableRecord::~TableRecord() {
	for (Fields::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
		delete *it;
	}
}

bool TableRecord::init(const tbl::RecordDesc &desc) {
	fini();
	for (tbl::RecordDesc::Fields::const_iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
		TableFieldBase *field = 0;
		switch (it->format.type) {
			case tbl::kFT_String:
				field = vnnew TableField_string();
				break;

			case tbl::kFT_Int:
				field = vnnew TableField_int();
				break;

			case tbl::kFT_UInt:
				field = vnnew TableField_uint();
				break;

			case tbl::kFT_Float:
				field = vnnew TableField_float();
				break;

			default:
				return false;
		}
		m_fields.push_back(field);
		if (it->format.key) {
			field->setKey(true);
			m_keys.push_back(field);
		}
	}
	return !m_fields.empty();
}

void TableRecord::fini() {
	for (Fields::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
		delete *it;
	}
	m_fields.clear();
	m_keys.clear();
}

size_t TableRecord::size() const {
	return m_fields.size();
}

bool TableRecord::hasKeys() const {
	return !m_keys.empty();
}

bool TableRecord::checkKeys() const {
	for (Fields::const_iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		if ((*it)->isNull()) {
			return false;
		}
	}
	return true;
}

TableFieldBase * TableRecord::field(size_t index) {
	return m_fields[index];
}

bool TableRecord::operator <(const TableRecord &other) const {
	for (size_t i = 0; i < m_keys.size(); ++i) {
		int ret = m_keys[i]->compare(*other.m_keys[i]);
		if (!ret) {
			continue;
		}
		return ret < 0;
	}
	return false;
}

void TableRecord::save(DataStream &stream) const {
	for (Fields::const_iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
		(*it)->save(stream);
	}
}