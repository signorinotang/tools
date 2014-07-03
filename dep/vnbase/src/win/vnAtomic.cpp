//
//  vnAtomic.cpp
//  vnbase
//
//  Created by Wayne on 13-11-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnAtomic.h"
#include <Windows.h>

_vn_begin

Atomic::Int32 Atomic::increase(Int32 &v) {
	return ::InterlockedIncrement((volatile unsigned int *)&v);
}

Atomic::Int64 Atomic::increase(Int64 &v) {
	return ::InterlockedIncrement64(&v);
}

Atomic::Int32 Atomic::decrease(Int32 &v) {
	return ::InterlockedDecrement((volatile unsigned int *)&v);
}

Atomic::Int64 Atomic::decrease(Int64 &v) {
	return ::InterlockedDecrement64(&v);
}

bool Atomic::compareAndSwap(Int32 &v, s32 swap, s32 comparand) {
	return ::InterlockedCompareExchange((volatile unsigned int *)&v, swap, comparand) == comparand;
}

bool Atomic::compareAndSwap(Int64 &v, s64 swap, s64 comparand) {
	return ::InterlockedCompareExchange64(&v, swap, comparand) == comparand;
}

_vn_end