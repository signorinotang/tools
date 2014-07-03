//
//  vnAssert.h
//  vnbase
//
//  Created by Wayne on 13-6-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnAssert_h
#define _vnAssert_h

#include "vnBaseDef.h"
#include <cassert>

#ifdef VN_DEBUG
#	define vnassert(x) assert(x)
#else
#	define vnassert(x)
#endif


#endif