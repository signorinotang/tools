//
//  vnLR_Node.cpp
//  vnlex
//
//  Created by Wayne on 13-7-8.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnLR_Node.h"

_vn_begin

LR_NodeNode::LR_NodeNode(const LR_Reduction &reduction, LR_Node **children)
: m_productionId(reduction.productionId)
, m_numChildren(reduction.numItems) {
	m_children = vnmalloc(LR_Node *, m_numChildren);
	memcpy(m_children, children, m_numChildren * sizeof(LR_Node *));
}

LR_NodeNode::~LR_NodeNode() {
	for (u32 i = 0; i < m_numChildren; ++i) {
		m_children[i]->drop();
	}
	vnfree(m_children);
}


_vn_end