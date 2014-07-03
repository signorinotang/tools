//
//  vnAtomic.cpp
//  vnbase
//
//  Created by Wayne on 13-6-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnAtomic.h"
#include <libkern/OSAtomic.h>

_vn_begin

Atomic::Int32 Atomic::increase(Int32 &v) {
	return OSAtomicIncrement32Barrier(&v);
}

Atomic::Int64 Atomic::increase(Int64 &v) {
	return OSAtomicIncrement64Barrier(&v);
}

Atomic::Int32 Atomic::decrease(Int32 &v) {
	return OSAtomicDecrement32Barrier(&v);
}

Atomic::Int64 Atomic::decrease(Int64 &v) {
	return OSAtomicDecrement64Barrier(&v);
}

bool Atomic::compareAndSwap(Int32 &v, s32 swap, s32 comparand) {
	return OSAtomicCompareAndSwap32Barrier(comparand, swap, &v);
}

bool Atomic::compareAndSwap(Int64 &v, s64 swap, s64 comparand) {
	return OSAtomicCompareAndSwap64Barrier(comparand, swap, &v);
}

_vn_end