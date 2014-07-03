//
//  vnFileManager.h
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFileManager_h
#define _vnFileManager_h

#include "vnSingleton.h"
#include "vnFileSystem.h"
#include "vnFilePath.h"
#include <map>
#include <list>

_vn_begin

class _vn_base_api FileManager : public Singleton<FileManager> {
	friend class Singleton<FileManager>;
public:
    ~FileManager();
    FileStream * open(const FilePath &path, bool readOnly = true);
    FileStream * create(const FilePath &path, bool writeOnly = true);
	bool remove(const FilePath &path);
	bool list(const FilePath &path, std::list<str8> &result);
	bool mkdir(const FilePath &path);

	FileSystem * getFileSystem(u32 fsId);
	void setFileSystem(u32 fsId, FileSystem *fs, bool grab = true);
	void listFileSystemId(std::list<u32> &result);
private:
	FileManager();
	
	typedef std::map<u32, FileSystem *> FileSystemMap;
	FileSystemMap m_systems;
};

template class _vn_base_api Singleton<FileManager>;

_vn_end

#endif