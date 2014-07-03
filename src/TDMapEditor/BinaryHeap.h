//
//  BinaryHeap.h
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__BinaryHeap__
#define __TDMapEditor__BinaryHeap__

#include "MapCollision.h"

class BinaryHeap {
public:
    MapCollision::Item * top() const;
    bool empty() const;
    
    void insert(MapCollision::Item *block);
    void update(MapCollision::Item *block);
    MapCollision::Item * pop();
    
    void clear();
protected:
    std::vector<MapCollision::Item *> m_binTree;
    
    void _walk_up(size_t index);
    void _walk_down(size_t index);
};

#endif /* defined(__TDMapEditor__BinaryHeap__) */
