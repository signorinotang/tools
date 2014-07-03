//
//  vnFixedAllocator.h
//  vnbase
//
//  Created by Wayne on 13-6-17.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFixedAllocator_h
#define _vnFixedAllocator_h

#include "vnScopedLock.h"
#include "vnFastMutex.h"
#include "vnAllocatedObject.h"

_vn_begin

template <typename T>
class FixedAllocator {
public:
	FixedAllocator() : m_size(1), m_capability(1), m_lastAllocIdx(0), m_lastFreeIdx(0) {
		m_chunks = vnnew Chunk[1];
		m_chunks->init();
	}
	~FixedAllocator() {
		for (int i = 0; i < m_size; ++i) {
			m_chunks[i].fini();
		}
		delete []m_chunks;
	}
	
	
	T * allocate() {
		return new(_alloc()) T();
	}
	
	T * allocate(const T &t) {
		return new(_alloc()) T(t);
	}
	
	T * allocate_not_construct() {
		return (T *)_alloc();
	}
	
	void deallocate(T *ptr) {
		ptr->~T();
		_free(ptr);
	}
	
	void deallocate_not_destruct(T *ptr) {
		_free(ptr);
	}
protected:
	struct Chunk : public AllocatedObject {
		void init() {
			m_availableCount = 0xFF;
			m_buf = vnmalloc(u8, sizeof(T) * 0xFF);
			m_firstAvailable = 0;
			u8 *p = m_buf;
			for (u8 i = 1; i < 0xFF; ++i) {
				*p = i;
				p += sizeof(T);
			}
		}
		void fini() {
			vnassert(empty());
			vnfree(m_buf);
		}
		
		void * alloc() {
			u8 *ptr = m_buf + sizeof(T) * m_firstAvailable;
			m_firstAvailable = *ptr;
			--m_availableCount;
			return ptr;
		}
		
		void free(void *ptr) {
			*(u8 *)ptr = m_firstAvailable;
			m_firstAvailable = (u8)(((u8 *)ptr - m_buf) / sizeof(T));
			++m_availableCount;
		}
		
		bool contains(void *ptr) const {
			if (ptr >= m_buf && ptr < m_buf + 0xFF * sizeof(T)) {
				return true;
			}
			return false;
		}
		
		bool empty() const {
			return m_availableCount == 0xFF;
		}
		
		bool full() const {
			return m_availableCount == 0;
		}
		
		u8 *m_buf;
		u32 m_firstAvailable;
		u32 m_availableCount;
	};
	
	void * _alloc() {
		ScopedLock<FastMutex> guard(m_lock);
		(void)guard;
		Chunk *c = m_chunks + m_lastAllocIdx;
		if (!c->full()) {
			return c->alloc();
		}
		for (int i = 0; i < m_size; ++i) {
			c = m_chunks + i;
			if (!c->full()) {
				m_lastAllocIdx = i;
				return c->alloc();
			}
		}
		m_lastAllocIdx = m_size;
		if (m_size == m_capability) {
			m_capability = m_size + 1;
			Chunk *cs = vnnew Chunk[m_capability];
			memcpy(cs, m_chunks, sizeof(Chunk) * m_size);
			delete []m_chunks;
			m_chunks = cs;
		}
		c = m_chunks + m_size++;
		c->init();
		return c->alloc();
	}
	
	void _free(void *ptr) {
		ScopedLock<FastMutex> guard(m_lock);
		(void)guard;
		Chunk *c = m_chunks + m_lastFreeIdx;
		if (!c->contains(ptr)) {
			int i;
			for (i = 0; i < m_size; ++i) {
				c = m_chunks + i;
				if (c->contains(ptr)) {
					break;
				}
			}
			vnassert(i < m_size);
			m_lastFreeIdx = i;
		}
		c->free(ptr);
		if (c->empty()) {
			m_lastFreeIdx = 0;
			Chunk *last = m_chunks + m_size - 1;
			if (c != last) {
				if (last->empty()) {
					last->fini();
					--m_size;
					if (m_size == m_lastAllocIdx) {
						--m_lastAllocIdx;
					}
					--last;
				}
				if (c != last) {
					Chunk t = *last;
					*last = *c;
					*c = t;
				}
			}
		}
	}
	
	FastMutex m_lock;
	Chunk *m_chunks;
	int m_size;
	int m_capability;
	int m_lastAllocIdx;
	int m_lastFreeIdx;
};


_vn_end

#endif