//
//  MapObject_SpawnPoint.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-23.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_SpawnPoint__
#define __TDMapEditor__MapObject_SpawnPoint__

#include "MapObject.h"
#include "MapObjectDesc_SpawnPoint.h"

class MapObject_SpawnPoint : public MapObject {
    DECL_MAP_OBJECT(MapObject_SpawnPoint)
public:
    MapObject_SpawnPoint(MapObjectDesc_SpawnPoint *desc);
    
    virtual void setPosition(const vn::vector2i &position);
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
    
protected:
    vn::RefCountedPtr<Text> m_id;
    vn::EditValueObserverProxy m_opId;
    
    void _updateId();
    
    void _onIdChanged(vn::EditValue *);
};

#endif /* defined(__TDMapEditor__MapObject_SpawnPoint__) */
