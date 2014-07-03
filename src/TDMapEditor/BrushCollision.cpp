//
//  BrushCollision.cpp
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "BrushCollision.h"
#include "EditorApp.h"
#include "MapObjectDesc_SpawnPoint.h"
#include "MapObjectDesc_Building.h"

#include "vnUILocatorRect.h"

#include <sstream>

Brush_Collision::Brush_Collision()
: m_uiWindow(0)
, m_moved(false)
, m_collisionMask(1) {
    
}

Brush_Collision::~Brush_Collision() {
    
}

void Brush_Collision::init() {
    BlockBrush::init();
    m_uiWindow = vnnew vn::UIWindow();
    m_uiWindow->setSizeable(false);
    vn::UILocatorRectEx *locator = vnnew vn::UILocatorRectEx(m_uiWindow);
    locator->setSize(vn::vector2f(140, 182));
    locator->setPositionXAlignment(vn::UILocatorRectEx::kBig);
    locator->setPosition(vn::vector2f(16, 64));
    
    vn::UIFactory::instance().createElementsFromTDL(m_uiWindow, "1:/TDMapEditor/ui_brush_collision.tdl");
    vn::UIFactory::ElementMap namedElements;
    m_uiWindow->buildElementMap(namedElements);
    
    m_uiWater = dynamic_cast<vn::UIButton *>(namedElements["water"]);
    if (m_uiWater) {
        m_uiWater->bindAction_Clicked(vn::make_memfn(this, &Brush_Collision::_onWaterClicked));
    }
}

void Brush_Collision::fini() {
    BlockBrush::fini();
    VN_SAFE_DROP(m_uiWindow);
}

void Brush_Collision::begin(const vn::vector2f &pos) {
    _updateBrush(pos);
    m_lastPos = pos;
    m_moved = false;
}

void Brush_Collision::end(const vn::vector2f &pos) {
    _updateBrush(pos);
    if (!m_moved) {
        MapCollision &mc = EditorApp::instance().mapCollision();
        mc.clearPath();
        std::vector<const MapCollision::Item *> result;
        int cost = -1;
        std::ostringstream oss;
        oss << "path: ";
        for (auto it = m_spawnPoints.begin(); it != m_spawnPoints.end(); ++it) {
            oss << "[" << it->first << "] -> ";
            if (!mc.findPath(it->second, m_index, m_collisionMask, result, cost)) {
                oss << -1;
            } else {
                for (auto iit = result.begin(); iit != result.end(); ++iit) {
                    (*iit)->node->set(true);
                }
                oss << cost;
            }
            oss << "     ";
        }
        VN_LOG_INFO(oss.str());
    }
}

void Brush_Collision::update(vn::f32 deltaTime, const vn::vector2f &pos, bool paint) {
    if (paint) {
        if (pos != m_lastPos) {
            m_moved = true;
            EditorApp::instance().camera().move(m_lastPos - pos);
            m_lastPos = pos;
        }
    } else {
        _updateBrush(pos);
    }
}

bool Brush_Collision::busy() const {
    return false;
}

void Brush_Collision::switchIn() {
    BlockBrush::switchIn();
    EditorApp::instance().layer_collision()->setVisible(true);
    EditorApp::instance().uiRootElement()->addChild(m_uiWindow);
    
    const MapData &mapData = EditorApp::instance().mapData();
    for (size_t i = 0, c = mapData.objectDescCount(); i < c; ++i) {
        const MapObjectDesc_SpawnPoint *sp = dynamic_cast<const MapObjectDesc_SpawnPoint *>(mapData.objectDesc(i));
        if (sp) {
            m_spawnPoints[sp->getId()] = sp->getPosition();
        }
        const MapObjectDesc_Building *building = dynamic_cast<const MapObjectDesc_Building *>(mapData.objectDesc(i));
        if (building && building->getSpawnPointId() && building->getSpawnPointPosition()) {
            m_spawnPoints[building->getSpawnPointId()] = building->buildSpawnPoint();
        }
    }
}

void Brush_Collision::switchOut() {
    BlockBrush::switchOut();
    EditorApp::instance().layer_collision()->setVisible(false);
    EditorApp::instance().uiRootElement()->removeChild(m_uiWindow);
    EditorApp::instance().mapCollision().clearPath();
    m_spawnPoints.clear();
}

void Brush_Collision::onKeyDown(vn::c32 chr, bool repeat) {
}

void Brush_Collision::onMouseRightDown(const vn::vector2f &pos) {
    
}

void Brush_Collision::onMouseRightUp(const vn::vector2f &pos) {
    
}

void Brush_Collision::onMapObjectRemoved(MapObject *mobj) {

}

void Brush_Collision::onMapDecorationRemoved(MapDecoration *deco) {

}

void Brush_Collision::_onWaterClicked(vn::UIElement *) {
    if (m_uiWater->isPushed()) {
        m_uiWater->setPushed(false);
        m_collisionMask = 1;
    } else {
        m_uiWater->setPushed(true);
        m_collisionMask = 3;
    }
}