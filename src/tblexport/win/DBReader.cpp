#include "DBReader.h"
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sstream>

bool pathIsRelative( const wchar_t *path ) {
	if ( *path == '\\' || *path == '/' ) {
		return false;
	}
	if ( *path && path[1] == ':' ) {
		return false;
	}
	return true;
}

DBReader::DBReader()
: m_environment(0)
, m_connection(0)
, m_statement(0) {
	if (SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_environment ) == SQL_ERROR) {
		m_environment = 0;
		return ;
	}
	RETCODE ret = SQLSetEnvAttr(m_environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (ret != SQL_SUCCESS) {
		SQLFreeEnv(m_environment);
		m_environment = 0;
	}
}

DBReader::~DBReader() {
	close();
	if (m_environment) {
		SQLFreeEnv(m_environment);
		m_environment = 0;
	}
}

bool DBReader::openExcelFile(const std::wstring &fileName) {
	close();
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_environment, &m_connection ) != SQL_SUCCESS) {
		m_connection = 0;
		return false;
	}
	std::wstring filePath;
	if (pathIsRelative(fileName.c_str())) {
		wchar_t sz[MAX_PATH];
		DWORD ret = ::GetCurrentDirectoryW( MAX_PATH, sz );
		filePath.reserve(ret + fileName.length() + 1);
		filePath = sz;
		filePath.push_back( L'/' );
		filePath.append(fileName);
	} else {
		filePath = fileName;
	}
	
	std::wostringstream oss;

	oss << L"DRIVER=Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb);DBQ=" << filePath << ";READONLY=1;MAXSCANROWS=0";

	RETCODE ret = SQLDriverConnectW(m_connection, 0, (wchar_t *)oss.str().c_str(), (SQLSMALLINT)oss.str().length(), 0, 0, 0, SQL_DRIVER_COMPLETE); 
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLFreeConnect( m_connection );
		m_connection = 0;
		return false;
	}
	return true;
}

void DBReader::close() {
	if (m_statement) {
		SQLFreeStmt(m_statement, SQL_DROP);
		m_statement = 0;
	}
	m_fields.clear();

	if (m_connection) {
		SQLDisconnect(m_connection);
		SQLFreeConnect(m_connection);
		m_connection = 0;
	}
}

bool DBReader::tableList(std::list<std::wstring> &tables) {
	RETCODE ret;
	if (m_statement) {
		SQLFreeStmt(m_statement, SQL_CLOSE);
	} else {
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_connection, &m_statement);
		if (ret != SQL_SUCCESS) {
			m_statement = 0;
			return false;
		}
	}
	ret = SQLTablesW(m_statement, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	if ( ret != SQL_SUCCESS ) {
		return false;
	}
	wchar_t sz[256];
	ret = SQLBindCol(m_statement, 3, SQL_C_WCHAR, sz, sizeof(sz), 0);
	if (ret != SQL_SUCCESS) {
		return false;
	}
	tables.clear();
	while ( SQLFetch(m_statement) == SQL_SUCCESS ) {
		tables.push_back(sz);
	}
	return true;
}

bool DBReader::execute(const wchar_t *sql) {
	RETCODE ret;
	if (m_statement) {
		SQLFreeStmt(m_statement, SQL_CLOSE);
	} else {
		ret = SQLAllocHandle(SQL_HANDLE_STMT, m_connection, &m_statement);
		if (ret != SQL_SUCCESS) {
			m_statement = 0;
			return false;
		}
	}
	ret = SQLExecDirectW(m_statement, (wchar_t *)sql, SQL_NTS);
	if ( ret != SQL_SUCCESS ) {
		return false;
	}
	return true;
}

bool DBReader::bindColumn(unsigned short index, std::string &field, bool *null) {
	if (!index) {
		return false;
	}
	const int kBufferSize = 20480;
	Field &item = m_fields[index];
	if ( item.buffer ) {
		delete []item.buffer;
	}
	item.type = SQL_C_WCHAR;
	item.buffer = new unsigned char[kBufferSize];
	item.outputValue = &field;
	item.outputNull = null;
	RETCODE ret = SQLBindCol(m_statement, index, (SQLSMALLINT)item.type, item.buffer, kBufferSize, (SQLINTEGER *)&item.null);
	if (ret != SQL_SUCCESS) {
		m_fields.erase( index );
		return false;
	}
	return true;
}

