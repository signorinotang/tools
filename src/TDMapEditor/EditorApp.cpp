 //
//  EditorApp.cpp
//  TDMapEditor
//
//  Created by Wayne on 13-12-26.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "EditorApp.h"
#include "UIEditorRoot.h"
#include "vnRender2DInterface.h"
#include "vnFontManager.h"
#include "vnFileManager.h"
#include "vnTreeDataScript.h"
#include "vnTextureManager.h"
#include "vnPictureManager.h"
#include "vnUnicode.h"

#include "../common/UILogList.h"

#include "Renderable.h"
#include "BrushBlockType.h"
#include "BrushObjectNew.h"
#include "BrushObjectEdit.h"
#include "BrushDecoration.h"
#include "BrushCollision.h"
#include "BrushSSO.h"

#include "MapDecorationImage.h"
#include "MapDecorationAnimation.h"

#include "MapGridNode.h"

EditorApp::EditorApp()
: m_mouseDown(false)
, m_currentBrush(0)
, m_commandList(128)
, m_brushList(0)
, m_cameraZoomSource(1.f)
, m_cameraZoomDestination(1.f)
, m_cameraZoomFactor(0)
, m_uiNewMapRoot(0)
, m_uiNewMapHeight(0) {
    
}

vn::UIElement * EditorApp::_createRootElement() {
    return vnnew UIEditorRoot();
}

void EditorApp::_init(const vn::vector2f &size) {
    _loadConfigFile();
    vn::FontManager::instance().createFontFT("", "1:/common/msyh.ttf", 24);
    
    // ui
    vn::UIFactory::instance().createElementsFromTDL(m_uiRootElement, "1:/TDMapEditor/ui_main.tdl");
    vn::UIFactory::ElementMap namedElements;
    m_uiRootElement->buildElementMap(namedElements);
    
    
    UILogList *loglist = (UILogList *)vn::UIFactory::instance().createLogicFromTDL("1:/common/log_list.tdl");
    //vn::Log::instance().removeAllPrinters();
    vn::Log::instance().addPrinter(loglist, false);
    namedElements["wnd_log"]->addChild(loglist->root());
    
    m_brushList = (vn::UIList *)namedElements["brush_list"];
    m_brushList->setSelectedItem(m_brushList->item(0));
    m_brushList->bindAction_ItemClicked(vn::make_memfn(this, &EditorApp::_onBrushListItemClicked));
    
    namedElements["new"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onNew));
    namedElements["open"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onOpen));
    namedElements["save"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onSave));
    namedElements["save_as"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onSaveAs));
    namedElements["refresh"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onRefresh));
    
    m_uiNewMapRoot = namedElements["new_map_root"];
    m_uiNewMapHeight = dynamic_cast<vn::UITextField *>(namedElements["new_map_height"]);
    namedElements["new_map_ok"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onNewMapOk));
    namedElements["new_map_cancel"]->bindAction("clicked", vn::make_memfn(this, &EditorApp::_onNewMapCancel));
    if (m_uiNewMapHeight) {
        m_uiNewMapHeight->bindAction_TextCompleted(vn::make_memfn(this, &EditorApp::_onNewMapText));
    }
    
    m_fileBrowser = (UIFileBrowser *)vn::UIFactory::instance().createLogicFromTDL("1:/common/file_browser.tdl");
    m_fileBrowser->startup();
    
    // camera
    m_camera.setViewFocus(size * 0.5f);
    m_camera.setViewSize(size);
    
    // scene
    for (int i = 0; i < 3; ++i) {
        m_fieldBlockLayer[i] = vnnew SceneNode();
    }
    m_landBlockLayer = vnnew SceneNode();
    m_waterBlockLayer = vnnew SceneNode();
    m_collisionLayer = vnnew SceneNode();
    m_gridLayer = vnnew SceneNode();
    m_brushLayer = vnnew SceneNode();
    m_landObjectLayer = vnnew SceneNode();
    m_fieldObjectLayer = vnnew SceneNode();
    
    m_cliffLayer = vnnew SceneNode();
    for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
        m_cornerLayer[i] = vnnew SceneNode();
    }
    
    m_decorationLayer = vnnew SceneNode();
    m_ssoLayer = vnnew SceneNode();
    
    m_collisionLayer->setVisible(false);
    m_gridLayer->setVisible(false);
    
    
    // brushs
    m_currentBrush = vnnew Brush_MoveCamera();
    m_brushes[kBrush_MoveCamera] = m_currentBrush;
    m_brushes[kBrush_BlockType] = vnnew Brush_BlockType();
	m_brushes[kBrush_ObjectNew] = vnnew Brush_ObjectNew();
    m_brushes[kBrush_ObjectEdit] = vnnew Brush_ObjectEdit();
    m_brushes[kBrush_Decoration] = vnnew Brush_Decoration();
    m_brushes[kBrush_Collision] = vnnew Brush_Collision();
    m_brushes[kBrush_SSO] = vnnew Brush_SSO();
    
    for (BrushMap::iterator it = m_brushes.begin(); it != m_brushes.end(); ++it) {
        it->second->init();
    }
    m_currentBrush->switchIn();
    
    _newMap(29, 23);
    
}

