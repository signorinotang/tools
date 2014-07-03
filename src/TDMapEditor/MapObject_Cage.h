//
//  MapObject_Cage.h
//  TDMapEditor
//
//  Created by Wayne on 6/25/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_Cage__
#define __TDMapEditor__MapObject_Cage__

#include "MapObject.h"
#include "MapObjectDesc_Cage.h"

class MapObject_Cage : public MapObject {
    DECL_MAP_OBJECT(MapObject_Cage)
public:
    MapObject_Cage(MapObjectDesc_Cage *desc);
    
    virtual void setPosition(const vn::vector2i &position);
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
    
protected:
    vn::RefCountedPtr<Text> m_name;
    vn::EditValueObserverProxy m_opQuality;
    vn::EditValueObserverProxy m_opCloseId;
    vn::EditValueObserverProxy m_opOpenId;
    vn::EditValueObserverProxy m_opName;
    
    void _updateName();
    
    void _onQualityChanged(vn::EditValue *value);
    void _onCloseIdChanged(vn::EditValue *value);
    void _onOpenIdChanged(vn::EditValue *value);
    void _onNameChanged(vn::EditValue *value);
    
    bool m_addedToScene;
    static const vn::vector2i s_collisionIndex[4];
};

#endif /* defined(__TDMapEditor__MapObject_Cage__) */
