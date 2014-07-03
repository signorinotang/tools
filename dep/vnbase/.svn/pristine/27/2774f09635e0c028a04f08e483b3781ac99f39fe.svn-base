//
//  vnFastMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-11-26.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFastMutex.h"

_vn_begin

FastMutex::FastMutex() {
	::InitializeCriticalSection(&m_impl);
}

FastMutex::~FastMutex() {
	::DeleteCriticalSection(&m_impl);
}

void FastMutex::lock() {
	::EnterCriticalSection(&m_impl);
}

bool FastMutex::try_lock() {
	return ::TryEnterCriticalSection(&m_impl) == TRUE;
}

void FastMutex::unlock() {
	::LeaveCriticalSection(&m_impl);
}

_vn_end