//
//  vnBuffer.h
//  vnbase
//
//  Created by Wayne on 13-6-14.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnBuffer_h
#define _vnBuffer_h

#include "vnRefCounted.h"
#include "vnDataStream.h"

_vn_begin

class _vn_base_api Buffer : virtual public RefCounted {
public:
	virtual void * lock() = 0;
	virtual void unlock() = 0;
	virtual size_t size() = 0;
	virtual size_t write(size_t offset, const void *data, size_t size) = 0;
};

class _vn_base_api BufferStream : public DataStream {
public:
	BufferStream(Buffer *buffer, bool grab = true);
	~BufferStream();

	Buffer * buffer() const;

	virtual size_t read(void *data, size_t size);
	virtual size_t write(const void *data, size_t size);
	virtual bool seek(s64 offset, SeekDir way);
	virtual s64 tell();
private:
	Buffer *m_buffer;
	size_t m_pos;
};

class _vn_base_api FixedBuffer : public Buffer {
public:
	FixedBuffer(size_t size);
	virtual ~FixedBuffer();
	virtual void * lock();
	virtual void unlock();
	virtual size_t size();
	virtual size_t write(size_t offset, const void *data, size_t size);
private:
	void *m_data;
	size_t m_size;
};

class _vn_base_api ProxyBuffer : public Buffer {
public:
	ProxyBuffer(void *data, size_t size);
	virtual void * lock();
	virtual void unlock();
	virtual size_t size();
	virtual size_t write(size_t offset, const void *data, size_t size);
private:
	void *m_data;
	size_t m_size;
};

_vn_end

#endif