//
//  TableField.cpp
//  tblexport
//
//

#include "TableField.h"

TableFieldBase::TableFieldBase()
: m_key(false) {

}

TableFieldBase::~TableFieldBase() {

}

void TableFieldBase::setKey(bool key) {
	m_key = key;
}

bool TableFieldBase::isKey() const {
	return m_key;
}
