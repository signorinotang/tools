//
//  vnSingletonManager.cpp
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnSingletonManager.h"
#include "vnScopedLock.h"

#include "vnMemoryTracker.h"

_vn_begin

SingletonManager::SingletonManager() {
#ifdef VN_MEMORY_TRACKER
    MemoryTracker::instance();
#endif
}

SingletonManager::~SingletonManager() {
	destroy_all();
}

SingletonManager & SingletonManager::instance() {
	static SingletonManager s_ins;
	return s_ins;
}

void SingletonManager::_create(void *&ptr, void *(*creator)(), void (*destroyer)()) {
	ScopedLock<FastMutex> guard(m_lock);
	if (!ptr) {
		ptr = creator();
		m_destroyers.push_back(destroyer);
	}
}

void SingletonManager::_destroyed(void (*destroyer)()) {
    ScopedLock<FastMutex> guard(m_lock);
	for (std::list<void (*)()>::iterator it = m_destroyers.begin(); it != m_destroyers.end(); ++it) {
        if (*it == destroyer) {
            m_destroyers.erase(it);
            break;
        }
    }
}

void SingletonManager::destroy_all() {
	while (!m_destroyers.empty()) {
		m_destroyers.back()();
	}
}

_vn_end