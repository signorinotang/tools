//
//  vnTypes.h
//  vnbase
//
//  Created by Wayne on 13-6-5.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnTypes_h
#define _vnTypes_h

#include "vnBaseDef.h"
#include "vnAssert.h"
#include <string>

_vn_begin

#if VN_COMPILER == VN_COMPILER_MSVC

typedef signed __int8		s8;
typedef signed __int16		s16;
typedef signed __int32		s32;
typedef signed __int64		s64;
typedef unsigned __int8		u8;
typedef unsigned __int16	u16;
typedef unsigned __int32	u32;
typedef unsigned __int64	u64;

typedef char				c8;
typedef wchar_t				c16;
typedef	__int32				c32;

typedef long				ssize_t;

#else

typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed long long	s64;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef char				c8;
typedef short				c16;
typedef wchar_t				c32;

#endif

typedef float				f32;
typedef double				f64;
typedef std::basic_string<c8, std::char_traits<c8>, std::allocator<c8> > str8;
typedef std::basic_string<c16, std::char_traits<c16>, std::allocator<c16> > str16;
typedef std::basic_string<c32, std::char_traits<c32>, std::allocator<c32> > str32;
typedef std::wstring strw;

_vn_end


#endif