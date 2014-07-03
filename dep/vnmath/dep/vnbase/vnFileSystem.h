//
//  vnFileSystem.h
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFileSystem_h
#define _vnFileSystem_h

#include "vnRefCounted.h"
#include "vnDataStream.h"
#include <list>

_vn_begin

class FileStream : public DataStream, public AllocatedObject {
public:
	virtual s64 size() = 0;
	virtual void close() = 0;
};

class _vn_base_api FileSystem : virtual public RefCounted {
public:
	virtual FileStream * open(const str8 &file, bool readOnly = true) = 0;
	virtual FileStream * create(const str8 &file, bool writeOnly = true) = 0;
	virtual bool remove(const str8 &path) = 0;
	virtual bool list(const str8 &path, std::list<str8> &result) = 0;
	virtual bool mkdir(const str8 &path) = 0;
};

class _vn_base_api FileSystemOS : public FileSystem {
public:
	FileSystemOS(const c8 *path = 0);
	FileSystemOS(const str8 &path);
	virtual FileStream * open(const str8 &file, bool readOnly = true);
	virtual FileStream * create(const str8 &file, bool writeOnly = true);
	virtual bool remove(const str8 &path);
	virtual bool list(const str8 &path, std::list<str8> &result);
	virtual bool mkdir(const str8 &path);
protected:
	str8 m_path;
};

_vn_end

#endif