void EditorApp::_fini() {
    
    m_currentBrush->switchOut();
    m_currentBrush = 0;
    for (BrushMap::iterator it = m_brushes.begin(); it != m_brushes.end(); ++it) {
        it->second->fini();
    }
    m_brushes.clear();
    
    for (MapObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
        (*it)->drop();
    }
    m_objects.clear();
    
    m_fileBrowser.release();
}

void EditorApp::_update(vn::f32 deltaTime) {
	if (m_cameraZoomFactor != 1.f) {
		m_cameraZoomFactor += deltaTime * 4.f;
		if (m_cameraZoomFactor > 1.f) {
			m_cameraZoomFactor = 1.f;
		}
		vn::f32 zoom = m_cameraZoomSource + (m_cameraZoomDestination - m_cameraZoomSource) * m_cameraZoomFactor;
		m_camera.setZoom(zoom);
	}
	m_camera.update();
    
    m_currentBrush->update(deltaTime, vn::GfxApplication::instance().getMousePosition(), m_mouseDown);
    
    m_visibleNodes.clear();
    m_landBlockLayer->update(deltaTime);
    m_waterBlockLayer->update(deltaTime);
    for (int i = 0; i < 3; ++i) {
        m_fieldBlockLayer[i]->update(deltaTime);
    }

    m_cliffLayer->update(deltaTime);
    for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
        m_cornerLayer[i]->update(deltaTime);
    }
    
    m_landObjectLayer->update(deltaTime);
    m_fieldObjectLayer->update(deltaTime);
    m_brushLayer->update(deltaTime);
    m_collisionLayer->update(deltaTime);
    m_gridLayer->update(deltaTime);
    
    m_decorationLayer->update(deltaTime);
    m_ssoLayer->update(deltaTime);
    
    m_landBlockLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    
    
    m_waterBlockLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    
    if (m_cornerLayer[0]->isVisible()) {
        m_cornerLayer[0]->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    
    if (m_cornerLayer[4]->isVisible()) {
        m_cornerLayer[4]->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    
    if (m_cliffLayer->isVisible()) {
        m_cliffLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    
    m_landObjectLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    
    
    for (size_t i = 1; i < 4; ++i) {
        m_fieldBlockLayer[i - 1]->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
        if (!m_cornerLayer[i]->isVisible()) {
            continue;
        }
        m_cornerLayer[i]->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    
    m_decorationLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    m_ssoLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    
    m_fieldObjectLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    
    if (m_collisionLayer->isVisible()) {
        m_collisionLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    
    if (m_gridLayer->isVisible()) {
        m_gridLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    }
    m_brushLayer->queryVisibleChildren(m_camera.boundingBox(), m_visibleNodes);
    for (std::vector<SceneNode *>::iterator it = m_visibleNodes.begin(); it != m_visibleNodes.end(); ++it) {
        (*it)->updateRenderables();
    }
}

void EditorApp::_render() {
    m_device->setViewMatrix(m_camera.viewMatrix());
    
    for (std::vector<SceneNode *>::iterator it = m_visibleNodes.begin(); it != m_visibleNodes.end(); ++it) {
        (*it)->renderRenderables();
    }
    
    m_device->setViewMatrix(vn::matrix3(vn::matrix3::c_identity));
    vn::Render2DInterface::instance().setBlendMode(vn::Render2DInterface::kAlphaBlend);
}

void EditorApp::_resize(const vn::vector2f &size) {
    m_camera.setViewFocus(size * 0.5f);
    m_camera.setViewSize(size);
}

void EditorApp::do_undo() {
	if (m_mouseDown || m_currentBrush->busy()) {
		return ;
	}
	m_commandList.undo();
}

void EditorApp::do_redo() {
	if (m_mouseDown|| m_currentBrush->busy()) {
		return ;
	}
	m_commandList.redo();
}

void EditorApp::onRootMouseLeftDown(const vn::vector2f &pt) {
    vn::UIRoot::instance().captureMouse(m_uiRootElement);
    m_mouseDown = true;
    m_currentBrush->begin(pt);
}

void EditorApp::onRootMouseLeftUp(const vn::vector2f &pt) {
    if (m_mouseDown) {
        m_mouseDown = false;
        vn::UIRoot::instance().releaseMouse(m_uiRootElement);
        m_currentBrush->end(pt);
    }
}

void EditorApp::onRootMouseRightDown(const vn::vector2f &pt) {
    m_currentBrush->onMouseRightDown(pt);
}

void EditorApp::onRootMouseRightUp(const vn::vector2f &pt) {
    m_currentBrush->onMouseRightUp(pt);
}

void EditorApp::onRootMouseCaptureCancelled() {
    m_mouseDown = false;
    m_currentBrush->end(vn::GfxApplication::instance().getMousePosition());
}

void EditorApp::onRootKeyDown(vn::c32 chr, bool repeat) {
    switch (chr) {
        case '\x1B': {
            // ESC
            if (m_fileBrowser->visible()) {
                m_fileBrowser->hide();
            } else {
                m_currentBrush->onKeyDown(chr, repeat);
            }
            break;
        }
#if VN_PLATFORM == VN_PLATFORM_WIN
		case 221:
#else
		case ']':
#endif
        {
              if (m_cameraZoomDestination < 8.f) {
				  m_cameraZoomSource = m_camera.getZoom();
				  m_cameraZoomDestination *= 2.f;
				  m_cameraZoomFactor = 0.f;
                  char buf[16];
				  sprintf(buf, "zoom %u%%", (vn::u32)(m_cameraZoomDestination * 100.f));
                  VN_LOG_INFO(buf);
              }
              break;
		}
#if VN_PLATFORM == VN_PLATFORM_WIN
		case 219:
#else      
		case '[':
#endif
        {
			if (m_cameraZoomDestination > 0.25f) {
				m_cameraZoomSource = m_camera.getZoom();
				m_cameraZoomDestination *= 0.5f;
				m_cameraZoomFactor = 0.f;
				char buf[16];
				sprintf(buf, "zoom %u%%", (vn::u32)(m_cameraZoomDestination * 100.f));
				VN_LOG_INFO(buf);
			}
            break;
        }
            
        case '0': {
            m_gridLayer->setVisible(!m_gridLayer->isVisible());
            break;
        }
            
        case 'O':
        case 'o': {
            bool flag = !m_cornerLayer[0]->isVisible();
            for (int i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
                m_cornerLayer[i]->setVisible(flag);
            }
            break;
        }
            
        case 'P':
        case 'p': {
            m_cliffLayer->setVisible(!m_cliffLayer->isVisible());
            break;
        }
    
        case 'Q':
        case 'q':
            if (!repeat) {
                _setBrush(kBrush_MoveCamera);
            }
            break;
            
        case 'W':
        case 'w':
            if (!repeat) {
                _setBrush(kBrush_BlockType);
            }
            break;
            
        case 'E':
        case 'e':
            if (!repeat) {
                _setBrush(kBrush_ObjectNew);
            }
            break;
            
        case 'R':
        case 'r':
            if (!repeat) {
                _setBrush(kBrush_ObjectEdit);
            }
            break;
            
        case 'T':
        case 't':
            if (!repeat) {
                _setBrush(kBrush_Decoration);
            }
            break;
            
        case 'U':
        case 'u':
            if (!repeat) {
                _setBrush(kBrush_Collision);
            }
            break;
            
        case 'I':
        case 'i':
            if (!repeat) {
                _setBrush(kBrush_SSO);
            }
            break;
            
#if VN_PLATFORM == VN_PLATFORM_WIN
		case 192:
#else
        case '`':
#endif
		{
            if (!repeat) {
                if (m_currentBrush != m_brushes[kBrush_Collision].ptr()) {
                    m_collisionLayer->setVisible(!m_collisionLayer->isVisible());
                }
            }
            break;
        }
            
        default:
            m_currentBrush->onKeyDown(chr, repeat);
            break;
    }
}

void EditorApp::onRootKeyUp(vn::c32 chr) {
    
}

void EditorApp::onRootFocusIn() {
    //VN_LOG_DEBUG("root_focus_in");
}

void EditorApp::onRootFocusOut() {
    //VN_LOG_DEBUG("root_focus_out");
}

Camera & EditorApp::camera() {
    return m_camera;
}

void EditorApp::_setBrush(BrushName name) {
    if (m_mouseDown) {
        return ;
    }
    Brush *brush = m_brushes[name].ptr();
    vnassert(brush);
    if (brush == m_currentBrush) {
        return ;
    }
    m_currentBrush->switchOut();
    m_currentBrush = brush;
    m_currentBrush->switchIn();
    m_brushList->setSelectedItem(m_brushList->item(name));
}

SceneNode * EditorApp::layer_brush() {
    return m_brushLayer.ptr();
}
            
SceneNode * EditorApp::layer_object_land() {
    return m_landObjectLayer.ptr();
}

SceneNode * EditorApp::layer_object_field() {
    return m_fieldObjectLayer.ptr();
}

SceneNode * EditorApp::layer_collision() {
    return m_collisionLayer.ptr();
}

bool EditorApp::setBlockType(const vn::vector2i &index, MapBlockType type, MapBlockType *oldType) {
    MapBlock *block = m_mapData.block(index);
    if (!block) {
        return false;
    }
    if (block->type == type) {
        return false;
    }
    if (oldType) {
        *oldType = block->type;
    }
    block->type = type;
    MapBlockNode *node = m_blockNodeIndices[block];
    MapBlock *upBlock = m_mapData.block(index - vn::vector2i(0, 1));
    MapBlock *downBlock = m_mapData.block(index + vn::vector2i(0, 1));
    if (isMapBlockField(block->type)) {
        block->cliff = false;
        m_fieldBlockLayer[block->type - kMBT_Field_Red]->addChild(node);
        if (downBlock && !isMapBlockField(downBlock->type) && !downBlock->cliff) {
            downBlock->cliff = true;
            m_blockNodeIndices[downBlock]->updateBlock();
            _updateBlockCornerAndCliff(downBlock->position + vn::vector2i(1, 1));
            _updateBlockCornerAndCliff(downBlock->position + vn::vector2i(0, 1));
        }
    } else {
        if (upBlock) {
            if (isMapBlockField(upBlock->type)) {
                block->cliff = true;
            } else {
                _updateBlockCornerAndCliff(upBlock->position);
                _updateBlockCornerAndCliff(upBlock->position + vn::vector2i(1, 0));
            }
        }
        if (downBlock && downBlock->cliff) {
            downBlock->cliff = false;
            m_blockNodeIndices[downBlock]->updateBlock();
            _updateBlockCornerAndCliff(downBlock->position + vn::vector2i(1, 1));
            _updateBlockCornerAndCliff(downBlock->position + vn::vector2i(0, 1));
        }
        if (block->type == kMBT_Water || block->type == kMBT_DeepWater) {
            m_waterBlockLayer->addChild(node);
        } else {
            m_landBlockLayer->addChild(node);
        }
    }
    node->updateBlock();
    _updateBlockCornerAndCliff(index);
    _updateBlockCornerAndCliff(index + vn::vector2i(1, 0));
    _updateBlockCornerAndCliff(index + vn::vector2i(1, 1));
    _updateBlockCornerAndCliff(index + vn::vector2i(0, 1));
    
    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            m_mapCollision.update(m_mapData, index + vn::vector2i(x, y));
        }
    }
    
    return true;
}

vn::UIElement * EditorApp::uiRootElement() {
    return m_uiRootElement;
}

vn::CommandList & EditorApp::commandList() {
	return m_commandList;
}
            
void EditorApp::addMapObject(MapObject *mobj, bool grab) {
    if (grab) {
        mobj->grab();
    }
    m_objects.push_back(mobj);
    m_mapData.addObjectDesc(mobj->desc());
    m_objectSpace.place(mobj->getPosition(), mobj->size(), mobj);
    mobj->addNodeToScene();
}
            
void EditorApp::removeMapObject(MapObject *mobj) {
    for (MapObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
        if (*it == mobj) {
            m_objectSpace.place(mobj->getPosition(), mobj->size(), 0);
            mobj->removeNodeFromScene();
            m_mapData.removeObjectDesc(mobj->desc());
            m_currentBrush->onMapObjectRemoved(mobj);
            mobj->drop();
			m_objects.erase(it);
			break;
        }
    }
}

void EditorApp::setMapObjectPosition(MapObject *mobj, const vn::vector2i &pos) {
    m_objectSpace.place(mobj->getPosition(), mobj->size(), 0);
    mobj->setPosition(pos);
    m_objectSpace.place(pos, mobj->size(), mobj);
}
			
bool EditorApp::isSpaceEmpty(const vn::vector2i &pos, const vn::vector2i &size) {
	return m_objectSpace.isEmpty(pos, size);
}

MapObject * EditorApp::pickMapObjectByViewPos(const vn::vector2f &pos) {
    return m_objectSpace.pick(m_camera.convertFromView(pos) / 40.f);
}

MapDecoration * EditorApp::createDecorationImage(const vn::vector2f &pos, const vn::str8 &imagePath, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color) {
    MapDecorationImage *image = vnnew MapDecorationImage();
    image->setDesc(vnnew MapDecorationDesc(), false);
    image->setPosition(pos);
    image->setExtent(extent);
    image->setFlipX(flipX);
    image->setFlipY(flipY);
    image->setColor(color);
    image->setLightMode(lightMode);
    image->setImagePath(imagePath);
    addDecoration(image, true);
    return image;
}

MapDecoration * EditorApp::createDecorationAnimation(const vn::vector2f &pos, const vn::str8 &animationPath, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color) {
    MapDecorationAnimation *anim = vnnew MapDecorationAnimation();
    MapDecorationDesc *desc = vnnew MapDecorationDesc();
    desc->type = MapDecorationDesc::kAnimation;
    anim->setDesc(desc, false);
    anim->setPosition(pos);
    anim->setExtent(extent);
    anim->setFlipX(flipX);
    anim->setFlipY(flipY);
    anim->setColor(color);
    anim->setLightMode(lightMode);
    anim->setAnimationPath(animationPath);
    addDecoration(anim, true);
    return anim;
}

void EditorApp::addDecoration(MapDecoration *deco, bool grab) {
    m_decorationLayer->addChild(deco, grab);
    m_decorations.push_back(deco);
    m_mapData.addDecoration(deco->getDesc());
}

MapDecoration * EditorApp::pickDecoration(const vn::vector2f &pos) {
    for (MapDecorationArray::reverse_iterator it = m_decorations.rbegin(); it != m_decorations.rend(); ++it) {
        if ((*it)->contains(pos)) {
            return *it;
        }
    }
    return 0;
}

void EditorApp::removeDecoration(MapDecoration *deco) {
    for (MapDecorationArray::iterator it = m_decorations.begin(); it != m_decorations.end(); ++it) {
        if (*it == deco) {
            m_mapData.removeDecoration(deco->getDesc());
            for (BrushMap::iterator it = m_brushes.begin(); it != m_brushes.end(); ++it) {
                it->second->onMapDecorationRemoved(deco);
            }
            m_decorations.erase(it);
            m_decorationLayer->removeChild(deco);
            break;
        }
    }
}

void EditorApp::addSSO(const MapSSOPtr &sso) {
    m_ssoLayer->addChild(sso.ptr());
    m_mapData.addStateScriptObject(sso->desc());
}

void EditorApp::removeSSO(const MapSSOPtr &sso) {
    m_currentBrush->onMapSSORemoved(sso);
    m_mapData.removeStateScriptObject(sso->desc());
    m_ssoLayer->removeChild(sso.ptr());
}

MapSSOPtr EditorApp::pickSSO(const vn::vector2f &pos) {
    for (size_t i = 0, c = m_ssoLayer->numChildren(); i < c; ++i) {
        MapSSO *sso = (MapSSO *)m_ssoLayer->child(i);
        if (sso->contains(pos)) {
            sso->grab();
            return sso;
        }
    }
    return MapSSOPtr();
}

MapData & EditorApp::mapData() {
    return m_mapData;
}

MapCollision & EditorApp::mapCollision() {
    return m_mapCollision;
}

void EditorApp::_updateBlockCornerAndCliff(const vn::vector2i &index)  {
    if (index == vn::vector2i::kZero || index == m_mapData.size()) {
        return ;
    }
    if (index.x == m_mapData.size().x && index.y == 0) {
        return ;
    }
    if (index.y == m_mapData.size().y && index.x == 0) {
        return ;
    }
    const MapBlock *block[4] = {
        m_mapData.block(index - vn::vector2i(1, 1)),
        m_mapData.block(index - vn::vector2i(0, 1)),
        m_mapData.block(index),
        m_mapData.block(index - vn::vector2i(1, 0))
    };
    if (!block[0] && !block[1]) {
        block[0] = block[3];
        block[1] = block[2];
    } else if (!block[0] && !block[3]) {
        block[0] = block[1];
        block[3] = block[2];
    } else if (!block[1] && !block[2]) {
        block[1] = block[0];
        block[2] = block[3];
    } else if (!block[2] && !block[3]) {
        block[2] = block[1];
        block[3] = block[0];
    }

    _updateBlockWaterCorner(index, block);
    _updateBlockFieldCorner(index, block);
    _updateBlockCliff(index, block);

}

void EditorApp::_updateBlockWaterCorner(const vn::vector2i &index, const MapBlock *block[4]) {
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if (block[i]->type == kMBT_Land && !block[i]->cliff) {
            flag = true;
            break;
        }
    }
    MapBlockCornerType type(0, block[0]->isWater(), block[1]->isWater(), block[2]->isWater(), block[3]->isWater());
    MapBlockCornerMap *corners = &m_mapData.corners(0);
    if (!flag || !type.valid()) {
        MapBlockCornerMap::iterator it = corners->find(index);
        if (it != corners->end()) {
            MapBlockCornerNodeMap::iterator nit = m_blockCornerNodeIndices[0].find(&it->second);
            if (nit != m_blockCornerNodeIndices[0].end()) {
                m_cornerLayer[0]->removeChild(nit->second);
                m_blockCornerNodeIndices[0].erase(nit);
            }
            corners->erase(it);
        }
    } else {
        MapBlockCorner &corner = (*corners)[index];
        corner.type = type;
        corner.position = index;
        MapBlockCornerNode *&node = m_blockCornerNodeIndices[0].insert(std::make_pair(&corner, (MapBlockCornerNode *)0)).first->second;
        if (node) {
            node->updateCorner();
        } else {
            node = vnnew MapBlockCornerNode(&corner);
            m_cornerLayer[0]->addChild(node, false);
        }
    }
    
    type = MapBlockCornerType(4, block[0]->type == kMBT_DeepWater, block[1]->type == kMBT_DeepWater, block[2]->type == kMBT_DeepWater, block[3]->type == kMBT_DeepWater);
    corners = &m_mapData.corners(4);
    if (!type.valid()) {
        MapBlockCornerMap::iterator it = corners->find(index);
        if (it != corners->end()) {
            MapBlockCornerNodeMap::iterator nit = m_blockCornerNodeIndices[4].find(&it->second);
            if (nit != m_blockCornerNodeIndices[4].end()) {
                m_cornerLayer[4]->removeChild(nit->second);
                m_blockCornerNodeIndices[4].erase(nit);
            }
            corners->erase(it);
        }
    } else {
        MapBlockCorner &corner = (*corners)[index];
        corner.type = type;
        corner.position = index;
        MapBlockCornerNode *&node = m_blockCornerNodeIndices[4].insert(std::make_pair(&corner, (MapBlockCornerNode *)0)).first->second;
        if (node) {
            node->updateCorner();
        } else {
            node = vnnew MapBlockCornerNode(&corner);
            m_cornerLayer[4]->addChild(node, false);
        }
    }
}


void EditorApp::_updateBlockCliff(const vn::vector2i &index, const MapBlock *block[4]) {
    MapBlockCliffType type;
    type.block0 = isMapBlockField(block[0]->type);
    type.block1 = isMapBlockField(block[1]->type);
    type.block2 = isMapBlockField(block[2]->type);
    type.block3 = isMapBlockField(block[3]->type);
    
    if (!type.valid()) {
        MapBlockCliffMap::iterator it = m_mapData.cliffs().find(index);
        if (it != m_mapData.cliffs().end()) {
            MapBlockCliffNodeMap::iterator nit = m_blockCliffNodeIndices.find(&it->second);
            if (nit != m_blockCliffNodeIndices.end()) {
                m_cliffLayer->removeChild(nit->second);
                m_blockCliffNodeIndices.erase(nit);
            }
            m_mapData.cliffs().erase(it);
        }
    } else {
        type.block2 = type.block3 = 0;
        const MapBlock *p = m_mapData.block(block[3]->position + vn::vector2i(0, 1));
        if (!p) {
            p = block[3];
        }
        if (p->isWater()) {
            type.type |= 1;
        }
        p = m_mapData.block(block[2]->position + vn::vector2i(0, 1));
        if (!p) {
            p = block[2];
        }

        if (p->isWater()) {
            type.type |= 2;
        }
        MapBlockCliff &cliff = m_mapData.cliffs()[index];
        cliff.type = type;
        cliff.position = index;
        MapBlockCliffNode *&node = m_blockCliffNodeIndices.insert(std::make_pair(&cliff, (MapBlockCliffNode *)0)).first->second;
        if (node) {
            node->updateCliff();
        } else {
            node = vnnew MapBlockCliffNode(&cliff);
            m_cliffLayer->addChild(node, false);
        }
    }
}

void EditorApp::_updateBlockFieldCorner(const vn::vector2i &index, const MapBlock *block[4]) {
    MapBlockType bt[3] = {kMBT_Field_Red, kMBT_Field_Green, kMBT_Field_White};
    MapBlockCornerType type;
    for (size_t i = 0; i < 3; ++i) {
        MapBlockCornerMap &corners = m_mapData.corners(i + 1);
        MapBlockCornerNodeMap &indices = m_blockCornerNodeIndices[i + 1];
        SceneNode *layer = m_cornerLayer[i + 1].ptr();
        type.value = (vn::u8)_buildBlockFiledCornerType(block, bt[i]);
        if (!type.valid()) {
            MapBlockCornerMap::iterator it = corners.find(index);
            if (it != corners.end()) {
                MapBlockCornerNodeMap::iterator nit = indices.find(&it->second);
                if (nit != indices.end()) {
                    layer->removeChild(nit->second);
                    indices.erase(nit);
                }
                corners.erase(it);
            }
        } else {
            type.type = i + 1;
            MapBlockCorner &corner = corners[index];
            corner.type = type;
            corner.position = index;
            MapBlockCornerNode *&node = indices.insert(std::make_pair(&corner, (MapBlockCornerNode *)0)).first->second;
            if (node) {
                node->updateCorner();
            } else {
                node = vnnew MapBlockCornerNode(&corner);
                layer->addChild(node, false);
            }
        }
    }
    
}

vn::u32 EditorApp::_buildBlockFiledCornerType(const MapBlock *block[4], MapBlockType blockType) {
    bool flag = false;
    for (int i = 0; i < 4; ++i) {
        if (block[i]->type == blockType) {
            flag = true;
            break;
        }
    }
    vn::u32 type = 0;
    if (flag) {
        for (int i = 0; i < 4; ++i) {
            if (block[i]->type == blockType) {
                type |= 1 << i;
            }
        }
    }
    return type;
}

void EditorApp::_onBrushListItemClicked(vn::UIElement *, vn::UIListItem *item) {
    if (!item) {
        return ;
    }
    _setBrush((BrushName)m_brushList->itemIndex(item));
}

void EditorApp::_onNew(vn::UIElement *) {
    if (m_uiNewMapRoot) {
        m_uiNewMapRoot->bringToTop();
        m_uiNewMapRoot->setVisible(true);
        vn::UIRoot::instance().requireFocus(m_uiNewMapHeight);
        m_uiNewMapHeight->selectAll();
    } else {
        _newMap(29, 23);
    }
}

void EditorApp::_onOpen(vn::UIElement *) {
    m_fileBrowser->setTitle("Open TDM File");
    m_fileBrowser->setExtensionFilter(".tdm");
    m_fileBrowser->setReadOnly(true);
    m_fileBrowser->setFileName("");
    m_fileBrowser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onFileOpen));
    m_fileBrowser->show(m_uiRootElement);
}

