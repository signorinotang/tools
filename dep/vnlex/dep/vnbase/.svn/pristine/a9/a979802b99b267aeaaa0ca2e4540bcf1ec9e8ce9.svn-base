//
//  vnFastMutex.h
//  vnbase
//
//  Created by Wayne on 13-6-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFastMutex_h
#define _vnFastMutex_h

#include "vnNonCopyable.h"

#if VN_PLATFORM == VN_PLATFORM_WIN
#	include <Windows.h>
#elif VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#   include <libkern/OSAtomic.h>
#elif VN_PLATFORM == VN_PLATFORM_FREEBSD
#	include <pthread.h>
#else

#endif

_vn_begin

class _vn_base_api FastMutex : public NonCopyable {
public:
	FastMutex();
	~FastMutex();
	void lock();
	bool try_lock();
	void unlock();
private:
#if VN_PLATFORM == VN_PLATFORM_WIN
    CRITICAL_SECTION m_impl;
#elif VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
    volatile OSSpinLock m_impl;
#elif VN_PLATFORM == VN_PLATFORM_FREEBSD
    pthread_spinlock_t m_impl;
#else
    
#endif
};

_vn_end

#endif