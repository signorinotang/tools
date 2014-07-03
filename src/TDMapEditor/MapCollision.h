//
//  MapCollision.h
//  TDMapEditor
//
//  Created by Wayne on 4/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapCollision__
#define __TDMapEditor__MapCollision__

#include "MapData.h"
#include "SceneNode.h"

enum CollisionType {
    kCT_Invalid = 0,
    kCT_Land = 1,
    kCT_Water = 2
};

class MapCollisionItemNode;

typedef vn::RefCountedPtr<MapCollisionItemNode> MapCollisionItemNodePtr;

class MapCollision {
public:
    
    
    struct Item : public vn::AllocatedObject {
        vn::vector2i index;
        CollisionType type;
        MapCollisionItemNodePtr node;
        int objects;
        
        struct {
            vn::u32 pass;
            size_t open;
            Item *parent;
            int f_cost, g_cost, h_cost;
        } path;
        
        Item(const vn::vector2i &idx, CollisionType t);
        ~Item();
    };
    
    MapCollision();
    ~MapCollision();
    
    void reset(MapData &mapData);
    void update(MapData &mapData, const vn::vector2i &index);
    void incrementObject(const vn::vector2i &index);
    void decrementObject(MapData &mapData, const vn::vector2i &index);
    void clearPath();
    
    CollisionType itemType(const vn::vector2i &index);
    Item * item(const vn::vector2i &index);
    
    bool findPath(const vn::vector2i &start, const vn::vector2i &destination, vn::u32 collisionMask, std::vector<const Item *> &result, int &cost);
    
protected:
    std::vector<Item *> m_items;
    vn::vector2i m_size;
    vn::u32 m_pathFindingPass;
    
    CollisionType _build(MapData &mapData, const vn::vector2i &index);
    Item * _item(const vn::vector2i &index) {
        return m_items[index.y * m_size.x + index.x];
    }
    
    size_t _adjacentItems(const vn::vector2i &index, vn::u32 collisionMask, Item *blocks[], int weights[]);
    bool _findPath(Item *src, Item *dest, vn::u32 collisionMask, std::vector<const Item *> &result, int &cost);
};

class MapCollisionItemNode : public SceneNode {
public:
    MapCollisionItemNode();
    void init(MapCollision::Item *item);
    void itemUpdated();
    void set(bool value);
private:
    MapCollision::Item *m_item;
    SceneNode *m_node;
};


#endif /* defined(__TDMapEditor__MapCollision__) */
