//
//  vnAtomic.cpp
//  vnbase
//
//  Created by Wayne on 13-7-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnAtomic.h"
#include <sys/types.h>
#include <machine/atomic.h>

_vn_begin

Atomic::Int32 Atomic::increase(Int32 &v) {
	return atomic_fetchadd_int((volatile unsigned int *)&v, 1) + 1;
}

Atomic::Int64 Atomic::increase(Int64 &v) {
	return atomic_fetchadd_long((volatile unsigned long *)&v, 1) + 1;
}

Atomic::Int32 Atomic::decrease(Int32 &v) {
	return atomic_fetchadd_int((volatile unsigned int *)&v, -1) - 1;
}

Atomic::Int64 Atomic::decrease(Int64 &v) {
	return atomic_fetchadd_long((volatile unsigned long *)&v, -1) - 1;
}

bool Atomic::compareAndSwap(Int32 &v, s32 swap, s32 comparand) {
	return atomic_cmpset_int((volatile unsigned int *)&v, comparand, swap) != 0;
}

bool Atomic::compareAndSwap(Int64 &v, s64 swap, s64 comparand) {
	return atomic_cmpset_long((volatile unsigned long *)&v, (unsigned long)comparand, (unsigned long)swap) != 0;
}

_vn_end