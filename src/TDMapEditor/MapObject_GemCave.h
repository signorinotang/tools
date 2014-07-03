//
//  MapObject_GemCave.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-8.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject_GemCave__
#define __TDMapEditor__MapObject_GemCave__

#include "MapObject.h"
#include "MapObjectDesc_GemCave.h"

class MapObject_GemCave : public MapObject {
    DECL_MAP_OBJECT(MapObject_GemCave)
public:
    MapObject_GemCave(MapObjectDesc_GemCave *desc);
    virtual void setPosition(const vn::vector2i &position);
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
    
protected:
    
    void _updateCaveParts();
    void _updateCaveGems();
    
    vn::RefCountedPtr<Quad> m_caveBackground;
    vn::RefCountedPtr<Quad> m_cavePart[4];
    vn::RefCountedPtr<Text> m_caveGems;
    
    vn::EditValueObserverProxy m_opHole0, m_opHole1, m_opHole2, m_opHole3;
    vn::EditValueObserverProxy m_opNumGems;
    
    void _onHole0Changed(vn::EditValue *);
    void _onHole1Changed(vn::EditValue *);
    void _onHole2Changed(vn::EditValue *);
    void _onHole3Changed(vn::EditValue *);
    void _onNumGemsChanged(vn::EditValue *);
    
    // 0  1  2  3  4
    // 15          5
    // 14          6
    // 13          7
    // 12 11 10 9  8
    static const vn::vector2i s_collisionIndex[16];
    bool m_collision[16];
    bool m_addedToScene;
    
    void _updateCollision(size_t index, bool value);
};

#endif /* defined(__TDMapEditor__MapObject_GemCave__) */
