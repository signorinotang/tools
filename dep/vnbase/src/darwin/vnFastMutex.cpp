//
//  vnFastMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-6-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFastMutex.h"

_vn_begin

FastMutex::FastMutex()
: m_impl(OS_SPINLOCK_INIT) {
    
}

FastMutex::~FastMutex() {
    
}

void FastMutex::lock() {
    OSSpinLockLock(&m_impl);
}

bool FastMutex::try_lock() {
    return OSSpinLockTry(&m_impl);
}

void FastMutex::unlock() {
	OSSpinLockUnlock(&m_impl);
}

_vn_end