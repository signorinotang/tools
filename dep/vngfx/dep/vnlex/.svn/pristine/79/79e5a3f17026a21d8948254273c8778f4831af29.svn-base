//
//  vnToken.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef _vnToken_h
#define _vnToken_h

#include "vnLexDef.h"
#include "vnRefCounted.h"

#include <list>

_vn_begin

class _vn_lex_api Token : public RefCounted {
public:
	struct File : RefCounted {
		str8 name;
	};

	enum Type {
		kNull = 0,
		kIdentifier,
		kSymbol,
		kNumber,

		kCharacter,
		kInteger,
		kLongInteger,
		kFloat,
		kDouble,
		kString,

		kKeyword,
	};

	Token(const File *file);
	virtual ~Token();

	Type type;
	str8 text;
	union {
		u32 int32;
		u64 int64;
		f32 float32;
		f64 float64;
	};

	const File * file() const;
	int row;

	Token & set(Type t);
	Token & set(Type t, const str8 &s);
private:
	const File *m_file;
};

typedef RefCountedPtr<Token> TokenPtr;

typedef std::list<TokenPtr> TokenList;

_vn_end

#endif


