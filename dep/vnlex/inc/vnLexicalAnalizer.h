//
//  vnLexicalAnalizer.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef _vnLexicalAnalizer_h
#define _vnLexicalAnalizer_h

#include "vnLexer.h"
#include <map>

_vn_begin

struct LR_Terminal;

class _vn_lex_api LA_Space : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_Identifier : public Lexer::Analizer {
public:
    LA_Identifier();
    LA_Identifier(const LR_Terminal *keywords);
	virtual Result process(Lexer::Context &context);
	static bool isDigit(c8 chr);
	static bool isLetter(c8 chr);
	bool isKeyword(const std::string &s, u32 &outId);
	void registerKeyword(const c8 *keyword, u32 kwId);
private:
	typedef std::map<str8, u32> KeywordMap;
	KeywordMap m_keywords;
};

class _vn_lex_api LA_Character : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_CharacterEx : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_String : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_Number : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_Comment : public Lexer::Analizer {
public:
	virtual Result process(Lexer::Context &context);
};

class _vn_lex_api LA_Symbol : public Lexer::Analizer {
public:
    LA_Symbol();
    LA_Symbol(const LR_Terminal *symbols);
	virtual ~LA_Symbol();
	virtual Result process(Lexer::Context &context);
	void registerSymbol(const c8 *symbol, u32 sId);
private:
	struct Symbol;
	typedef std::map<c8, Symbol *> SymbolMap;
	struct Symbol {
		u32 sId;
		SymbolMap symbols;
		Symbol() ;
		~Symbol();
	};
	
	SymbolMap m_symbols;
};

_vn_end

#endif