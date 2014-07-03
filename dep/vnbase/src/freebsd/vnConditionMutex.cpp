//
//  vnConditionMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-9-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnConditionMutex.h"
#include "vnAssert.h"
#include <sys/time.h>

_vn_begin

ConditionMutex::ConditionMutex() {
	pthread_cond_init(&m_cond, 0);
	pthread_mutex_init(&m_mutex, 0);
}

ConditionMutex::~ConditionMutex() {
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
}

void ConditionMutex::lock() {
	pthread_mutex_lock(&m_mutex);
}

bool ConditionMutex::try_lock() {
	return pthread_mutex_trylock(&m_mutex) == 0;
}

void ConditionMutex::unlock() {
	pthread_mutex_unlock(&m_mutex);
}

void ConditionMutex::signal() {
	pthread_cond_signal(&m_cond);
}

void ConditionMutex::broadcast() {
	pthread_cond_broadcast(&m_cond);
}

void ConditionMutex::wait() {
	pthread_cond_wait(&m_cond, &m_mutex);
}

bool ConditionMutex::timed_wait(int milliseconds) {
	vnassert(milliseconds >= 0);
	timeval tv;
	timespec ts;
	::gettimeofday(&tv, 0);
	ts.tv_sec = tv.tv_sec + milliseconds / 1000;
	ts.tv_nsec = tv.tv_usec + milliseconds % 1000 * 1000000L;
	ts.tv_sec += ts.tv_nsec / 1000000000L;
	ts.tv_nsec %= 1000000000L;
	return pthread_cond_timedwait(&m_cond, &m_mutex, &ts) == 0;
}

_vn_end