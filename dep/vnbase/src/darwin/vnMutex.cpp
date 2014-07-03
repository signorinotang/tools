//
//  vnMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-6-26.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnMutex.h"

_vn_begin

Mutex::Mutex() {
	pthread_mutex_init(&m_impl, 0);
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&m_impl);
}

void Mutex::lock() {
	pthread_mutex_lock(&m_impl);
}

bool Mutex::try_lock() {
	return pthread_mutex_trylock(&m_impl) == 0;
}

void Mutex::unlock() {
	pthread_mutex_unlock(&m_impl);
}

_vn_end