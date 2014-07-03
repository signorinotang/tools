//
//  MapObject.h
//  TDMapEditor
//
//  Created by Wayne on 14-1-7.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __TDMapEditor__MapObject__
#define __TDMapEditor__MapObject__

#include "vnEditValue.h"
#include "MapObjectDesc.h"
#include "SceneNode.h"

class MapObject : public vn::RefCounted {
public:
    
    MapObject(MapObjectDesc *desc);
    virtual MapObject * clone() const;
    
    vn::EditValueObject * object() const;
    MapObjectDesc * desc() const;
    SceneNode * node() const;
    
    virtual void setPosition(const vn::vector2i &position);
    const vn::vector2i & getPosition() const;
    const vn::vector2i & size() const;
    
    virtual void addNodeToScene();
    virtual void removeNodeFromScene();
    
protected:
    vn::RefCountedPtr<vn::EditValueObject> m_object;
    vn::RefCountedPtr<MapObjectDesc> m_desc;
    SceneNodePtr m_node;
    
    vn::vector2i m_position;
    vn::vector2i m_size;
};


class MapObjectSpace : public vn::AllocatedObject {
public:
    MapObjectSpace();
    ~MapObjectSpace();
    void reset(const vn::vector2i &size);
    bool isEmpty(const vn::vector2i &position, const vn::vector2i &size) const;
    void place(const vn::vector2i &position, const vn::vector2i &size, MapObject *obj);
    MapObject *pick(const vn::vector2i &position) const;
protected:
    MapObject **m_space;
    vn::vector2i m_size;
};

class MapObjectFactory : public vn::Singleton<MapObjectFactory> {
public:
    MapObject * create(const vn::str8 &name);
    MapObject * create(MapObjectDesc *desc);
    template <class _DescType>
    void _register(MapObject *(*creator)(MapObjectDesc *)) {
        m_factory[_DescType::className()] = creator;
    }
private:
    typedef std::map<const vn::c8 *, MapObject *(*)(MapObjectDesc *)> Factory;
    Factory m_factory;
};

#define DECL_MAP_OBJECT(cls) \
    public:\
        typedef cls ObjectType;\
        static const vn::c8 *kObjectTypeName;\
        static MapObject * create(MapObjectDesc *);\
        virtual cls * clone() const;

#define IMPL_MAP_OBJECT(cls, desc_cls) \
    struct _auto_reg_##cls {\
        _auto_reg_##cls() {\
            MapObjectFactory::instance()._register<desc_cls>(cls::create);\
        }\
    } auto_reg_##cls##_ins;\
    const vn::c8 *cls::kObjectTypeName = #cls;\
    MapObject * cls::create(MapObjectDesc *desc) { return vnnew cls(dynamic_cast<desc_cls *>(desc)); }\
    cls * cls::clone() const {\
		vn::RefCountedPtr<desc_cls> desc = static_cast<desc_cls *>(m_desc.ptr())->clone();\
		return vnnew cls(desc.ptr());\
	}

#endif /* defined(__TDMapEditor__MapObject__) */
