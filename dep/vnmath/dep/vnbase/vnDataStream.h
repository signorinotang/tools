//
//  vnDataStream.h
//  vnbase
//
//  Created by Wayne on 13-6-12.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnDataStream_h
#define _vnDataStream_h

#include "vnTypes.h"

_vn_begin

class _vn_base_api DataStream {
public:
	struct Exception {

	};

	struct ReadException : Exception {

	};

	struct WriteException : Exception {

	};

	struct buf {
		void *ptr;
		size_t size;
		buf(void *_p, size_t _s) : ptr(_p), size(_s) {}
	};

	struct cbuf {
		const void *ptr;
		size_t size;
		cbuf(const void *_p, size_t _s) : ptr(_p), size(_s) {}
	};
    
    struct z_u32 {
        u32 value;
        z_u32() {}
        z_u32(u32 _value) : value(_value) {}
		operator u32 &() { return value; }
		operator u32 () const { return value; }
    };
    
    struct z_s32 {
        s32 value;
        z_s32() {}
        z_s32(s32 _value) : value(_value) {}
		operator s32 &() { return value; }
		operator s32 () const { return value; }
    };

	enum SeekDir {
		kBegin = SEEK_SET,
		kCurrent = SEEK_CUR,
		kEnd = SEEK_END
	};

	virtual size_t read(void *buffer, size_t size) = 0;
	virtual size_t write(const void *buffer, size_t size)  = 0;
	virtual bool seek(s64 offset, SeekDir way = kBegin) = 0;
	virtual s64 tell() = 0;

	DataStream & operator >>(bool &value);
	DataStream & operator >>(s8 &value);
	DataStream & operator >>(u8 &value);
	DataStream & operator >>(s16 &value);
	DataStream & operator >>(u16 &value);
	DataStream & operator >>(s32 &value);
	DataStream & operator >>(u32 &value);
	DataStream & operator >>(s64 &value);
	DataStream & operator >>(u64 &value);
	DataStream & operator >>(f32 &value);
	DataStream & operator >>(f64 &value);
	DataStream & operator >>(str8 &value);
	DataStream & operator >>(str16 &value);
	DataStream & operator >>(str32 &value);
	DataStream & operator >>(const buf &value);
    DataStream & operator >>(z_u32 &value);
    DataStream & operator >>(z_s32 &value);

	DataStream & operator <<(bool value);
	DataStream & operator <<(s8 value);
	DataStream & operator <<(u8 value);
	DataStream & operator <<(s16 value);
	DataStream & operator <<(u16 value);
	DataStream & operator <<(s32 value);
	DataStream & operator <<(u32 value);
	DataStream & operator <<(s64 value);
	DataStream & operator <<(u64 value);
	DataStream & operator <<(f32 value);
	DataStream & operator <<(f64 value);
	DataStream & operator <<(const str8 &value);
	DataStream & operator <<(const str16 &value);
	DataStream & operator <<(const str32 &value);
	DataStream & operator <<(const cbuf &value);
    DataStream & operator <<(const z_u32 &value);
    DataStream & operator <<(const z_s32 &value);
	
	DataStream & operator <<(const c8 *value);
	DataStream & operator <<(const c16 *value);
	DataStream & operator <<(const c32 *value);

};

_vn_end

#endif