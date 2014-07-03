//
//  MapBlockNode.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-30.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapBlockNode__
#define __TDMapEditor__MapBlockNode__

#include "SceneNode.h"
#include "MapData.h"

class MapBlockNode : public SceneNode {
public:
    MapBlockNode(MapBlock *block);
    
    void updateBlock();
    
    MapBlock * block() const;
    
protected:
    
    MapBlock *m_block;
};

typedef std::map<MapBlock *, MapBlockNode *> MapBlockNodeMap;

#endif /* defined(__TDMapEditor__MapBlockNode__) */
