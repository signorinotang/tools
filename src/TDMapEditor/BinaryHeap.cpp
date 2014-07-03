//
//  BinaryHeap.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BinaryHeap.h"

MapCollision::Item * BinaryHeap::top() const {
    return m_binTree.front();
}

bool BinaryHeap::empty() const {
    return m_binTree.empty();
}

void BinaryHeap::insert(MapCollision::Item *block) {
    vnassert(block);
    size_t idx = m_binTree.size();
    m_binTree.push_back(block);
    _walk_up(idx);
}

MapCollision::Item * BinaryHeap::pop() {
    if (m_binTree.empty()) {
        return 0;
    }
    MapCollision::Item *block = m_binTree.front();
    block->path.open = -1;
    if (m_binTree.size() == 1) {
        m_binTree.pop_back();
    } else {
        m_binTree.front() = m_binTree.back();
        m_binTree.pop_back();
        _walk_down(0);
    }
    return block;
}

void BinaryHeap::update(MapCollision::Item *block) {
    size_t index = block->path.open;
    _walk_up(index);
    if (index == block->path.open) {
        _walk_down(index);
    }
}


void BinaryHeap::_walk_up(size_t index) {
    while (index) {
        size_t parent = (index - 1) / 2;
        if (m_binTree[parent]->path.f_cost <= m_binTree[index]->path.f_cost) {
            break;
        }
        vn::math::swap(m_binTree[index], m_binTree[parent]);
        m_binTree[index]->path.open = index;
        index = parent;
    }
    m_binTree[index]->path.open = index;
}

void BinaryHeap::_walk_down(size_t index) {
    for (;;) {
        size_t childIdx = index * 2 + 1;
        if (childIdx >= m_binTree.size()) {
            break;
        }
        size_t t = childIdx + 1;
        if (t < m_binTree.size() && m_binTree[t]->path.f_cost < m_binTree[childIdx]->path.f_cost) {
            childIdx = t;
        }
        
        if (m_binTree[childIdx]->path.f_cost >= m_binTree[index]->path.f_cost) {
            break;
        }
        vn::math::swap(m_binTree[childIdx], m_binTree[index]);
        m_binTree[index]->path.open = index;
        index = childIdx;
    }
    m_binTree[index]->path.open = index;
}