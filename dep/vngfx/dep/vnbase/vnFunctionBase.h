//
//  vnFunctionBase.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnFunctionBase_h
#define _vnFunctionBase_h

#include "vnRefCounted.h"

_vn_begin

template <typename _Func = void()>
class function;

namespace internal {
	template <typename _Func>
	struct MemFuncTraits;
}

template <typename _Func>
typename internal::MemFuncTraits<_Func>::FunctionType make_memfn(typename internal::MemFuncTraits<_Func>::ObjectType *ptr, _Func func) {
	return typename internal::MemFuncTraits<_Func>::FunctionType(ptr, func);
}

_vn_end

#endif