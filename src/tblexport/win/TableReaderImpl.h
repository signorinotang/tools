#pragma once

#include "..\TableReader.h"
#include "DBReader.h"

class TableReaderImpl : public TableReader {
public:

	virtual bool open(const tbl::RecordDesc &desc, const FilePath &inputFile);
	virtual bool fetch();
	virtual void close();

private:
	DBReader m_reader;

	bool _bind();
};