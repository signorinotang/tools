//
//  vnGfxDef.h
//  vngfx
//
//  Created by Wayne on 13-11-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnGfxDef_h
#define vngfx_vnGfxDef_h

#include "vnTypes.h"

#ifdef VN_GFX_CONFIG_FILE
#include VN_GFX_CONFIG_FILE
#endif

#ifdef VN_GFX_DYNLIB
#	if VN_COMPILER == VN_COMPILER_MSVC
#		ifdef VN_GFX_EXPORT
#			define _vn_gfx_api __declspec(dllexport)
#		else
#			define _vn_gfx_api __declspec(dllimport)
#		endif
#	elif VN_COMPILER == VN_COMPILER_LLVM || VN_COMPILER == VN_COMPILER_GUNC
#		ifdef VN_GFX_EXPORT
#			define _vn_gfx_api __attribute__((visibility("default")))
#		else
#			define _vn_gfx_api
#		endif
#	else
#		define _vn_gfx_api
#	endif
#else
#	define _vn_gfx_api
#endif

#endif
