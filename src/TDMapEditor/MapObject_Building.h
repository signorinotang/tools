//
//  MapObject_Building.h
//  TDMapEditor
//
//  Created by Wayne on 4/30/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_Building__
#define __TDMapEditor__MapObject_Building__

#include "MapObject.h"
#include "MapObjectDesc_Building.h"

class MapObject_Building : public MapObject {
    DECL_MAP_OBJECT(MapObject_Building)
public:
    MapObject_Building(MapObjectDesc_Building *desc);
    virtual void setPosition(const vn::vector2i &position);
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
    
protected:
    vn::RefCountedPtr<Text> m_num, m_spid;
    SceneNodePtr m_spNode;
    
    vn::EditValueObserverProxy m_opQuality;
    vn::EditValueObserverProxy m_opGold;
    vn::EditValueObserverProxy m_opSPId;
    vn::EditValueObserverProxy m_opSPPos;
    vn::EditValueObserverProxy m_opBuffId;
    
    void _updateNum();
    void _updateSpawnPoint();

    void _onQualityChanged(vn::EditValue *value);
    void _onGoldChanged(vn::EditValue *value);
    void _onSPIdChanged(vn::EditValue *value);
    void _onSPPosChanged(vn::EditValue *value);
    void _onBuffIdChanged(vn::EditValue *value);
    
    bool m_addedToScene;
    
    static const vn::vector2i s_collisionIndex[4];
};


#endif /* defined(__TDMapEditor__MapObject_Building__) */
