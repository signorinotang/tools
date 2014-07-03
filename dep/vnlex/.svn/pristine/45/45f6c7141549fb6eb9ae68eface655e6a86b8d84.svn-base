//
//  vnToken.cpp
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnToken.h"

_vn_begin

Token::Token(const File *file)
: m_file(file)
, type(kNull)
, row(0) {
	int64 = 0;
	if (file) {
		file->grab();
	}
}

Token::~Token() {
	VN_SAFE_DROP(m_file);
}

const Token::File * Token::file() const {
	return m_file;
}

Token & Token::set(Type t) {
	type = t;
	return *this;
}

Token & Token::set(Type t, const str8 &s) {
	type = t;
	text = s;
	return *this;
}

_vn_end