bool DBReader::bindColumn(unsigned short index, int &field, bool *null) {
	if (!index) {
		return false;
	}
	const int kBufferSize = sizeof(int);
	Field &item = m_fields[index];
	if (item.buffer) {
		delete[]item.buffer;
	}
	item.type = SQL_C_SLONG;
	item.buffer = new unsigned char[kBufferSize];
	item.outputValue = &field;
	item.outputNull = null;
	RETCODE ret = SQLBindCol(m_statement, index, (SQLSMALLINT)item.type, item.buffer, kBufferSize, (SQLINTEGER *)&item.null);
	if (ret != SQL_SUCCESS) {
		m_fields.erase(index);
		return false;
	}
	return true;
}

bool DBReader::bindColumn(unsigned short index, unsigned int &field, bool *null) {
	if ( !index ) {
		return false;
	}
	const int kBufferSize = sizeof(unsigned int);
	Field &item = m_fields[index];
	if ( item.buffer ) {
		delete []item.buffer;
	}
	item.type = SQL_C_ULONG;
	item.buffer = new unsigned char[kBufferSize];
	item.outputValue = &field;
	item.outputNull = null;
	RETCODE ret = SQLBindCol( m_statement, index, (SQLSMALLINT)item.type, item.buffer, kBufferSize, (SQLINTEGER *)&item.null );
	if ( ret != SQL_SUCCESS ) {
		m_fields.erase( index );
		return false;
	}
	return true;
}

bool DBReader::bindColumn(unsigned short index, float &field, bool *null) {
	if (!index) {
		return false;
	}
	const int kBufferSize = sizeof(float);
	Field &item = m_fields[index];
	if (item.buffer) {
		delete []item.buffer;
	}
	item.type = SQL_C_FLOAT;
	item.buffer = new unsigned char[kBufferSize];
	item.outputValue = &field;
	item.outputNull = null;
	RETCODE ret = SQLBindCol(m_statement, index, (SQLSMALLINT)item.type, item.buffer, kBufferSize, (SQLINTEGER *)&item.null);
	if (ret != SQL_SUCCESS) {
		m_fields.erase(index);
		return false;
	}
	return true;
}

bool DBReader::fetch() {
	if (SQLFetch( m_statement ) != SQL_SUCCESS) {
		return false;
	}
	for (Fields::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
		Field &item = it->second;
		switch (item.type) {
		case SQL_C_WCHAR:
			if ( item.null == SQL_NULL_DATA ) {
				if ( item.outputNull ) {
					*item.outputNull = true;
				}
				std::string *str = (std::string *)item.outputValue;
				str->clear();
			} else {
				if (item.outputNull) {
					*item.outputNull = false;
				}
				std::string *str = (std::string *)item.outputValue;
				toUTF8((const wchar_t *)item.buffer, *str);
			}
			break;
		case SQL_C_SLONG:
			if (item.null == SQL_NULL_DATA) {
				if (item.outputNull) {
					*item.outputNull = true;
				}
				int *val = (int *)item.outputValue;
				*val = 0;
			} else {
				if (item.outputNull) {
					*item.outputNull = false;
				}
				int *val = (int *)item.outputValue;
				*val = *(int *)item.buffer;
			}
			break;
		case SQL_C_ULONG:
			if ( item.null == SQL_NULL_DATA ) {
				if ( item.outputNull ) {
					*item.outputNull = true;
				}
				unsigned int *val = (unsigned int *)item.outputValue;
				*val = 0;
			} else {
				if ( item.outputNull ) {
					*item.outputNull = false;
				}
				unsigned int *val = (unsigned int *)item.outputValue;
				*val = *(unsigned int *)item.buffer;
			}
			break;
		case SQL_C_FLOAT:
			if ( item.null == SQL_NULL_DATA ) {
				if ( item.outputNull ) {
					*item.outputNull = true;
				}
				float *val = (float *)item.outputValue;
				*val = 0;
			} else {
				if ( item.outputNull ) {
					*item.outputNull = false;
				}
				float *val = (float *)item.outputValue;
				*val = *(float *)item.buffer;
			}
			break;
		}
	}

	return true;
}

void DBReader::toUTF8(const wchar_t *input, std::string &output) {
	size_t len = wcslen(input);
	DWORD ret = ::WideCharToMultiByte(CP_UTF8, 0, input, len, 0, 0, NULL, NULL);
	output.resize(ret);
	if (ret) {
		::WideCharToMultiByte(CP_UTF8, 0, input, len, &output[0], ret, NULL, NULL);
	}
}