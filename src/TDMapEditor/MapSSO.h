//
//  MapSSO.h
//  TDMapEditor
//
//  Created by Wayne on 6/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapSSO__
#define __TDMapEditor__MapSSO__

#include "StateScriptObject.h"
#include "SceneNode.h"
#include "MapData.h"

class MapSSO : public StateScriptObject, public SceneNode {
public:
    MapSSO(const MapStateScriptObjectDescPtr &desc = MapStateScriptObjectDescPtr::kNull);
    bool init(const FilePath &scriptFile, const vn::vector2f &pos);
    bool init();
    
    virtual void sso_setVisible(bool visible);
    virtual void sso_setScale(const vn::vector2f &scale);
    virtual void sso_move(const vn::vector2f &offset);
    virtual void sso_setRotation(const vn::f32 rotation);
    virtual void sso_playSound(vn::u32 soundId) {}
    virtual void sso_destroy();
    virtual void anim_frame(const aabox2f &rect, const PicturePtr &image);
    
    virtual void setPosition(const vector2f &position);
    virtual void update(vn::f32 deltaTime);
    
    bool contains(const vn::vector2f &point) const;
    
    void setFrameVisible(bool visible);
    
    const MapStateScriptObjectDescPtr & desc() { return m_desc; }
    
protected:
    MapStateScriptObjectDescPtr m_desc;
    SceneNode *m_objectNode = vnnew SceneNode();
    SceneNode *m_frameNode = vnnew SceneNode();
    WireFrame *m_frame = vnnew WireFrame();
    Quad *m_quad = vnnew Quad;
};

typedef RefCountedPtr<MapSSO> MapSSOPtr;

#endif /* defined(__TDMapEditor__MapSSO__) */
