//
//  vnFileManager.cpp
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFileManager.h"
#include "vnFileSystem.h"

_vn_begin

FileManager::FileManager() {
	m_systems[0] = vnnew FileSystemOS();
}

FileManager::~FileManager() {
	for (FileSystemMap::iterator it = m_systems.begin(); it != m_systems.end(); ++it) {
		it->second->drop();
	}
}

FileStream * FileManager::open(const vn::FilePath &path, bool readOnly) {
    FileSystemMap::iterator it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->open(path.fileName, readOnly);
	}
	return 0;
}

FileStream * FileManager::create(const vn::FilePath &path, bool writeOnly) {
    FileSystemMap::iterator it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->create(path.fileName, writeOnly);
	}
	return 0;
}

bool FileManager::remove(const FilePath &path) {
	FileSystemMap::iterator it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->remove(path.fileName);
	}
	return false;
}

bool FileManager::list(const FilePath &path, std::list<str8> &result) {
	FileSystemMap::iterator it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->list(path.fileName, result);
	}
	return false;
}

bool FileManager::mkdir(const FilePath &path) {
	FileSystemMap::iterator it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->mkdir(path.fileName);
	}
	return false;
}


FileSystem * FileManager::getFileSystem(u32 fsId) {
	FileSystemMap::iterator it = m_systems.find(fsId);
	if (it != m_systems.end()) {
		return it->second;
	}
	return 0;
}

void FileManager::setFileSystem(u32 fsId, FileSystem *fs, bool grab) {
	if (fs) {
		if (grab) {
			fs->grab();
		}
		std::pair<FileSystemMap::iterator, bool> ret = m_systems.insert(std::make_pair(fsId, fs));
		if (!ret.second) {
			ret.first->second->drop();
			ret.first->second = fs;
		}
	} else {
		FileSystemMap::iterator it = m_systems.find(fsId);
		if (it != m_systems.end()) {
			it->second->drop();
			m_systems.erase(it);
		}
	}
}

void FileManager::listFileSystemId(std::list<u32> &result) {
	result.clear();
	for (FileSystemMap::iterator it = m_systems.begin(); it != m_systems.end(); ++it) {
		result.push_back(it->first);
	}
}


_vn_end