void EditorApp::_onSave(vn::UIElement *) {
    if (m_fileName.empty()) {
        _onSaveAs(0);
    } else {
        _saveToFile();
    }
}

void EditorApp::_onSaveAs(vn::UIElement *) {
    m_fileBrowser->setTitle("Save TDM File As");
    m_fileBrowser->setExtensionFilter(".tdm");
    m_fileBrowser->setReadOnly(false);
    m_fileBrowser->setFileName(m_fileName);
    m_fileBrowser->bindAction_FileOpen(vn::make_memfn(this, &EditorApp::_onFileSave));
    m_fileBrowser->show(m_uiRootElement);
}

void EditorApp::_onRefresh(vn::UIElement *) {
    m_blockNodeIndices.clear();
    m_landBlockLayer->clearChildren();
    m_waterBlockLayer->clearChildren();
    for (int i = 0; i < 3; ++i) {
        m_fieldBlockLayer[i]->clearChildren();
    }
    
    m_cliffLayer->clearChildren();
    m_blockCliffNodeIndices.clear();
    
    for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
        m_cornerLayer[i]->clearChildren();
        m_blockCornerNodeIndices[i].clear();
    }
    
    vn::TextureManager::instance().freeUnusedTextures();
    
    for (int i = 0; i < m_mapData.size().y; ++i) {
        for (int j = 0; j < m_mapData.size().x; ++j) {
            vn::vector2i index(j, i);
            MapBlock *block = m_mapData.block(vn::vector2i(j, i));
            MapBlock *downBlock = m_mapData.block(index + vn::vector2i(0, 1));
            MapBlockNode *node = vnnew MapBlockNode(block);
            if (isMapBlockField(block->type)) {
                block->cliff = false;
                m_fieldBlockLayer[block->type - kMBT_Field_Red]->addChild(node, false);
                if (downBlock && !isMapBlockField(downBlock->type) && !downBlock->cliff) {
                    downBlock->cliff = true;
                }
            } else {
                if (downBlock && downBlock->cliff) {
                    downBlock->cliff = false;
                }
                if (block->type == kMBT_Water || block->type == kMBT_DeepWater) {
                    m_waterBlockLayer->addChild(node, false);
                } else {
                    m_landBlockLayer->addChild(node, false);
                }
            }
            m_blockNodeIndices[block] = node;
            
            _updateBlockCornerAndCliff(index);
            _updateBlockCornerAndCliff(index + vn::vector2i(1, 0));
            _updateBlockCornerAndCliff(index + vn::vector2i(1, 1));
            _updateBlockCornerAndCliff(index + vn::vector2i(0, 1));
            
        }
    }
}

