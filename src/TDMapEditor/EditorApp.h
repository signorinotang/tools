//
//  EditorApp.h
//  TDMapEditor
//
//  Created by Wayne on 13-12-26.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__EditorApp__
#define __TDMapEditor__EditorApp__

#include "../common/GfxAppDelegate.h"
#include "../common/UIFileBrowser.h"

#include "Camera.h"
#include "MapBlockNode.h"
#include "MapBlockCornerNode.h"
#include "MapBlockCliffNode.h"

#include "Brush.h"
#include "MapObject.h"
#include "MapDecoration.h"
#include "MapCollision.h"
#include "MapSSO.h"

#include "vnLog.h"
#include "vnCommandList.h"

class EditorApp
: public GfxAppDelegate
, public vn::Singleton<EditorApp> {
public:
    EditorApp();
  
	virtual void do_undo();
	virtual void do_redo();
    
    void onRootMouseLeftDown(const vn::vector2f &pt);
    void onRootMouseLeftUp(const vn::vector2f &pt);
    void onRootMouseRightDown(const vn::vector2f &pt);
    void onRootMouseRightUp(const vn::vector2f &pt);
    void onRootMouseCaptureCancelled();
    void onRootKeyDown(vn::c32 chr, bool repeat);
    void onRootKeyUp(vn::c32 chr);
    void onRootFocusIn();
    void onRootFocusOut();
    
    Camera & camera();
    SceneNode * layer_brush();
    SceneNode * layer_object_land();
    SceneNode * layer_object_field();
    SceneNode * layer_collision();
    
    bool setBlockType(const vn::vector2i &index, MapBlockType type, MapBlockType *oldType = 0);
    
    vn::UIElement * uiRootElement();
    
	vn::CommandList & commandList();
	
    void addMapObject(MapObject *mobj, bool grab = true);
    void removeMapObject(MapObject *mobj);
    void setMapObjectPosition(MapObject *mobj, const vn::vector2i &pos);
    bool isSpaceEmpty(const vn::vector2i &pos, const vn::vector2i &size);
    MapObject * pickMapObjectByViewPos(const vn::vector2f &pos);
    
    MapDecoration * createDecorationImage(const vn::vector2f &pos, const vn::str8 &imagePath, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color);
    
    MapDecoration * createDecorationAnimation(const vn::vector2f &pos, const vn::str8 &animationPath, bool flipX, bool flipY, bool lightMode, const vn::vector2f &extent, vn::color_t color);
    
    void addDecoration(MapDecoration *deco, bool grab);
    MapDecoration * pickDecoration(const vn::vector2f &pos);
    void removeDecoration(MapDecoration *deco);
    
    MapData & mapData();
    MapCollision & mapCollision();
    
    void addSSO(const MapSSOPtr &sso);
    void removeSSO(const MapSSOPtr &sso);
    MapSSOPtr pickSSO(const vn::vector2f &pos);
protected:
    virtual vn::UIElement * _createRootElement();
    virtual void _init(const vn::vector2f &size);
    virtual void _fini();
    virtual void _update(vn::f32 deltaTime);
    virtual void _render();
    virtual void _resize(const vn::vector2f &size);
    
    
    Camera m_camera;
	vn::f32 m_cameraZoomSource;
	vn::f32 m_cameraZoomDestination;
	vn::f32 m_cameraZoomFactor;
    
    SceneNodePtr m_fieldBlockLayer[3];
    SceneNodePtr m_landBlockLayer;
    SceneNodePtr m_waterBlockLayer;
    SceneNodePtr m_cliffLayer;
    SceneNodePtr m_cornerLayer[MapBlockCornerType::kTypeCount];
    
    SceneNodePtr m_collisionLayer;
    SceneNodePtr m_gridLayer;
    SceneNodePtr m_brushLayer;
    SceneNodePtr m_landObjectLayer;
    SceneNodePtr m_fieldObjectLayer;
    SceneNodePtr m_decorationLayer;
    SceneNodePtr m_ssoLayer;
    MapData m_mapData;
    MapCollision m_mapCollision;
    MapBlockNodeMap m_blockNodeIndices;
    
    MapBlockCliffNodeMap m_blockCliffNodeIndices;
    MapBlockCornerNodeMap m_blockCornerNodeIndices[MapBlockCornerType::kTypeCount];
    
    std::vector<SceneNode *> m_visibleNodes;
    
    bool m_mouseDown;
    
    typedef std::map<BrushName, BrushPtr> BrushMap;
    BrushMap m_brushes;
    Brush *m_currentBrush;
    
    void _setBrush(BrushName name);
    
    void _updateBlockCornerAndCliff(const vn::vector2i &index);
    
    void _updateBlockCliff(const vn::vector2i &index, const MapBlock *block[4]);
    void _updateBlockFieldCorner(const vn::vector2i &index, const MapBlock *block[4]);
    void _updateBlockWaterCorner(const vn::vector2i &index, const MapBlock *block[4]);
    vn::u32 _buildBlockFiledCornerType(const MapBlock *block[4], MapBlockType blockType);
    
    vn::UIList *m_brushList;
    void _onBrushListItemClicked(vn::UIElement *, vn::UIListItem *item);
    
    void _onNew(vn::UIElement *);
    void _onOpen(vn::UIElement *);
    void _onSave(vn::UIElement *);
    void _onSaveAs(vn::UIElement *);
    void _onRefresh(vn::UIElement *);
    
    void _onNewMapText(vn::UIElement *, const vn::str32 &);
    void _onNewMapOk(vn::UIElement *);
    void _onNewMapCancel(vn::UIElement *);
    
    UIFileBrowserPtr m_fileBrowser;
    vn::FilePath m_filePath;
    vn::str8 m_fileName;
    void _onFileOpen(const vn::FilePath &path, const vn::str8 &name);
    void _onFileSave(const vn::FilePath &path, const vn::str8 &name);
    
    void _newMap(vn::u32 width, vn::u32 height);
    void _saveToFile();
    void _loadFromFile();
    void _loadConfigFile();
    
    
  	vn::CommandList m_commandList;
    MapObjectSpace m_objectSpace;
    typedef std::vector<MapObject *> MapObjects;
    MapObjects m_objects;
    
    vn::UIElement *m_uiNewMapRoot;
    vn::UITextField *m_uiNewMapHeight;
    
    typedef std::vector<MapDecoration *> MapDecorationArray;
    MapDecorationArray m_decorations;
    
};

#endif /* defined(__TDMapEditor__EditorApp__) */
