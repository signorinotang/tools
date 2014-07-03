//
//  MapObject_Tree1x1.h
//  TDMapEditor
//
//  Created by Wayne on 5/9/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_Tree1x1__
#define __TDMapEditor__MapObject_Tree1x1__

#include "MapObject.h"
#include "MapObjectDesc_Tree1x1.h"

class MapObject_Tree1x1 : public MapObject {
    DECL_MAP_OBJECT(MapObject_Tree1x1)
public:
    MapObject_Tree1x1(MapObjectDesc_Tree1x1 *desc);
    virtual void setPosition(const vn::vector2i &position);
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
protected:
    Quad *m_quad;
    Text *m_gold;
    vn::EditValueObserverProxy m_opType;
    vn::EditValueObserverProxy m_opQuality;
    vn::EditValueObserverProxy m_opGold;
    
    void _onTypeChanged(vn::EditValue *value);
    void _onQualityChanged(vn::EditValue *value);
    void _onGoldChanged(vn::EditValue *value);
    
    void _updateQuad(vn::u8 type);
};

#endif /* defined(__TDMapEditor__MapObject_Tree1x1__) */
