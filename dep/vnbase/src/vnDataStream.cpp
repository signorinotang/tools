//
//  vnDataStream.cpp
//  vnbase
//
//  Created by Wayne on 13-6-12.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnDataStream.h"
#include "vnUnicode.h"

_vn_begin

DataStream & DataStream::operator >>(bool &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s8 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u8 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s16 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u16 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(s64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(u64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(f32 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(f64 &value) {
	if (read(&value, sizeof(value)) != sizeof(value)) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(str8 &value) {
	z_u32 len;
	*this >> len;
	if (!len.value) {
		value.clear();
	} else {
		value.resize(len.value);
		if (read(&value[0], len.value) != len.value) {
			throw ReadException();
		}
	}
	return *this;
}

DataStream & DataStream::operator >>(str16 &value) {
	str8 t;
	*this >> t;
	value.reserve(t.length());
	Unicode::convert(t.c_str(), t.length(), value);
	return *this;
}

DataStream & DataStream::operator >>(str32 &value) {
	str8 t;
	*this >> t;
	value.reserve(t.length());
	Unicode::convert(t.c_str(), t.length(), value);
	return *this;
}

DataStream & DataStream::operator >>(const buf &value) {
	if (read(value.ptr, value.size) != value.size) {
		throw ReadException();
	}
	return *this;
}

DataStream & DataStream::operator >>(z_u32 &value) {
    u8 lead;
    *this >> lead;
    int num = 0;
    if ((lead & 0x80) == 0) {
        value.value = lead;
    } else if ((lead & 0x40) == 0) {
        num = 1;
        value.value = lead & 0x3F;
    } else if ((lead & 0x20) == 0) {
        num = 2;
        value.value = lead & 0x1F;
    } else if ((lead & 0x10) == 0) {
        num = 3;
        value.value = lead & 0x0F;
    }
    if (num) {
        u8 tail[3];
        if (read(tail, num) != num) {
            throw ReadException();
        }
        for (int i = 0; i < num; ++i) {
            value.value <<= 8;
            value.value |= tail[i];
        }
    }
    return *this;
}

DataStream & DataStream::operator >>(z_s32 &value) {
    u8 lead;
    s32 s = 0;
    *this >> lead;
    int num = 0;
    if ((lead & 0x80) == 0) {
        value.value = lead;
        if (lead & 0x40) {
            s = 0xFFFFFF80;
        }
    } else if ((lead & 0x40) == 0) {
        num = 1;
        value.value = lead & 0x3F;
        if (lead & 0x20) {
            s = 0xFFFFC000;
        }
    } else if ((lead & 0x20) == 0) {
        num = 2;
        value.value = lead & 0x1F;
        if (lead & 0x10) {
            s = 0xFFE00000;
        }
    } else if ((lead & 0x10) == 0) {
        num = 3;
        value.value = lead & 0x0F;
        if (lead & 0x08) {
            s = 0xF0000000;
        }
    }
    if (num) {
        u8 tail[3];
        if (read(tail, num) != num) {
            throw ReadException();
        }
        for (int i = 0; i < num; ++i) {
            value.value <<= 8;
            value.value |= tail[i];
        }
    }
    value.value |= s;
    return *this;
}

///////////////////////////////////////////////////////

DataStream & DataStream::operator <<(bool value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s8 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u8 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s16 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u16 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(s64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(u64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(f32 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(f64 value) {
	if (write(&value, sizeof(value)) != sizeof(value)) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(const str8 &value) {
	z_u32 len;
    len.value = (u32)value.length();
	*this << len;
	if (len.value) {
		if (write(&value[0], len.value) != len.value) {
			throw WriteException();
		}
	}
	return *this;
}

DataStream & DataStream::operator <<(const str16 &value) {
	str8 t;
	t.reserve(value.length());
	Unicode::convert(value.c_str(), value.length(), t);
	return *this << t;
}

DataStream & DataStream::operator <<(const str32 &value) {
	str8 t;
	t.reserve(value.length());
	Unicode::convert(value.c_str(), value.length(), t);
	return *this << t;
}

DataStream & DataStream::operator <<(const cbuf &value) {
	if (write(value.ptr, value.size) != value.size) {
		throw WriteException();
	}
	return *this;
}

DataStream & DataStream::operator <<(const z_u32 &value) {
    vnassert(value.value <= 0x0FFFFFFF);
    u8 buf[4] = {};
    int num = 1;
    if (value.value < 0x80) {
        buf[0] = (u8)value.value;
    } else if (value.value < 0x4000) {
        buf[0] = (u8)((value.value >> 8) | 0x80);
        buf[1] = (u8)value.value;
        num = 2;
    } else if (value.value < 0x200000) {
        buf[0] = (u8)((value.value >> 16) | 0xC0);
        buf[1] = (u8)(value.value >> 8);
        buf[2] = (u8)value.value;
        num = 3;
    } else {
        buf[0] = (u8)((value.value >> 24) | 0xE0);
        buf[1] = (u8)(value.value >> 16);
        buf[2] = (u8)(value.value >> 8);
        buf[3] = (u8)value.value;
        num = 4;
    }
    if (write(buf, num) != num) {
        throw WriteException();
    }
    return *this;
}

DataStream & DataStream::operator <<(const z_s32 &value) {
    vnassert(value.value <= 134217726 && value.value >= -134217727);
    u8 buf[4] = {};
    int num = 1;
    if (value.value < 64 && value.value >= -64) {
        buf[0] = (u8)(value.value & 0x7F);
    } else if (value.value < 8192 && value.value >= -8192) {
        buf[0] = (u8)(((value.value >> 8) & 0x3F) | 0x80);
        buf[1] = (u8)value.value;
        num = 2;
    } else if (value.value < 1048576 && value.value >= -1048576) {
        buf[0] = (u8)(((value.value >> 16) & 0x1F) | 0xC0);
        buf[1] = (u8)(value.value >> 8);
        buf[2] = (u8)value.value;
        num = 3;
    } else {
        buf[0] = (u8)(((value.value >> 24) & 0x0F) | 0xE0);
        buf[1] = (u8)(value.value >> 16);
        buf[2] = (u8)(value.value >> 8);
        buf[3] = (u8)value.value;
        num = 4;
    }
    if (write(buf, num) != num) {
        throw WriteException();
    }
    return *this;
}

DataStream & DataStream::operator <<(const c8 *value) {
	z_u32 len;
    len.value = (u32)std::char_traits<c8>::length(value);
	*this << len;
	if (len.value) {
		if (write(value, len.value) != len.value) {
			throw WriteException();
		}
	}
	return *this;
}

DataStream & DataStream::operator <<(const c16 *value) {
	u16 len = (u16)std::char_traits<c16>::length(value);
	str8 t;
	t.reserve(len);
	Unicode::convert(value, len, t);
	return *this << t;
}

DataStream & DataStream::operator <<(const c32 *value) {
	u16 len = (u16)std::char_traits<c32>::length(value);
	str8 t;
	t.reserve(len);
	Unicode::convert(value, len, t);
	return *this << t;
}

_vn_end