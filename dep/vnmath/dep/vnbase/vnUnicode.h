//
//  vnUnicode.h
//  vnbase
//
//  Created by Wayne on 13-6-8.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnUnicode_h
#define _vnUnicode_h

#include "vnTypes.h"

_vn_begin

class _vn_base_api Unicode {
public:
	static ssize_t convert(const c8 *src, size_t src_len, c32 *dest);
	static ssize_t convert(const c8 *src, size_t src_len, c32 *dest, size_t dest_len);
	static ssize_t convert(const c8 *src, size_t src_len, str32 &dest);

	static ssize_t convert(const c8 *src, size_t src_len, c16 *dest);
	static ssize_t convert(const c8 *src, size_t src_len, c16 *dest, size_t dest_len);
	static ssize_t convert(const c8 *src, size_t src_len, str16 &dest);

	static ssize_t convert(const c16 *src, size_t src_len, c8 *dest);
	static ssize_t convert(const c16 *src, size_t src_len, c8 *dest, size_t dest_len);
	static ssize_t convert(const c16 *src, size_t src_len, str8 &dest);

	static ssize_t convert(const c16 *src, size_t src_len, c32 *dest);
	static ssize_t convert(const c16 *src, size_t src_len, c32 *dest, size_t dest_len);
	static ssize_t convert(const c16 *src, size_t src_len, str32 &dest);

	static ssize_t convert(const c32 *src, size_t src_len, c8 *dest);
	static ssize_t convert(const c32 *src, size_t src_len, c8 *dest, size_t dest_len);
	static ssize_t convert(const c32 *src, size_t src_len, str8 &dest);

	static ssize_t convert(const c32 *src, size_t src_len, c16 *dest);
	static ssize_t convert(const c32 *src, size_t src_len, c16 *dest, size_t dest_len);
	static ssize_t convert(const c32 *src, size_t src_len, str16 &dest);
};

_vn_end

#endif