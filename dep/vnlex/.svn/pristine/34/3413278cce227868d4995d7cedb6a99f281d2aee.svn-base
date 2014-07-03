//
//  vnLR_Node.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef _vnLR_Node_h
#define _vnLR_Node_h

#include "vnToken.h"
#include "vnLR_State.h"

_vn_begin

class _vn_lex_api LR_Node : public RefCounted {
public:
	virtual bool isToken() const = 0;
	virtual Token * token() const = 0;
	virtual u32 productionId() const = 0;
	virtual u32 numChildren() const = 0;
	virtual LR_Node * child(u32 index) const = 0;
};

class _vn_lex_api LR_NodeToken : public LR_Node {
public:
	LR_NodeToken(Token *token) : m_token(token) { token->grab(); }
	virtual ~LR_NodeToken() { m_token->drop(); }
	virtual bool isToken() const { return true; }
	virtual Token * token() const { return m_token; }
	virtual u32 productionId() const { return -1; }
	virtual u32 numChildren() const { return 0; }
	virtual LR_Node * child(u32 index) const { return 0; }
private:
	Token *m_token;
};

class _vn_lex_api LR_NodeNode : public LR_Node {
public:
	LR_NodeNode(const LR_Reduction &reduction, LR_Node **children);
	virtual ~LR_NodeNode();
	virtual bool isToken() const { return false; }
	virtual Token * token() const { return 0; }
	virtual u32 productionId() const { return m_productionId; }
	virtual u32 numChildren() const { return m_numChildren; }
	virtual LR_Node * child(u32 index) const { return m_children[index]; }
private:
	u32 m_productionId;
	LR_Node **m_children;
	u32 m_numChildren;
};

_vn_end

#endif