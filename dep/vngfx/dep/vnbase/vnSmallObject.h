//
//  vnSmallObject.h
//  vnbase
//
//  Created by Wayne on 13-8-7.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnSmallObjectAllocator_h
#define vnbase_vnSmallObjectAllocator_h

#include "vnSingleton.h"
#include "vnFixedAllocator.h"

_vn_begin

class _vn_base_api SmallObjectAllocator : public Singleton<SmallObjectAllocator> {
	friend class Singleton<SmallObjectAllocator>;
public:
	void * allocate(size_t size);
	void * allocate(size_t size, const char *file, int line, int hint);
	void deallocate(void *ptr, size_t size);
private:
	SmallObjectAllocator();
	template <size_t n>
	struct Block {
		long _[n];
	};
	FixedAllocator< Block<1> > m_alloc_1;
	FixedAllocator< Block<2> > m_alloc_2;
	FixedAllocator< Block<3> > m_alloc_3;
	FixedAllocator< Block<4> > m_alloc_4;
	FixedAllocator< Block<5> > m_alloc_5;
	FixedAllocator< Block<6> > m_alloc_6;
	FixedAllocator< Block<7> > m_alloc_7;
	FixedAllocator< Block<8> > m_alloc_8;
};

template class _vn_base_api Singleton<SmallObjectAllocator>;

class _vn_base_api SmallObject {
public:
	void * operator new(size_t size) {
		return SmallObjectAllocator::instance().allocate(size);
	}
	void * operator new(size_t size, const char *file, int line, int hint) {
		return SmallObjectAllocator::instance().allocate(size, file, line, hint);
	}
	void * operator new(size_t, void *p) {
		return p;
	}
	void * operator new[](size_t size) {
		return SmallObjectAllocator::instance().allocate(size);
	}
	void * operator new[](size_t size, const char *file, int line, int hint) {
		return SmallObjectAllocator::instance().allocate(size, file, line, hint);
	}
	void operator delete(void *p, size_t size) {
		SmallObjectAllocator::instance().deallocate(p, size);
	}
	void operator delete(void *p, size_t size, const c8 *, int, int) {
		SmallObjectAllocator::instance().deallocate(p, size);
	}
	void operator delete(void *, void *) {
        
	}
	void operator delete[](void *p, size_t size) {
		SmallObjectAllocator::instance().deallocate(p, size);
	}
	void operator delete[](void *p, size_t size, const c8 *, int, int) {
		SmallObjectAllocator::instance().deallocate(p, size);
	}
};

_vn_end

#endif
