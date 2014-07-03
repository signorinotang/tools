//
//  SceneNode.h
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __gamelib__SceneNode__
#define __gamelib__SceneNode__

#include "Renderable.h"

class SceneNode : public vn::RefCounted {
public:
    SceneNode();
    virtual ~SceneNode();
    
    virtual void update(vn::f32 deltaTime);
    
	virtual void setPosition(const vn::vector2f &pos);
	const vn::vector2f & getPosition() const;
	
	void setRotation(vn::real radians);
	vn::real getRotation() const;
	
	void setScale(const vn::vector2f &scale);
	const vn::vector2f & getScale() const;
	
	void setVisible(bool visible);
	bool isVisible() const;
	
	void setLocalBoundingBox(const vn::aabox2f &rect);
	const vn::aabox2f & getLocalBoundingBox() const;
	
	void setInheritTransformM(bool inherit);
	bool isInheritTransformM() const;
	
	void setInheritTransformT(bool inherit);
	bool isInheritTransformT() const;
	
	const vn::vector2f & oritentation() const;
	const vn::matrix2 & transformM() const;
	const vn::vector2f & transformT() const;
	const vn::aabox2f & worldBoundingBox() const;
	
	void addRenderable(Renderable *renderable, bool grab = true);
	bool removeRenderable(Renderable *renderable);
	void clearRenderables();
    
	SceneNode * parent() const;
	void addChild(SceneNode *child, bool grab = true);
	void removeChild(SceneNode *child);
    void bringChildUp(SceneNode *child);
    void bringChildDown(SceneNode *child);
	void clearChildren();
    
    size_t numChildren() const { return m_children.size(); }
    SceneNode * child(size_t index) const { return m_children[index]; }

	vn::vector2f convertToWorld(const vn::vector2f &point);
	vn::vector2f convertFromWorld(const vn::vector2f &point);
	
	void markDirty();
	void cleanDirty();
	
	void updateRenderables();
	void renderRenderables();
	
	void queryVisibleNodes(const vn::aabox2f &view, std::vector<SceneNode *> &nodes);
	void queryVisibleChildren(const vn::aabox2f &view, std::vector<SceneNode *> &nodes);
    
protected:
	SceneNode *m_parent;
	
	vn::vector2f m_position;
	vn::real m_rotation;
	vn::vector2f m_orientation;
	vn::vector2f m_scale;
	bool m_visible;
	
	vn::matrix2 m_transformM;
	vn::vector2f m_transformT;
    
	bool m_inheritTransformM;
	bool m_inheritTransformT;
	
	typedef std::vector<SceneNode *> Nodes;
	Nodes m_children;
	
    typedef std::vector<Renderable *> Renderables;
    Renderables m_renderables;
	
	vn::aabox2f m_localBoundingBox;
	vn::aabox2f m_worldBoundingBox;
	bool m_dirty;
	
private:
	void _updateTransform();
	void _updateWorldBoundingBox();
};

typedef vn::RefCountedPtr<SceneNode> SceneNodePtr;

#endif /* defined(__gamelib__SceneObject__) */
