//
//  MapGridNode.cpp
//  TDMapEditor
//
//  Created by Wayne on 14-1-6.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "MapGridNode.h"
#include "vnFontManager.h"

MapGridNode::MapGridNode(const vn::vector2i &index) {
    vn::color_t color(0xFF, 0xFF, 0xFF, 0x20);
    Quad *p = vnnew Quad();
    *p << vn::aabox2f(0, 0, 38, 2) << color << vn::Render2DInterface::kAlphaAdditive;
    addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(0, 2, 2, 40) << color << vn::Render2DInterface::kAlphaAdditive;
    addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(38, 0, 40, 38) << color << vn::Render2DInterface::kAlphaAdditive;
    addRenderable(p, false);
    
    p = vnnew Quad();
    *p << vn::aabox2f(2, 38, 40, 40) << color << vn::Render2DInterface::kAlphaAdditive;
    addRenderable(p, false);

    Text *t = vnnew Text();
    vn::c8 buf[32];
    sprintf(buf, "%u,%u", index.x + 1, index.y + 1);
    *t << vn::color_t(0xFF, 0xFF, 0xFF, 0x80) << vn::Render2DInterface::kAlphaAdditive << vn::RenderText::kCenter;
    *t << vn::FontManager::instance().defaultFont() << buf;
    SceneNode *node = vnnew SceneNode();
    node->setPosition(vn::vector2f(20, 20));
    node->setScale(vn::vector2f(0.6f, 0.6f));
    node->addRenderable(t, false);
    addChild(node, false);
    
    setPosition(index * 40);
    setLocalBoundingBox(vn::aabox2f(0, 0, 40, 40));
}