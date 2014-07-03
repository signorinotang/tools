//
//  MapObject_HeroField.h
//  TDMapEditor
//
//  Created by Wayne on 4/25/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_HeroField__
#define __TDMapEditor__MapObject_HeroField__

#include "MapObject.h"
#include "MapObjectDesc_HeroField.h"

class MapObject_HeroField : public MapObject {
    DECL_MAP_OBJECT(MapObject_HeroField)
public:
    MapObject_HeroField(MapObjectDesc_HeroField *desc);
    
    virtual void setPosition(const vn::vector2i &position);
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();

    
    
};

#endif /* defined(__TDMapEditor__MapObject_HeroField__) */
