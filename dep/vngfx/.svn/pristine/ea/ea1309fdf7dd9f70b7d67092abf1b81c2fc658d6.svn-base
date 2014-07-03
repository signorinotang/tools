//
//  vnPicture.cpp
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnPicture.h"

_vn_begin

Picture & Picture::buildTexcoord(const aabox2f &rect) {
    rect.build_corners(texcoords);
    return *this;
}

Picture & Picture::buildTexcoord(const vector2f &size, const aabox2f &rect) {
    aabox2f box = rect / size;
    box.build_corners(texcoords);
    return *this;
}

Picture & Picture::flip_horizontal() {
	math::swap(texcoords[0], texcoords[1]);
	math::swap(texcoords[2], texcoords[3]);
	return *this;
}


Picture & Picture::flip_vertical() {
	math::swap(texcoords[0], texcoords[3]);
	math::swap(texcoords[1], texcoords[2]);
	return *this;
}

Picture & Picture::rotate_90() {
	math::swap(texcoords[0], texcoords[1]);
	math::swap(texcoords[0], texcoords[2]);
	math::swap(texcoords[0], texcoords[3]);
    math::swap(size.x, size.y);
	return *this;
}

Picture & Picture::rotate_180() {
	math::swap(texcoords[0], texcoords[2]);
	math::swap(texcoords[1], texcoords[3]);
	return *this;
}

Picture & Picture::rotate_270() {
	math::swap(texcoords[0], texcoords[1]);
	math::swap(texcoords[1], texcoords[3]);
	math::swap(texcoords[1], texcoords[2]);
    math::swap(size.x, size.y);
	return *this;
}
_vn_end