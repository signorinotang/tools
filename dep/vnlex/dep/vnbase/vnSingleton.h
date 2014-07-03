//
//  vnSingleton.h
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnSingleton_h
#define _vnSingleton_h

#include "vnSingletonManager.h"
#include "vnMemory.h"

_vn_begin

template <class T>
class Singleton : public NonCopyable {
public:
	static T & instance() {
		if (!ms_instance) {
			SingletonManager::instance()._create(ms_instance, _create_instance, destory_instance);
		}
		return *(T *)ms_instance;
	}
    
    static void destory_instance() {
		if (ms_instance) {
			((T *)ms_instance)->~T();
			vnfree(ms_instance);
			ms_instance = 0;
			SingletonManager::instance()._destroyed(destory_instance);
		}
	}

private:
	static void * _create_instance() {
		void *ptr = vnmalloc(T, 1);
		new(ptr) T();
		return ptr;
	}



	static void *ms_instance;
};

template <class T>
void * Singleton<T>::ms_instance = 0;

_vn_end

#endif