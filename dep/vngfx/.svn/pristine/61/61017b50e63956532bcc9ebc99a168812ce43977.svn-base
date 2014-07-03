//
//  vnColor.h
//  vngfx
//
//  Created by Wayne on 13-12-6.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnColor_h
#define vngfx_vnColor_h

#include "vnGfxDef.h"

_vn_begin

struct color_t {
	union {
		struct {
			u8 red;
			u8 green;
			u8 blue;
			u8 alpha;
		};
		u32 value;
	};

	color_t();
	color_t(u32 _value);
	color_t(u8 r, u8 g, u8 b, u8 a = 0xFF);
	
	operator u32() const;

	bool operator ==(const color_t &rhs) const;
	bool operator !=(const color_t &rhs) const;

	color_t operator +(const color_t &rhs) const;
	color_t operator -(const color_t &rhs) const;
	color_t operator *(const color_t &rhs) const;
	color_t operator *(f32 rhs) const;

	color_t & operator =(const color_t &rhs);
	color_t & operator +=(const color_t &rhs);
	color_t & operator -=(const color_t &rhs);
	color_t & operator *=(const color_t &rhs);
	color_t & operator *=(f32 rhs);
	
	color_t & convertFromBGRA(u32 bgra);
	u32 convertToBGRA() const;
						 
	f32 luminance() const;
	
	color_t & interpolate(const color_t &a, const color_t &b, f32 f);
	
	static const color_t kWhite;
	static const color_t kBlack;
	static const color_t kRed;
	static const color_t kGreen;
	static const color_t kBlue;
	static const color_t kYellow;
	static const color_t kCyan;
	static const color_t kMegenta;
};

_vn_end

#endif
