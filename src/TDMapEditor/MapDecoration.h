//
//  MapDecoration.h
//  TDMapEditor
//
//  Created by Wayne on 4/1/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapDecoration__
#define __TDMapEditor__MapDecoration__

#include "SceneNode.h"
#include "MapDecorationDesc.h"

class MapDecoration : public SceneNode {
public:
    enum Type {
        kImage = 0,
        kAnimation
    };
    
    MapDecoration();
    
    virtual Type type() const = 0;
    
    virtual void setDesc(const MapDecorationDescPtr &desc, bool update = true);
    
    const MapDecorationDescPtr & getDesc() const;
    
    void setPosition(const vn::vector2f &position);
    
    bool isFlipX() const;
    bool isFlipY() const;
    bool isLightMode() const;
    bool isSelected() const;
    const vn::vector2f & getExtent() const;
    const vn::color_t & getColor() const;
    
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    void setSelected(bool selected);
    void setLightMode(bool lightMode);
    void setExtent(const vn::vector2f &extent);
    void setColor(const vn::color_t &color);
    bool contains(const vn::vector2f &point);
    virtual bool canOperate(const vn::vector2f &point);
    void setOperating(bool operating);
protected:
    virtual void _onExtentUpdate();
    virtual void _onLightModeUpdate();
    virtual void _onColorUpdate();
    
    SceneNode *m_node;
    bool m_flipX, m_flipY, m_lightMode;
    vn::color_t m_color;
    vn::vector2f m_extent;
    
    MapDecorationDescPtr m_desc;
private:
    SceneNode *m_bkg;
    SceneNode *m_operator;
    WireFrame *m_operatorFrame;
    WireFrame *m_frame;
};

typedef vn::RefCountedPtr<MapDecoration> MapDecorationPtr;

#endif /* defined(__TDMapEditor__MapDecoration__) */
