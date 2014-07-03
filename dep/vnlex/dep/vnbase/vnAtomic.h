//
//  vnAtomic.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnAtomic_h
#define _vnAtomic_h

#include "vnTypes.h"

_vn_begin

class _vn_base_api Atomic {
public:
	typedef volatile s32 Int32;
	typedef volatile s64 Int64;
	static Int32 increase(Int32 &v);
	static Int64 increase(Int64 &v);
	static Int32 decrease(Int32 &v);
	static Int64 decrease(Int64 &v);
	static bool compareAndSwap(Int32 &v, s32 swap, s32 comparand);
	static bool compareAndSwap(Int64 &v, s64 swap, s64 comparand);
};

_vn_end

#endif