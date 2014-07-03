//
//  vnNonCopyable.cpp
//  vnbase
//
//  Created by Wayne on 13-9-15.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnNonCopyable.h"

_vn_begin

NonCopyable::NonCopyable() {
	
}

NonCopyable::NonCopyable(const NonCopyable &) {
	
}

NonCopyable & NonCopyable::operator =(const NonCopyable &) {
	return *this;
}

_vn_end
