//
//  tblcoder.cpp
//  tblcoder
//
//  Created by Wayne on 14-2-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFileManager.h"
#include "vnUnicode.h"
#include "vnLog.h"
#include "vnTreeDataScript.h"
#include "TableRecordDesc.h"
#include <sstream>

using namespace vn;

#if VN_PLATFORM == VN_PLATFORM_WIN
int wmain(int argc, const wchar_t *argv[]) {
#else
int main(int argc, const char *argv[]) {
#endif

	Log::instance().addPrinter(vnnew LogPrinterSTDOUT(), false);

	FilePath scriptFileName;
	if (argc < 2) {
		VN_LOG_ERROR("Invalid Arguments.");
		VN_LOG_INFO("Usage: tblcoder script-file [output-path]");
		return -1;
	}

#if VN_PLATFORM == VN_PLATFORM_WIN
	Unicode::convert(argv[1], wcslen(argv[1]), scriptFileName.fileName);
#else
	scriptFileName.fileName = argv[1];
#endif

	str8 outputPath;
	if (argc > 2) {
#if VN_PLATFORM == VN_PLATFORM_WIN
		Unicode::convert(argv[2], wcslen(argv[2]), outputPath);
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
		return -1;
	}

	desc.init(node);
	node->drop();

	/*FilePath path(1, str8("Info.tbl"));
	DataStream* stream = vn::FileManager::instance().open(path);
	if (!stream) {
	VN_LOG_ERROR("open failed!!");
	}

	tbl::Table_info m_test;
	m_test.init(*stream);
	size_t index = m_test.find(1001, 1);*/

	std::ostringstream fileHeader, fileCpp;
	//******************************.h*************************************
	fileHeader << "#ifndef __Table_" + desc.name + "_H__\n";
	fileHeader << "#define __Table_" + desc.name + "_H__\n\n";
	fileHeader << "#include \"TableBase.h\"\n";
	fileHeader << "#include \"vnFilePath.h\"\n";

	fileHeader << "namespace tbl {\n";
	fileHeader << "struct Record_" + desc.name + " : public AllocatedObject {\n";
	u32 key = 0;
	for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
		str8 format;
		if ((*it).format.key) {
			++key;
		}
		switch ((*it).format.type)
		{
		case 0:
			VN_LOG_ERROR("FieldType ERROR!");
			return -1;
		case 1:
			format = "Field_string";
			break;
		case 2:
			format = "Field_int";
			break;
		case 3:
			format = "Field_uint";
			break;
		case 4:
			format = "Field_float";
			break;
		}
		fileHeader << "\t" + format + " " + "f_" + (*it).name + ";\n";
	}

	if (key) {
		u32 tmpKey = key;
		fileHeader << "\tint compare(";
		for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
			str8 format;
			if ((*it).format.key) {
				switch ((*it).format.type)
				{
				case 0:
					VN_LOG_ERROR("FieldType ERROR!");
					return -1;
				case 1:
					format = "const str8& _" + (*it).name;
					break;
				case 2:
					format = "s32 _" + (*it).name;
					break;
				case 3:
					format = "u32 _" + (*it).name;
					break;
				case 4:
					format = "f32 _" + (*it).name;
					break;
				}
				fileHeader << format;
				if (--tmpKey) {
					fileHeader << ",";
				}
			}
		}
		fileHeader << ") const;\n";
	}
	fileHeader << "\tvoid load(DataStream &stream);\n";
	fileHeader << "};\n\n";
	fileHeader << "class Table_" + desc.name + "{\n";
	fileHeader << "public:\n";
	fileHeader << "\tTable_" + desc.name + "();\n";
	fileHeader << "\t~Table_" + desc.name + "();\n";
	fileHeader << "\n";
	fileHeader << "\tbool init(DataStream &stream);\n";
	fileHeader << "\tbool init(u32 fsId, const str8& path);\n";
	fileHeader << "\tvoid fini();\n";
	fileHeader << "\n";
	fileHeader << "\tu32 version() const;\n";
	fileHeader << "\tsize_t size() const;\n";
	fileHeader << "\tconst Record_" + desc.name + " * at(size_t index) const;\n";
	fileHeader << "\n";
	if (key) {
		u32 tmpKey = key;
		fileHeader << "\tsize_t find(";
		for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
			str8 format;
			if ((*it).format.key) {
				switch ((*it).format.type)
				{
				case 0:
					VN_LOG_ERROR("FieldType ERROR!");
					return -1;
				case 1:
					format = "const str8& _" + (*it).name;
					break;
				case 2:
					format = "s32 _" + (*it).name;
					break;
				case 3:
					format = "u32 _" + (*it).name;
					break;
				case 4:
					format = "f32 _" + (*it).name;
					break;
				}
				fileHeader << format;
				if (--tmpKey) {
					fileHeader << ",";
				}
			}
		}
		fileHeader << ");\n";
	}
	fileHeader << "private:\n";
	fileHeader << "\tu32 m_version;\n";
	fileHeader << "\tRecord_" + desc.name + " *m_records;\n";
	fileHeader << "\tsize_t m_size;\n";
	fileHeader << "};\n";
	fileHeader << "}\n";
	fileHeader << "#endif\n";
	//*******************************************************************************



	//************************************.cpp***************************************
	//#include "table_"*** to do
	fileCpp << "#include \"Table_" + desc.name + ".h\"\n";
	fileCpp << "#include \"vnFileManager.h\"\n\n";
	fileCpp << "namespace tbl {\n";
	//function int compare(const Record_Info &other) const;
	if (key) {
		u32 tmpKey = key;
        size_t numKeys = 0;
		fileCpp << "int Record_" + desc.name + "::compare(";
		for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
			str8 format;
			if ((*it).format.key) {
				++numKeys;
                switch ((*it).format.type)
				{
				case 0:
					VN_LOG_ERROR("FieldType ERROR!");
					return -1;
				case 1:
					format = "const str8& _" + (*it).name;
					break;
				case 2:
					format = "s32 _" + (*it).name;
					break;
				case 3:
					format = "u32 _" + (*it).name;
					break;
				case 4:
					format = "f32 _" + (*it).name;
					break;
				}
				fileCpp << format;
				if (--tmpKey) {
					fileCpp << ",";
				}
			}
		}
		fileCpp << ") const {\n";
		//compare
        if (numKeys == 1) {
            for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
                if ((*it).format.key) {
                    fileCpp << "\treturn f_" << it->name << ".compare(_" << it->name << ");\n";
                    break;
                }
            }
        } else {
            fileCpp << "\tint ret;\n";
            for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
                if ((*it).format.key) {
                    fileCpp << "\tret = f_" << it->name << ".compare(_" << it->name << ");\n";
                    fileCpp << "\tif (ret) {\n";
                    fileCpp << "\t\treturn ret;\n";
                    fileCpp << "\t}\n";
                }
            }
            fileCpp << "\treturn 0;\n";
        }
		fileCpp << "}\n\n";
	}
	//function void load(DataStream &stream);
	fileCpp << "void Record_" + desc.name + "::load(DataStream &stream) {\n";
	for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
		fileCpp << "\tstream >> f_" + ((*it).name) + ";\n";
	}
	fileCpp << "}\n\n";
	//constructor
	fileCpp << "Table_" + desc.name + "::Table_" + desc.name + "()\n";
	fileCpp << ":m_records(0)\n";
	fileCpp << ",m_version(0)\n";
	fileCpp << ",m_size(0) {\n";
	fileCpp << "}\n\n";
	//destructor
	fileCpp << "Table_" + desc.name + "::~Table_" + desc.name + "() {\n";
	fileCpp << "\tfini();\n";
	fileCpp << "}\n\n";
	//function bool init(FilePath path);
	fileCpp << "bool Table_" + desc.name + "::init(u32 fsId, const str8& path) {\n";
	fileCpp << "\tFilePath name(fsId, path);\n";
	fileCpp << "\tif(!name.fileName.empty() && name.fileName.back() != '/') {\n";
	fileCpp << "\t\tname.fileName += '/';\n";
	fileCpp << "\t}\n";
	fileCpp << "\tname.fileName += \"" + desc.name + ".tbl\";\n";
	fileCpp << "\tFileStream* fs = FileManager::instance().open(name);\n";
	fileCpp << "\tif(!fs) {\n";
	fileCpp << "\t\treturn false;\n";
	fileCpp << "\t}\n";
	fileCpp << "\tbool ret = false;\n";
	fileCpp << "\ttry {\n";
	fileCpp << "\t\tret = init(*fs);\n";
	fileCpp << "\t} catch (DataStream::Exception& ) {\n\n";
	fileCpp << "\t}\n";
	fileCpp << "\tfs->close();\n";
	fileCpp << "\treturn ret;\n";
	fileCpp << "}\n";
	//function bool init(DataStream &stream);
	fileCpp << "bool Table_" + desc.name + "::init(DataStream &stream) {\n";
	fileCpp << "\tu32 head;\n";
	fileCpp << "\tstream >> head;\n";
	fileCpp << "\tif(head != kTableFileHeader) {\n";
	fileCpp << "\t\treturn false;\n";
	fileCpp << "\t}\n";
	fileCpp << "\tRecordFormat format;\n";
	fileCpp << "\tstream >> format;\n";
	fileCpp << "\tif(format.size() != ";
	fileCpp << desc.fields.size();
	fileCpp << ") {\n";
	fileCpp << "\t\treturn false;\n";
	fileCpp << "\t}\n";
	for (u32 i = 0; i < desc.fields.size(); ++i) {

		fileCpp << "\tif(format[";
		fileCpp << i;
		fileCpp << "].value != ";
		fileCpp << (u32)desc.fields[i].format.value;
		fileCpp << ") {\n";
		fileCpp << "\t\treturn false;\n";
		fileCpp << "\t}\n";
	}
	fileCpp << "\tstream >> m_version;\n";
    fileCpp << "#ifdef VN_2D\n";
	fileCpp << "\tDataStream::z_u32 numRecords;\n";
	fileCpp << "\tstream >> numRecords;\n";
	fileCpp << "\tm_size = numRecords.value;\n";
    fileCpp << "#else\n";
	fileCpp << "\t_read_z_u32(stream, m_size);\n";
    fileCpp << "#endif\n";
	fileCpp << "\tif(m_size == 0) {\n";
	fileCpp << "\t\treturn true;\n";
	fileCpp << "\t}\n";
	fileCpp << "\tm_records = vnnew Record_" + desc.name + "[m_size];\n";
	fileCpp << "\tfor(size_t i = 0; i < m_size; ++i) {\n";
	fileCpp << "\t\tm_records[i].load(stream);\n";
	fileCpp << "\t}\n";
	fileCpp << "\treturn true;\n";
	fileCpp << "}\n\n";
	//function void fini();
	fileCpp << "void Table_" + desc.name + "::fini() {\n";
	fileCpp << "\tVN_SAFE_DELETE_ARRAY(m_records);\n";
	fileCpp << "\tm_size = 0;\n";
	fileCpp << "}\n\n";
	//function u32 version() const;
	fileCpp << "u32 Table_" + desc.name + "::version() const {\n";
	fileCpp << "\treturn m_version;\n";
	fileCpp << "}\n\n";
	//function size_t size() const;
	fileCpp << "size_t Table_" + desc.name + "::size() const {\n";
	fileCpp << "\treturn m_size;\n";
	fileCpp << "}\n\n";
	//function const Record_Info * at(size_t index) const;
	fileCpp << "const Record_" + desc.name + " * Table_" + desc.name + "::at(size_t index) const{\n";
	fileCpp << "\tif(index < m_size) {\n";
	fileCpp << "\t\treturn m_records + index;\n";
	fileCpp << "\t}\n";
	fileCpp << "\treturn 0;\n";
	fileCpp << "}\n\n";
	//size_t find(u32 _id);
	if (key) {
		u32 tmpKey = key;
		fileCpp << "size_t Table_" + desc.name + "::find(";
		for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
			str8 format;
			if ((*it).format.key) {
				switch ((*it).format.type)
				{
				case 0:
					VN_LOG_ERROR("FieldType ERROR!");
					return -1;
				case 1:
					format = "const str8& _" + (*it).name;
					break;
				case 2:
					format = "s32 _" + (*it).name;
					break;
				case 3:
					format = "u32 _" + (*it).name;
					break;
				case 4:
					format = "f32 _" + (*it).name;
					break;
				}
				fileCpp << format;
				if (--tmpKey) {
					fileCpp << ",";
				}
			}
		}
		fileCpp << ") {\n";
		fileCpp << "\tssize_t low = 0, high = (ssize_t)m_size - 1, mid;\n";
		fileCpp << "\twhile(low <= high) {\n";
		fileCpp << "\t\tmid = (high + low)/2;\n";

		fileCpp << "\t\tint ret = m_records[mid].compare(";
		tmpKey = key;
		for (tbl::RecordDesc::Fields::iterator it = desc.fields.begin(); it != desc.fields.end(); ++it) {
			if ((*it).format.key) {
				fileCpp << "_" + ((*it).name);
				if (--tmpKey) {
					fileCpp << ",";
				}
			}
		}
		fileCpp << ");\n";
		fileCpp << "\t\tif(ret > 0) {\n";
		fileCpp << "\t\t\thigh = mid - 1;\n";
		fileCpp << "\t\t\tcontinue;\n";
		fileCpp << "\t\t}\n";
		fileCpp << "\t\tif (ret < 0) {\n";
		fileCpp << "\t\t\tlow = mid + 1;\n";
		fileCpp << "\t\t\tcontinue;\n";
		fileCpp << "\t\t}\n";
		fileCpp << "\t\treturn mid;\n";
		fileCpp << "\t}\n";
		fileCpp << "\treturn -1;\n";
		fileCpp << "}\n";
	}
	fileCpp << "}\n";
	//write file
	FilePath headFileName(1, str8("Table_") + desc.name + ".h");
	FilePath cppFileName(1, str8("Table_") + desc.name + ".cpp");

	FileStream* header = FileManager::instance().open(headFileName);
	FileStream* cpp = FileManager::instance().open(cppFileName);

	bool dirty = false;
	if (header) {
		size_t size = (size_t)header->size();
		//str8 original;
		//original.resize(size);
		//if (size) {
		//	header->read(&original[0], size);
		//}

		//if (original != fileHeader.str()) {
		//	dirty = true;
		//}
		c8 *original = vnmalloc(c8, size + 1);
		header->read(original, size);
		original[size] = 0;
		if (fileHeader.str() != original) {
			dirty = true;
		}
		vnfree(original);
		header->close();
	} else {
		dirty = true;
	}
	if (dirty) {
		header = FileManager::instance().create(headFileName);
		if (!header) {
			VN_LOG_ERROR("FILE.H CREATE FAILED!");
		}
		else {
			header->write(fileHeader.str().c_str(), fileHeader.str().length());
			header->close();
		}
	}
	dirty = false;
	if (cpp) {
		size_t size = (size_t)cpp->size();
		c8* original = vnmalloc(c8, size + 1);
		cpp->read(original, size);
		original[size] = 0;
		if (fileCpp.str() != original) {
			dirty = true;
		}
		vnfree(original);
		cpp->close();
	}	else {
		dirty = true;
	}
	if (dirty) {
		cpp = FileManager::instance().create(cppFileName);
		if (!cpp) {
			VN_LOG_ERROR("FILE.CPP CREATE FAILED!");
		} else {
			cpp->write(fileCpp.str().c_str(), fileCpp.str().length());
			cpp->close();
		}
	}	
	return 0;
}



