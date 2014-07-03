//
//  MapBlockCliffNode.h
//  TDMapEditor
//
//  Created by Wayne on 3/6/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapBlockCliffNode__
#define __TDMapEditor__MapBlockCliffNode__

#include "SceneNode.h"
#include "MapData.h"

class MapBlockCliffNode : public SceneNode {
public:
    MapBlockCliffNode(MapBlockCliff *cliff);
    
    void updateCliff();
    
protected:
    MapBlockCliff *m_cliff;
};

typedef std::map<MapBlockCliff *, MapBlockCliffNode *> MapBlockCliffNodeMap;

#endif /* defined(__TDMapEditor__MapBlockCliffNode__) */
