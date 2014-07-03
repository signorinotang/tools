//
//  vnFileSystem.cpp
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFileSystem.h"
#include "vnDataStream.h"
#include "vnUnicode.h"

#include <cstdio>
#include <Windows.h>

_vn_begin

namespace internal {
	class FileStreamOS : public FileStream {
	public:
		FileStreamOS(FILE *fp) : m_fp(fp) {}

		virtual ~FileStreamOS() {
			if (m_fp) {
				fclose(m_fp);
				m_fp = 0;
			}
		}

		virtual s64 size() {
			if (!m_fp) {
				return 0;
			}
			s64 pos = _ftelli64(m_fp);
			_fseeki64(m_fp, 0, SEEK_END);
			s64 ret = _ftelli64(m_fp);
			_fseeki64(m_fp, pos, SEEK_SET);
			return ret;
		}

		virtual void close() {
			delete this;
		}

		virtual size_t read(void *buffer, size_t size) {
			return fread(buffer, 1, size, m_fp);
		}
		virtual size_t write(const void *buffer, size_t size) {
			return fwrite(buffer, 1, size, m_fp);
		}

		virtual bool seek(s64 offset, SeekDir way) {
			int whence;
			switch (way) {
			case kBegin:
				whence = SEEK_SET;
				break;
			case kCurrent:
				whence = SEEK_CUR;
				break;
			case kEnd:
				whence = SEEK_END;
				break;
			}
			return _fseeki64(m_fp, offset, whence) == 0;
		}

		virtual s64 tell() {
			return _ftelli64(m_fp);
		}
	private:
		FILE *m_fp;
	};
}

FileSystemOS::FileSystemOS(const c8 *path) {
	if (path) {
		m_path = path;
		if (!m_path.empty() && *m_path.rbegin() != '/') {
			m_path += '/';
		}
	}
}

FileSystemOS::FileSystemOS(const str8 &path)
: m_path(path){
	if (!m_path.empty() && *m_path.rbegin() != '/') {
		m_path += '/';
	}
}

FileStream * FileSystemOS::open(const str8 &file, bool readOnly) {
	const wchar_t *mode = (readOnly ? L"rb" : L"r+b");
	FILE *fp = 0;
	strw name;
	if (m_path.empty()) {
		Unicode::convert(file.c_str(), file.length(), name);
	} else {
		str8 full = m_path + file;
		Unicode::convert(full.c_str(), full.length(), name);
	}
	if (_wfopen_s(&fp, name.c_str(), mode)) {
		return 0;
	}
	return vnnew internal::FileStreamOS(fp);
}

FileStream * FileSystemOS::create(const str8 &file, bool writeOnly) {
	const wchar_t *mode = (writeOnly ? L"wb" : L"w+b");
	FILE *fp = 0;
	strw name;
	if (m_path.empty()) {
		Unicode::convert(file.c_str(), file.length(), name);
	}
	else {
		str8 full = m_path + file;
		Unicode::convert(full.c_str(), full.length(), name);
	}
	if (_wfopen_s(&fp, name.c_str(), mode)) {
		return 0;
	}
	return vnnew internal::FileStreamOS(fp);
}

bool FileSystemOS::remove(const str8 &path) {
	strw name;
	if (m_path.empty()) {
		Unicode::convert(path.c_str(), path.length(), name);
	} else {
		str8 full = m_path + path;
		Unicode::convert(full.c_str(), full.length(), name);
	}
	return ::_wremove(name.c_str()) == 0;
}

bool FileSystemOS::mkdir(const str8 &path) {
	strw name;
	if (m_path.empty()) {
		Unicode::convert(path.c_str(), path.length(), name);
		return ::CreateDirectoryW(name.c_str(), NULL) != FALSE;
	}
	str8 full = m_path + path;
	Unicode::convert(full.c_str(), full.length(), name);
	return ::CreateDirectoryW(name.c_str(), NULL) != FALSE;
}

bool FileSystemOS::list(const str8 &path, std::list<str8> &result) {
	strw name;
	if (m_path.empty()) {
		Unicode::convert(path.c_str(), path.length(), name);
	} else {
		str8 full = m_path + path;
		Unicode::convert(full.c_str(), full.length(), name);
	}
	WIN32_FIND_DATAW fd;
	if (!name.empty()) {
		if (name.back() != '/' || name.back() != '//') {
			name.push_back('/');
		}
	}
	name.push_back('*');
	::HANDLE hFind = ::FindFirstFileW(name.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..")) {
				name = fd.cFileName;
				result.push_back(str8());
				str8 &fn = result.back();
				Unicode::convert(name.c_str(), name.length(), fn);
				fn.push_back('/');
			}
		} else {
			name = fd.cFileName;
			result.push_back(str8());
			Unicode::convert(name.c_str(), name.length(), result.back());
		}
	} while (::FindNextFileW(hFind, &fd) != FALSE);
	return true;
}

_vn_end