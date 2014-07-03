//
//  vnMathDef.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnMathDef_h
#define vnmath_vnMathDef_h

#include "vnBaseDef.h"

#ifdef VN_MATH_DYNLIB
#	if VN_COMPILER == VN_COMPILER_MSVC
#		ifdef VN_MATH_EXPORT
#			define _vn_math_api __declspec(dllexport)
#		else
#			define _vn_math_api __declspec(dllimport)
#		endif
#	elif VN_COMPILER == VN_COMPILER_LLVM || VN_COMPILER == VN_COMPILER_GUNC
#		ifdef VN_MATH_EXPORT
#			define _vn_math_api __attribute__((visibility("default")))
#		else
#			define _vn_math_api
#		endif
#	else
#		define _vn_math_api
#	endif
#else
#	define _vn_math_api
#endif

_vn_begin

typedef float real;

_vn_end

#endif
