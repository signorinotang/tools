//
//  vnSmallObject.cpp
//  vnbase
//
//  Created by Wayne on 13-8-7.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnSmallObject.h"

_vn_begin

SmallObjectAllocator::SmallObjectAllocator() {
	
}

void * SmallObjectAllocator::allocate(size_t size) {
	switch ((size - 1) / sizeof(long)) {
		case 0:	return m_alloc_1.allocate_not_construct();
		case 1:	return m_alloc_2.allocate_not_construct();
		case 2: return m_alloc_3.allocate_not_construct();
		case 3: return m_alloc_4.allocate_not_construct();
		case 4: return m_alloc_5.allocate_not_construct();
		case 5: return m_alloc_6.allocate_not_construct();
		case 6: return m_alloc_7.allocate_not_construct();
		case 7: return m_alloc_8.allocate_not_construct();
	}
	return Memory::allocate(size);
}

void * SmallObjectAllocator::allocate(size_t size, const char *file, int line, int hint) {
	switch ((size - 1) / sizeof(long)) {
		case 0:	return m_alloc_1.allocate_not_construct();
		case 1:	return m_alloc_2.allocate_not_construct();
		case 2: return m_alloc_3.allocate_not_construct();
		case 3: return m_alloc_4.allocate_not_construct();
		case 4: return m_alloc_5.allocate_not_construct();
		case 5: return m_alloc_6.allocate_not_construct();
		case 6: return m_alloc_7.allocate_not_construct();
		case 7: return m_alloc_8.allocate_not_construct();
	}
	return Memory::allocate(size, file, line, hint);
}

void SmallObjectAllocator::deallocate(void *ptr, size_t size) {
	switch ((size - 1) / sizeof(long)) {
		case 0:	return m_alloc_1.deallocate_not_destruct((Block<1> *)ptr);
		case 1:	return m_alloc_2.deallocate_not_destruct((Block<2> *)ptr);
		case 2:	return m_alloc_3.deallocate_not_destruct((Block<3> *)ptr);
		case 3:	return m_alloc_4.deallocate_not_destruct((Block<4> *)ptr);
		case 4:	return m_alloc_5.deallocate_not_destruct((Block<5> *)ptr);
		case 5:	return m_alloc_6.deallocate_not_destruct((Block<6> *)ptr);
		case 6:	return m_alloc_7.deallocate_not_destruct((Block<7> *)ptr);
		case 7:	return m_alloc_8.deallocate_not_destruct((Block<8> *)ptr);
	}
	return Memory::free(ptr);
}

_vn_end
