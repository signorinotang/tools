//
//  vnScopedLock.h
//  vnbase
//
//  Created by Wayne on 13-6-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnScopedLock_h
#define _vnScopedLock_h

#include "vnTypes.h"

_vn_begin

template <class _Lock>
class ScopedLock : public NonCopyable {
public:
	ScopedLock(_Lock &lock, bool locked = false)
	: m_lock(lock) { if (!locked) lock.lock(); }
	~ScopedLock() { m_lock.unlock(); }
private:
	_Lock &m_lock;
	
};

_vn_end

#endif