//
//  vnFastMutex.cpp
//  vnbase
//
//  Created by Wayne on 13-7-24.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFastMutex.h"

_vn_begin

FastMutex::FastMutex() {
    pthread_spin_init(&m_impl, PTHREAD_PROCESS_PRIVATE);
}

FastMutex::~FastMutex() {
    pthread_spin_destroy(&m_impl);
}

void FastMutex::lock() {
    pthread_spin_lock(&m_impl);
}

bool FastMutex::try_lock() {
    return pthread_spin_trylock(&m_impl) == 0;
}

void FastMutex::unlock() {
	pthread_spin_unlock(&m_impl);
}

_vn_end