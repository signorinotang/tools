//
//  vnThread.h
//  vnbase
//
//  Created by Wayne on 13-6-14.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnThread_h
#define _vnThread_h

#include "vnFunction0.h"
#include "vnRefCounted.h"

_vn_begin

class _vn_base_api Thread : public AllocatedObject {
public:
	static Thread * create(const function<u32()> &func);
	static void sleep(u32 milliseconds);
	static long current_tid();

	virtual bool join(u32 *retval = 0) = 0;
	virtual void detach() = 0;
	virtual long tid() = 0;
};

class _vn_base_api ThreadData : virtual public RefCounted {
public:
    static ThreadData * create();
    
    virtual void set(void *value) = 0;
    virtual void * get() = 0;
};

_vn_end


#endif