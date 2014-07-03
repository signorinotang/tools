//
//  vnLexer.cpp
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnLexer.h"
#include <sstream>

_vn_begin

Lexer::Lexer() {

}

Lexer::~Lexer() {
	for (Analizers::iterator it = m_analizers.begin(); it != m_analizers.end(); ++it) {
		(*it)->drop();
	}
}

void Lexer::add(Analizer *analizer, bool grab) {
	vnassert(analizer);
	m_analizers.push_back(analizer);
	if (grab) {
		analizer->grab();
	}
}

ssize_t Lexer::scan(const c8 *text, size_t length, const Token::File *file, TokenList &result) {
	result.clear();

	Context ctx = {};
	ctx.file = file;
	ctx.tokens = &result;
	
	const c8 *ptr;
	Analizers::iterator it = m_analizers.end();
	for (ptr = text; *ptr && length; ++ptr, --length) {
		ctx.chr = *ptr;
		if (it != m_analizers.end()) {
			Analizer::Result ret = (*it)->process(ctx);
			if (ret == Analizer::kAccept) {
				continue;
			}
			if (ret == Analizer::kError) {
				_makeErrorMsg(ctx);
				return -1;
			}
			ctx.state = 0;
		}
		for (it = m_analizers.begin(); it != m_analizers.end(); ++it) {
			Analizer::Result ret = (*it)->process(ctx);
			if (ret == Analizer::kAccept) {
				ctx.token_row = ctx.row;
				break;
			}
		}
		if (it == m_analizers.end()) {
			_makeErrorMsg(ctx);
			return -1;
		}
	}
	ctx.chr = 0;
	if ((*it)->process(ctx) == Analizer::kError) {
		_makeErrorMsg(ctx);
		return -1;
	}
	m_errorMsg.clear();
	return (ssize_t)(ptr - text);
}

const str8 & Lexer::error() const {
	return m_errorMsg;
}

void Lexer::_makeErrorMsg(const Context &ctx) {
	std::ostringstream oss;
	if (ctx.file) {
		oss << ctx.file->name.c_str();
	}
	oss << '(' << ctx.row + 1 << "): " << ctx.buf.c_str();
	m_errorMsg = oss.str();
}

_vn_end