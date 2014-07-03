//
//  MapBlockCornerNode.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-31.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapBlockCornerNode__
#define __TDMapEditor__MapBlockCornerNode__

#include "SceneNode.h"
#include "MapData.h"

class MapBlockCornerNode : public SceneNode {
public:
    MapBlockCornerNode(MapBlockCorner *corner);
    
    void updateCorner();
    
protected:
    MapBlockCorner *m_corner;
};

typedef std::map<MapBlockCorner *, MapBlockCornerNode *> MapBlockCornerNodeMap;

#endif /* defined(__TDMapEditor__MapBlockCornerNode__) */
