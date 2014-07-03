//
//  vnLexer.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef _vnLexer_h
#define _vnLexer_h

#include "vnToken.h"
#include <vector>

_vn_begin

class _vn_lex_api Lexer : public RefCounted {
public:
	struct Context;
	class Analizer : public RefCounted {
	public:
		enum Result {
			kError = 0,
			kAccept,
			kRefuse
		};
		virtual Result process(Context &context) = 0;
	};

	struct Context {
		const Token::File *file;
		char chr;
		int row;
		int token_row;
		str8 buf;
		int state;
		long data;
		TokenList *tokens;
	};

	Lexer();
	virtual ~Lexer();

	void add(Analizer *analizer, bool grab = true);

	ssize_t scan(const c8 *text, size_t length, const Token::File *file, TokenList &result);

	const str8 & error() const;

private:
	typedef std::vector<Analizer *> Analizers;
	Analizers m_analizers;
	void _makeErrorMsg(const Context &ctx);

	str8 m_errorMsg;
};

_vn_end

#endif