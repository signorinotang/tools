//
//  vnConditionMutex.h
//  vnbase
//
//  Created by Wayne on 13-9-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnConditionMutex_h
#define vnbase_vnConditionMutex_h

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

class _vn_base_api ConditionMutex : public NonCopyable {
public:
	ConditionMutex();
	~ConditionMutex();
	
	void lock();
	bool try_lock();
	void unlock();
	
	void signal();
	void broadcast();
	
	void wait();
	bool timed_wait(int milliseconds);
	
private:
#if VN_PLATFORM == VN_PLATFORM_WIN
	::CRITICAL_SECTION m_mutex;
	::CONDITION_VARIABLE m_cond;
#elif VN_PLATFORM == VN_PLATFORM_MAC || VN_PLATFORM == VN_PLATFORM_IOS
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
#elif VN_PLATFORM == VN_PLATFORM_FREEBSD
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
#else
	
#endif
};

_vn_end


#endif
