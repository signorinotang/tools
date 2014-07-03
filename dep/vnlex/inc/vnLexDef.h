//
//  vnLexDef.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef _vnLexDef_h
#define _vnLexDef_h

#include "vnTypes.h"

#ifdef VN_LEX_CONFIG_FILE
#include VN_LEX_CONFIG_FILE
#endif

#ifdef VN_LEX_DYNLIB
#	if VN_COMPILER == VN_COMPILER_MSVC
#		ifdef VN_LEX_EXPORT
#			define _vn_lex_api __declspec(dllexport)
#		else
#			define _vn_lex_api __declspec(dllimport)
#		endif
#	elif VN_COMPILER == VN_COMPILER_LLVM || VN_COMPILER == VN_COMPILER_GUNC
#		ifdef VN_LEX_EXPORT
#			define _vn_lex_api __attribute__((visibility("default")))
#		else
#			define _vn_lex_api
#		endif
#	else
#		define _vn_lex_api
#	endif
#else
#	define _vn_lex_api
#endif

#endif