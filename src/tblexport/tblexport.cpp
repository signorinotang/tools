//
// tblexport.cpp
// tblexport
//

#include "vnFileManager.h"
#include "vnUnicode.h"
#include "vnLog.h"
#include "vnTreeDataScript.h"

#include "TableRecordDesc.h"
#include "TableRecord.h"
#include "TableReader.h"

#include <set>
#include <list>

using namespace vn;

bool writeFileWithKeys(const tbl::RecordDesc &desc);
bool writeFileWithoutKeys(const tbl::RecordDesc &desc);
void writeFileHeader(DataStream &stream, const tbl::RecordDesc &desc);

#if VN_PLATFORM == VN_PLATFORM_WIN
int wmain(int argc, const wchar_t *argv[]) {
#else 
int main(int argc, const char *argv[]) {
#endif

	Log::instance().addPrinter(vnnew LogPrinterSTDOUT(), false);

	FilePath scriptFileName;
	FilePath inputFileName;
	if (argc < 3) {
		VN_LOG_ERROR("Invalid Arguments.");
		VN_LOG_INFO("Usage: tblexport script-file input-file [output-path]");
		return -1;
	}

#if VN_PLATFORM == VN_PLATFORM_WIN
	Unicode::convert(argv[1], wcslen(argv[1]), scriptFileName.fileName);
	Unicode::convert(argv[2], wcslen(argv[2]), inputFileName.fileName);
#else
	scriptFileName.fileName = argv[1];
	inputFileName.fileName = argv[2];
#endif

	str8 outputPath;
	if (argc > 3) {
#if VN_PLATFORM == VN_PLATFORM_WIN
		Unicode::convert(argv[3], wcslen(argv[3]), outputPath);
#else
		outputPath = argv[2];
#endif
	}

	FileManager::instance().setFileSystem(1, vnnew FileSystemOS(outputPath), false);

	tbl::RecordDesc desc;

	TreeDataScript parser;
	TreeDataNode *node = parser.parse(scriptFileName);
	if (!node) {
		VN_LOG_ERROR("file \"" << scriptFileName.fileName << "\" parse failed.");
		VN_LOG_FATAL("Export failed!");
		return -1;
	}

	desc.init(node);
	node->drop();

	TableReader &r = TableReader::instance();
	if (!r.open(desc, inputFileName)) {
		VN_LOG_FATAL("Export failed!");
		return -1;
	}

	bool ok;
	if (r.record().hasKeys()) {
		ok = writeFileWithKeys(desc);
	} else {
		ok = writeFileWithoutKeys(desc);
	}

	r.close();

	if (!ok) {
		VN_LOG_FATAL("Export failed :-(");
		return -1;
	}
	VN_LOG_INFO("Exported :-)");
	return 0;
}

bool writeFileWithKeys(const tbl::RecordDesc &desc) {
	std::set<TableRecord> records;
	TableReader &r = TableReader::instance();
	for (u32 i = 1;r.fetch(); ++i) {
		if (!r.record().checkKeys()) {
			//VN_LOG_WARN("Invalid Record #" << i);
			continue;
		}
		if (!records.insert(r.record()).second) {
			VN_LOG_WARN("Duplicated Record #" << i);
		}
	}
	VN_LOG_INFO("Fetch " << (u32)records.size() << " record(s).");
	FileStream *fs = FileManager::instance().create(FilePath(1, desc.name + ".tbl"));
	if (!fs) {
		VN_LOG_ERROR("create export-file \"" << desc.name << ".tbl\" failed.");
		return false;
	}
	try {
		writeFileHeader(*fs, desc);
		*fs << DataStream::z_u32((u32)records.size());
		for (auto it = records.begin(); it != records.end(); ++it) {
			it->save(*fs);
		}
	} catch (DataStream::Exception &) {
		fs->close();
		VN_LOG_ERROR("write export-file \"" << desc.name << ".tbl\" failed.");
		return false;
	}
	fs->close();
	return true;
}

bool writeFileWithoutKeys(const tbl::RecordDesc &desc) {
	std::list<TableRecord> records;
	TableReader &r = TableReader::instance();
	while (r.fetch()) {
		records.push_back(r.record());
	}
	VN_LOG_INFO("Fetch " << (u32)records.size() << " record(s).");
	FileStream *fs = FileManager::instance().create(FilePath(1, desc.name + ".tbl"));
	if (!fs) {
		VN_LOG_ERROR("create export-file \"" << desc.name << ".tbl\" failed.");
		return false;
	}
	try {
		writeFileHeader(*fs, desc);
		*fs << DataStream::z_u32((u32)records.size());
		for (auto it = records.begin(); it != records.end(); ++it) {
			it->save(*fs);
		}
	} catch (DataStream::Exception &) {
		fs->close();
		VN_LOG_ERROR("write export-file \"" << desc.name << ".tbl\" failed.");
		return false;
	}
	fs->close();
	return true;
}

void writeFileHeader(DataStream &stream, const tbl::RecordDesc &desc) {
	stream << tbl::kTableFileHeader;
	stream << DataStream::z_u32((u32)desc.fields.size());
	for (auto it = desc.fields.begin(); it != desc.fields.end(); ++it) {
		stream << it->format.value;
	}
	stream << desc.version;
}