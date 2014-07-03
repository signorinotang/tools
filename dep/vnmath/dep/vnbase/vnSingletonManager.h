//
//  vnSingletonManager.h
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnSingletonManager_h
#define _vnSingletonManager_h

#include "vnFastMutex.h"
#include <list>

_vn_begin

class _vn_base_api SingletonManager {
public:
	~SingletonManager();

	static SingletonManager & instance();

	void destroy_all();

	void _create(void *&ptr, void *(*creator)(), void (*destroyer)());
	void _destroyed(void (*destroyer)());
private:
	SingletonManager();
	SingletonManager(const SingletonManager &);

	FastMutex m_lock;
	std::list<void (*)()> m_destroyers;
};

_vn_end

#endif