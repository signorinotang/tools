//
//  vnLR_Parser.h
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef _vnLR_Parser_h
#define _vnLR_Parser_h

#include "vnLR_Node.h"
#include <vector>

_vn_begin

class LR_Parser : public RefCounted {
public:
	enum Result {
		kFailed = 0,
		kNext,
		kAccept
	};
	LR_Parser(const LR_State *state0);
	virtual ~LR_Parser();
	Result input(Token *token);
	bool accepted() const;
	LR_Node * result() const;
	void reset();
	
private:
	typedef std::vector<LR_Node *> NodeStack;
	typedef std::vector<const LR_State *> StateStack;
	NodeStack m_nodes;
	StateStack m_states;
	LR_Node *m_result;
	const LR_State *m_state0;

	void _clear();
	void _jump(u32 target, LR_Node *node);
	bool _push(Token *token);
};


_vn_end

#endif