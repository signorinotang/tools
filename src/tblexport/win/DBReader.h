#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

class DBReader {
public:

	DBReader();
	~DBReader();

	bool openExcelFile(const std::wstring &fileName);
	void close();

	bool tableList(std::list<std::wstring> &tables);

	bool execute(const wchar_t *sql );

	bool bindColumn(unsigned short index, std::string &field, bool *null = 0);
	bool bindColumn(unsigned short index, int &field, bool *null = 0);
	bool bindColumn(unsigned short index, unsigned int &field, bool *null = 0);
	bool bindColumn(unsigned short index, float &field, bool *null = 0);

	bool fetch();

	void toUTF8(const wchar_t *input, std::string &output);

private:

	struct Field {
		int type;
		unsigned char *buffer;
		void *outputValue;
		bool *outputNull;
		int null;
		Field() : type(0), buffer(0), null(0), outputValue(0), outputNull(0) {}
		~Field() { if ( buffer ) { delete []buffer; buffer = 0; } }
	};
	typedef std::map<unsigned short, Field> Fields;

	void *m_environment;
	void *m_connection;
	void *m_statement; 
	Fields m_fields;
};