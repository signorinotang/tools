//
//  vnLR_Parser.cpp
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnLR_Parser.h"

_vn_begin

LR_Parser::LR_Parser(const LR_State *state0)
: m_result(0)
, m_state0(state0) {
	vnassert(state0);
	m_nodes.reserve(32);
	m_states.reserve(32);
	m_states.push_back(state0);
}

LR_Parser::~LR_Parser() {
	VN_SAFE_DROP(m_result);
	for (NodeStack::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
		(*it)->drop();
	}
	m_nodes.clear();
}

bool LR_Parser::accepted() const {
	return m_result != 0;
}

LR_Node * LR_Parser::result() const {
	return m_result;
}

void LR_Parser::reset() {
	VN_SAFE_DROP(m_result);
	_clear();
	m_states.push_back(m_state0);
}

LR_Parser::Result LR_Parser::input(Token *token) {
	if (m_states.empty()) {
		return kFailed;
	}
	while (!_push(token)) {
		const LR_State *state = m_states.back();
		if (!state->reduction) {
			return kFailed;
		}
		if (token->type == Token::kNull && state->reduction->nodeId == 0) {
			m_result = m_nodes.back();
			m_result->grab();
			_clear();
			return kAccept;
		}
		unsigned int nodeId = state->reduction->nodeId;
		LR_NodeNode *node = vnnew LR_NodeNode(*state->reduction, &m_nodes.back() - state->reduction->numItems + 1);
		for (u32 i = 0; i < state->reduction->numItems; ++i) {
			m_nodes.pop_back();
			m_states.pop_back();
		}
		state = m_states.back();
		const LR_Jump *jump = state->jumps;
		for (;; ++jump) {
			if (jump->cType == LR_Jump::kNull) {
                node->drop();
                return kFailed;
            }
			if (jump->cType == LR_Jump::kNode && jump->nodeId == nodeId) {
				break;
			}
		}
		_jump(jump->targetId, node);
	}
	return kNext;
}

void LR_Parser::_clear() {
	for (NodeStack::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
		(*it)->drop();
	}
	m_nodes.clear();
	m_states.clear();
}

bool LR_Parser::_push(Token *token) {
	const LR_State *state = m_states.back();
	for (const LR_Jump *jump = state->jumps; jump->cType != LR_Jump::kNull; ++jump) {
		if (jump->cType == LR_Jump::kToken) {
			if (token->type == jump->tokenType) {
				_jump(jump->targetId, vnnew LR_NodeToken(token));
				return true;
			}
		}
		if (jump->cType == LR_Jump::kTokenWithValue) {
			if (token->type == jump->tokenType && token->int32 == jump->tokenValue) {
				_jump(jump->targetId, vnnew LR_NodeToken(token));
				return true;
			}
		}
	}
	return false;
}

void LR_Parser::_jump(u32 target, LR_Node *node) {
	m_states.push_back(m_state0 + target);
	m_nodes.push_back(node);
}

_vn_end