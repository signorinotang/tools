//
//  Renderable.h
//  gamelib
//
//  Created by Wayne on 13-12-27.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef __gamelib__Renderable__
#define __gamelib__Renderable__

#include "vnMatrix2.h"
#include "vnColor.h"
#include "vnPicture.h"
#include "vnRender2DInterface.h"
#include "vnRenderText.h"

#include <list>

class SceneNode;

class Renderable : public vn::RefCounted {
	friend class SceneNode;
public:
	Renderable();
    virtual void update() {}
    virtual void render() = 0;
	
	SceneNode * sceneNode() const;
	
protected:
	virtual void onTransformUpdated() {}
	SceneNode *m_sceneNode;
};

class Quad : public Renderable {
public:
    Quad();
    
	virtual void update();
    virtual void render();
    
    Quad & operator <<(const vn::aabox2f &rect);
    Quad & operator <<(const vn::color_t &color);
    Quad & operator <<(const vn::Picture *picture);
	Quad & operator <<(vn::Render2DInterface::BlendMode blend);
	
protected:
	virtual void onTransformUpdated();
	vn::aabox2f m_rect;
	vn::vector2f m_corners[4];
    vn::color_t m_colors[4];
    vn::vector2f m_texcoords[4];
    vn::Texture2DPtr m_texture;
    vn::Render2DInterface::BlendMode m_blend;
	bool m_dirty;
};

class WireFrame : public Renderable {
public:
    WireFrame();
    
    virtual void update();
    virtual void render();
    
    WireFrame & operator <<(const vn::aabox2f &rect);
    WireFrame & operator <<(const vn::color_t &color);
  	WireFrame & operator <<(vn::Render2DInterface::BlendMode blend);
    WireFrame & operator <<(vn::f32 width);
    
protected:
    virtual void onTransformUpdated();
    vn::aabox2f m_rect;
    vn::color_t m_colors[8];
    vn::vector2f m_corners[8];
    vn::vector2f m_texcoords[8];
    vn::f32 m_width;
    vn::Render2DInterface::BlendMode m_blend;
	bool m_dirty;
};

class Text : public Renderable {
public:
	Text();
	
	virtual void update();
	virtual void render();
	
	Text & operator <<(const vn::aabox2f &rect);
	Text & operator <<(const vn::color_t &color);
	Text & operator <<(const vn::str8 &text);
	Text & operator <<(const vn::str32 &text);
	Text & operator <<(const vn::FontPtr &font);
    Text & operator <<(vn::RenderText::Alignment align);
	Text & operator <<(vn::Render2DInterface::BlendMode blend);
protected:
	vn::RenderText m_renderText;
	vn::Render2DInterface::BlendMode m_blend;
};


#endif /* defined(__gamelib__Renderable__) */
