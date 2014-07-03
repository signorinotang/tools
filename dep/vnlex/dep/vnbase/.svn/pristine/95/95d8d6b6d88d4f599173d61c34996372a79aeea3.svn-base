//
//  vnRecursiveLock.h
//  vnbase
//
//  Created by Wayne on 14-1-5.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnbase_vnRecursiveLock_h
#define vnbase_vnRecursiveLock_h

#include "vnThread.h"

_vn_begin

template <class _Lock>
class RecursiveLock : protected _Lock {
public:
	RecursiveLock() : m_tid(0), m_count(0) {}
	
	void lock() {
		if (m_lock.try_lock()) {
			++m_count;
			m_tid = Thread::current_tid();
			return ;
		}
		if (m_tid == Thread::current_tid()) {
			++m_count;
			return ;
		}
		m_lock.lock();
		++m_count;
		m_tid = Thread::current_tid();
	}
	
	bool try_lock() {
		if (m_lock.try_lock()) {
			++m_count;
			m_tid = Thread::current_tid();
			return true;
		}
		if (m_tid == Thread::current_tid()) {
			++m_count;
			return true;
		}
		return false;
	}
	
	void unlock() {
		if (--m_count == 0) {
			m_tid = 0;
			m_lock.unlock();
		}
	}
	
private:
	_Lock m_lock;
	long m_tid;
	int m_count;
};

_vn_end


#endif
