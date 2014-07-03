//
//  vnMutex.h
//  vnbase
//
//  Created by Wayne on 13-6-26.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnMutex_h
#define _vnMutex_h

#include "vnNonCopyable.h"

#if VN_PLATFORM == VN_PLATFORM_WIN
#	include <Windows.h>
#elif VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
#   include <pthread.h>
#elif VN_PLATFORM == VN_PLATFORM_FREEBSD
#	include <pthread.h>
#else

#endif

_vn_begin

class _vn_base_api Mutex : public NonCopyable {
public:
	Mutex();
	~Mutex();
	void lock();
	bool try_lock();
	void unlock();
private:
#if VN_PLATFORM == VN_PLATFORM_WIN
	::HANDLE m_impl;
#elif VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
	pthread_mutex_t m_impl;
#elif VN_PLATFORM == VN_PLATFORM_FREEBSD
	pthread_mutex_t m_impl;
#else
	
#endif
};

_vn_end

#endif