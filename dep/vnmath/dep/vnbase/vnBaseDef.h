//
//  vnBaseDef.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnBaseDef_h
#define _vnBaseDef_h

#include <cstddef>
#include "vnPlatform.h"

#define _vn_begin 	namespace vn {
#define _vn_end		}
#define _vn_ns		::vn

#ifdef VN_BASE_CONFIG_FILE
#include VN_BASE_CONFIG_FILE
#endif

#ifdef VN_BASE_DYNLIB
#	if VN_COMPILER == VN_COMPILER_MSVC
#		ifdef VN_BASE_EXPORT
#			define _vn_base_api __declspec(dllexport)
#		else
#			define _vn_base_api __declspec(dllimport)
#		endif
#	elif VN_COMPILER == VN_COMPILER_LLVM || VN_COMPILER == VN_COMPILER_GUNC
#		ifdef VN_BASE_EXPORT
#			define _vn_base_api __attribute__((visibility("default")))
#		else
#			define _vn_base_api
#		endif
#	else
#		define _vn_base_api
#	endif
#else
#	define _vn_base_api
#endif

#ifdef VN_DEBUG
#   define VN_MEMORY_TRACKER
#endif

#if VN_COMPILER == VN_COMPILER_MSVC
#	pragma warning(disable:4251)
#endif

#endif