void EditorApp::_onNewMapText(vn::UIElement *, const vn::str32 &) {
    _onNewMapOk(0);
}

void EditorApp::_onNewMapOk(vn::UIElement *) {
    vn::str8 txt;
    vn::Unicode::convert(m_uiNewMapHeight->getText().c_str(), m_uiNewMapHeight->getText().length(), txt);
    vn::u32 height;
    if (!sscanf(txt.c_str(), "%u", &height)) {
        return ;
    }
    if (height < 23) {
        return ;
    }
    vn::UIRoot::instance().releaseFocus(m_uiNewMapHeight);
    _newMap(29, height);
    m_uiNewMapRoot->setVisible(false);
}

void EditorApp::_onNewMapCancel(vn::UIElement *) {
    vn::UIRoot::instance().releaseFocus(m_uiNewMapHeight);
    m_uiNewMapRoot->setVisible(false);
}

void EditorApp::_onFileOpen(const vn::FilePath &path, const vn::str8 &name) {
    if (name.empty()) {
        return ;
    }
    m_filePath = path;
    m_fileName = name;
    _loadFromFile();
}

void EditorApp::_onFileSave(const vn::FilePath &path, const vn::str8 &name) {
    if (name.empty()) {
        return ;
    }
    m_filePath = path;
    m_fileName = name;
    if (m_fileName.length() >= 4) {
        if (strcmp(m_fileName.c_str() + m_fileName.length() - 4, ".tdm")) {
            m_fileName += ".tdm";
        }
    } else {
        m_fileName += ".tdm";
    }
    _saveToFile();
}

