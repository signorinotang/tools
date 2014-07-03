//
//  vnBuffer.cpp
//  vnbase
//
//  Created by Wayne on 13-6-14.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnBuffer.h"
#include "vnMemory.h"

_vn_begin

BufferStream::BufferStream(Buffer *buffer, bool grab)
: m_buffer(buffer)
, m_pos(0) {
	vnassert(buffer);
	if (grab) {
		m_buffer->grab();
	}
}

BufferStream::~BufferStream() {
	m_buffer->drop();
}

Buffer * BufferStream::buffer() const {
	return m_buffer;
}

size_t BufferStream::read(void *data, size_t size) {
	void *ptr = m_buffer->lock();
	if (!ptr) {
		return 0;
	}
	size_t ret = m_buffer->size() - m_pos;
	if (size < ret) {
		ret = size;
	}
	if (ret) {
		memcpy(data, (u8 *)ptr + m_pos, ret);
		m_pos += ret;
	}
	m_buffer->lock();
	return ret;
}

size_t BufferStream::write(const void *data, size_t size) {
	size_t ret = m_buffer->write(m_pos, data, size);
	m_pos += ret;
	return ret;
}

bool BufferStream::seek(s64 offset, SeekDir way) {
	s64 pos;
	switch (way) {
		case kBegin:
			pos = offset;
			break;
		case kCurrent:
			pos = m_pos + offset;
			break;
		case kEnd:
			pos = m_buffer->size() + offset;
			break;
	}
	if (pos < 0 || pos > m_buffer->size()) {
		return false;
	}
	m_pos = (size_t)pos;
	return true;
}

s64 BufferStream::tell() {
	return m_pos;
}

///////////////////////////////////////////////////////

FixedBuffer::FixedBuffer(size_t size)
: m_data(0)
, m_size(size) {
	vnassert(size);
	m_data = vnmalloc(u8, size);
}

FixedBuffer::~FixedBuffer() {
	vnfree(m_data);
	m_data = 0;
}

void * FixedBuffer::lock() {
	return m_data;
}

void FixedBuffer::unlock() {
	
}

size_t FixedBuffer::size() {
	return m_size;
}

size_t FixedBuffer::write(size_t offset, const void *data, size_t size) {
	if (offset >= m_size || !size) {
		return 0;
	}
	size_t ret = m_size - offset;
	if (size < ret) {
		ret = size;
	}
	memcpy((u8 *)m_data + offset, data, ret);
	return ret;
}

ProxyBuffer::ProxyBuffer(void *data, size_t size)
: m_data(data)
, m_size(size) {
	vnassert(data && size);
}

void * ProxyBuffer::lock() {
	return m_data;
}

void ProxyBuffer::unlock() {
	
}

size_t ProxyBuffer::size() {
	return m_size;
}

size_t ProxyBuffer::write(size_t offset, const void *data, size_t size) {
	if (offset >= m_size || !size) {
		return 0;
	}
	size_t ret = m_size - offset;
	if (size < ret) {
		ret = size;
	}
	memcpy((u8 *)m_data + offset, data, ret);
	return ret;
}

_vn_end