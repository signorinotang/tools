//
//  vnConditionMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-9-20.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnConditionMutex.h"
#include "vnAssert.h"

_vn_begin

ConditionMutex::ConditionMutex() {
	::InitializeCriticalSection(&m_mutex);
	::InitializeConditionVariable(&m_cond);
}

ConditionMutex::~ConditionMutex() {
	::DeleteCriticalSection(&m_mutex);
}

void ConditionMutex::lock() {
	::EnterCriticalSection(&m_mutex);
}

bool ConditionMutex::try_lock() {
	return ::TryEnterCriticalSection(&m_mutex) != FALSE;
}

void ConditionMutex::unlock() {
	::LeaveCriticalSection(&m_mutex);
}

void ConditionMutex::signal() {
	::WakeConditionVariable(&m_cond);
}

void ConditionMutex::broadcast() {
	::WakeAllConditionVariable(&m_cond);
}

void ConditionMutex::wait() {
	::SleepConditionVariableCS(&m_cond, &m_mutex, INFINITE);
}

bool ConditionMutex::timed_wait(int milliseconds) {
	vnassert(milliseconds >= 0);
	return ::SleepConditionVariableCS(&m_cond, &m_mutex, milliseconds) != FALSE;
}

_vn_end