void EditorApp::_newMap(vn::u32 width, vn::u32 height) {
    vnassert(width && height);
    _setBrush(kBrush_MoveCamera);
    m_commandList.clear();
    m_blockNodeIndices.clear();
    m_landBlockLayer->clearChildren();
    m_waterBlockLayer->clearChildren();
    for (int i = 0; i < 3; ++i) {
        m_fieldBlockLayer[i]->clearChildren();
    }
    
    m_landObjectLayer->clearChildren();
    m_fieldObjectLayer->clearChildren();
    m_gridLayer->clearChildren();
    
    m_cliffLayer->clearChildren();
    m_blockCliffNodeIndices.clear();
    
    for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
        m_cornerLayer[i]->clearChildren();
        m_blockCornerNodeIndices[i].clear();
    }
    
    m_decorations.clear();
    m_decorationLayer->clearChildren();
    
    m_ssoLayer->clearChildren();
    
    m_mapData.fini();
    for (MapObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
        (*it)->drop();
    }
    m_objects.clear();
    
    m_fileName.clear();
    
    m_mapData.init(width, height);
    for (int i = 0; i < m_mapData.size().y; ++i) {
        for (int j = 0; j < m_mapData.size().x; ++j) {
            MapBlock *block = m_mapData.block(vn::vector2i(j, i));
            MapBlockNode *node = vnnew MapBlockNode(block);
            if (isMapBlockField(block->type)) {
                m_fieldBlockLayer[block->type - kMBT_Field_Red]->addChild(node, false);
            } else if (block->type == kMBT_Water || block->type == kMBT_DeepWater) {
                m_waterBlockLayer->addChild(node, false);
            } else {
                m_landBlockLayer->addChild(node, false);
            }
            m_blockNodeIndices[block] = node;
            m_gridLayer->addChild(vnnew MapGridNode(block->position), false);
        }
    }
    
    m_objectSpace.reset(m_mapData.size());
    m_mapCollision.reset(m_mapData);
}

