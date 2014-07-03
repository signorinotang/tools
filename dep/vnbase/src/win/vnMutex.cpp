//
//  vnMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-11-23.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnMutex.h"

_vn_begin

Mutex::Mutex()
: m_impl(::CreateMutexW(NULL, NULL, NULL)) {
	
}

Mutex::~Mutex() {
	::CloseHandle(m_impl);
}

void Mutex::lock() {
	::WaitForSingleObject(m_impl, INFINITE);
}

bool Mutex::try_lock() {
	return ::WaitForSingleObject(m_impl, 0) == WAIT_OBJECT_0;
}

void Mutex::unlock() {
	::ReleaseMutex(m_impl);
}


_vn_end