//
//  vnMemory.h
//  vnbase
//
//  Created by Wayne on 13-6-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnMemory_h
#define _vnMemory_h

#include "vnTypes.h"

_vn_begin

class _vn_base_api Memory {
public:
	static void * allocate(size_t size);
	static void * allocate(size_t size, const char *file, int line, int hint);
	static void free(void *ptr);
};

_vn_end

#ifdef VN_DEBUG
#	define vnmalloc(type, count) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count), __FILE__, __LINE__, 0)
#	define vnmalloc_hint(type, count, hint) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count), __FILE__, __LINE__, hint)
#else
#	define vnmalloc(type, count) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count))
#	define vnmalloc_hint(type, count, hint) (type *)_vn_ns::Memory::allocate(sizeof(type) *(count))
#endif

#define vnfree(ptr) _vn_ns::Memory::free(ptr)

#endif