void EditorApp::_saveToFile() {
    vn::FilePath path = m_filePath;
    path.fileName += m_fileName;
    vn::FileStream *fs = vn::FileManager::instance().create(path);
    if (!fs) {
        VN_LOG_ERROR("open file \"" << path.getFullPath() << "\" failed!");
        m_fileName.clear();
        return ;
    }
    try {
        *fs << vn::DataStream::cbuf("TDM", 4);
        m_mapData.save(*fs);
    } catch (vn::DataStream::Exception &) {
        VN_LOG_ERROR("write file \"" << path.getFullPath() << "\" failed!");
        fs->close();
        m_fileName.clear();
        return ;
    }
    fs->close();
    VN_LOG_INFO("save to file \"" << path.getFullPath() << "\"");
}

void EditorApp::_loadFromFile() {
    vn::FilePath path = m_filePath;
    path.fileName += m_fileName;
    vn::FileStream *fs = vn::FileManager::instance().open(path);
    if (!fs) {
        VN_LOG_ERROR("open file \"" << path.getFullPath() << "\" failed!");
        m_fileName.clear();
        return ;
    }
    try {
        vn::c8 sign[4];
        *fs >> vn::DataStream::buf(sign, 4);
        if (strcmp(sign, "TDM")) {
            VN_LOG_ERROR("file \"" << path.getFullPath() << "\" format invalid!");
            fs->close();
            m_fileName.clear();
            return ;
        }
        _setBrush(kBrush_MoveCamera);
        m_commandList.clear();
        m_blockNodeIndices.clear();
        for (int i = 0; i < 3; ++i) {
            m_fieldBlockLayer[i]->clearChildren();
        }
        m_landBlockLayer->clearChildren();
        m_waterBlockLayer->clearChildren();
        m_landObjectLayer->clearChildren();
        m_fieldObjectLayer->clearChildren();
        m_gridLayer->clearChildren();
        
        m_cliffLayer->clearChildren();
        m_blockCliffNodeIndices.clear();
        
        for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
            m_cornerLayer[i]->clearChildren();
            m_blockCornerNodeIndices[i].clear();
        }
        
        for (MapObjects::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
            (*it)->drop();
        }
        m_objects.clear();
        
        m_decorations.clear();
        m_decorationLayer->clearChildren();
        m_ssoLayer->clearChildren();
        
        m_mapData.load(*fs);
        m_objectSpace.reset(m_mapData.size());
        m_mapCollision.reset(m_mapData);
        for (size_t i = 0, c = m_mapData.objectDescCount(); i < c; ++i) {
            MapObject *obj = MapObjectFactory::instance().create(m_mapData.objectDesc(i));
            m_objects.push_back(obj);
            m_objectSpace.place(obj->getPosition(), obj->size(), obj);
            obj->addNodeToScene();
        }
    } catch (vn::DataStream::Exception &) {
        VN_LOG_ERROR("read file \"" << path.getFullPath() << "\" failed!");
        fs->close();
        m_fileName.clear();
        return ;
    }
    fs->close();
    
    for (int y = 0; y < m_mapData.size().y; ++y) {
        for (int x = 0; x < m_mapData.size().x; ++x) {
            MapBlock *block = m_mapData.block(vn::vector2i(x, y));
            MapBlockNode *node = vnnew MapBlockNode(block);
            m_blockNodeIndices[block] = node;
            if (isMapBlockField(block->type)) {
                m_fieldBlockLayer[block->type - kMBT_Field_Red]->addChild(node, false);
            } else if (block->type == kMBT_Water || block->type == kMBT_DeepWater) {
                m_waterBlockLayer->addChild(node, false);
            } else {
                m_landBlockLayer->addChild(node, false);
            }
            m_gridLayer->addChild(vnnew MapGridNode(block->position), false);
        }
    }
    for (size_t i = 0; i < MapBlockCornerType::kTypeCount; ++i) {
        MapBlockCornerMap &corners = m_mapData.corners(i);
        MapBlockCornerNodeMap &indices = m_blockCornerNodeIndices[i];
        SceneNode *layer = m_cornerLayer[i].ptr();
        for (MapBlockCornerMap::iterator it = corners.begin(); it != corners.end(); ++it) {
            MapBlockCornerNode *node = vnnew MapBlockCornerNode(&it->second);
            indices[&it->second] = node;
            layer->addChild(node, false);
        }
    }
    
    for (MapBlockCliffMap::iterator it = m_mapData.cliffs().begin(); it != m_mapData.cliffs().end(); ++it) {
        MapBlockCliffNode *node = vnnew MapBlockCliffNode(&it->second);
        m_blockCliffNodeIndices[&it->second] = node;
        m_cliffLayer->addChild(node, false);
    }
    
    for (size_t i = 0; i < m_mapData.numDecorations(); ++i) {
        const MapDecorationDescPtr &desc = m_mapData.decoration(i);
        if (desc->type == MapDecorationDesc::kImage) {
            MapDecorationImage *image = vnnew MapDecorationImage();
            image->setDesc(desc);
            m_decorationLayer->addChild(image, false);
            m_decorations.push_back(image);
        } else if (desc->type == MapDecorationDesc::kAnimation) {
            MapDecorationAnimation *anim = vnnew MapDecorationAnimation();
            anim->setDesc(desc);
            m_decorationLayer->addChild(anim, false);
            m_decorations.push_back(anim);
        }
    }
    
    for (size_t i = 0; i < m_mapData.numStateScriptObjects();) {
        MapSSO *sso = vnnew MapSSO(m_mapData.stateScriptObject(i));
        if (!sso->init()) {
            m_mapData.removeStateScriptObject(m_mapData.stateScriptObject(i));
            delete sso;
        } else {
            m_ssoLayer->addChild(sso, false);
            ++i;
        }
    }
    
    VN_LOG_INFO("load from file \"" << path.getFullPath() << "\"");
}

void EditorApp::_loadConfigFile() {
    vn::TreeDataScript tds;
    vn::TreeDataNode *root = tds.parse(vn::FilePath(0, "config.tdl"));
    vnassert(root);
    if (!root) {
        return ;
    }
    for (vn::TreeDataNode *p = root->firstChild(); p; p = p->nextNeighbour()) {
        if (p->name() == "FileSystem") {
            vn::u32 fsId = p->queryAttributeInteger("id", -1);
            vn::str8 path;
            if (fsId != -1 && p->queryAttributeString("path", path)) {
                vn::FileManager::instance().setFileSystem(fsId, vnnew vn::FileSystemOS(path), false);
            }
        }
    }
    root->drop();
}