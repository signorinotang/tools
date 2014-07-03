//
//  vnUIRenderPicture.h
//  vngfx
//
//  Created by Wayne on 13-12-22.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIRenderPicture_h
#define vngfx_vnUIRenderPicture_h

#include "vnGfxDef.h"
#include "vnPicture.h"
#include "vnColor.h"
#include "vnUIFactory.h"

_vn_begin

class UIRenderer;

class _vn_gfx_api UIRenderPicture : public RefCounted {
public:
	virtual UIRenderPicture * clone() const = 0;
	
	virtual void update(f32 deltaTime);
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	
	virtual vector2f calculateMinimumSize() const;
};

typedef RefCountedPtr<UIRenderPicture> UIRenderPicturePtr;

class _vn_gfx_api UIRenderPictureEmpty : public UIRenderPicture {
	VN_DECL_UI_RENDER_PICTURE()
public:
	virtual UIRenderPictureEmpty * clone() const;
};

class _vn_gfx_api UIRenderPicture0 : public UIRenderPicture {
	VN_DECL_UI_RENDER_PICTURE()
public:
	virtual UIRenderPicture0 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	virtual void setLocation(const aabox2f &rect);
protected:
	aabox2f m_rect;
};

class _vn_gfx_api UIRenderPicture1 : public UIRenderPicture0 {
	VN_DECL_UI_RENDER_PICTURE()
public:
	virtual UIRenderPicture1 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void init(const TreeDataObject *object);
	
	void setPicture(const PicturePtr &picture);
	const PicturePtr & getPicture() const;
	
protected:
	PicturePtr m_picture;
};

class _vn_gfx_api UIRenderPicture3V : public UIRenderPicture {
	VN_DECL_UI_RENDER_PICTURE()
public:
	UIRenderPicture3V();
	virtual UIRenderPicture3V * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setHeightTop(f32 height);
	f32 getHeightTop() const;
	
	void setHeightBottom(f32 height);
	f32 getHeightBottom() const;
	
protected:
	aabox2f m_rect[3];
	PicturePtr m_picture[3];
	f32 m_heightTop, m_heightBottom;
};

class _vn_gfx_api UIRenderPicture3H : public UIRenderPicture {
	VN_DECL_UI_RENDER_PICTURE()
public:
	UIRenderPicture3H();
	virtual UIRenderPicture3H * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setWidthLeft(f32 width);
	f32 getWidthLeft() const;
	
	void setWidthRight(f32 width);
	f32 getWidthRight() const;
	
protected:
	aabox2f m_rect[3];
	PicturePtr m_picture[3];
	f32 m_widthLeft, m_widthRight;
};

class _vn_gfx_api UIRenderPicture9 : public UIRenderPicture {
	VN_DECL_UI_RENDER_PICTURE()
public:
	UIRenderPicture9();
	virtual UIRenderPicture9 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setWidthLeft(f32 width);
	f32 getWidthLeft() const;
	
	void setWidthRight(f32 width);
	f32 getWidthRight() const;
	
	void setHeightTop(f32 height);
	f32 getHeightTop() const;
	
	void setHeightBottom(f32 height);
	f32 getHeightBottom() const;
	
protected:
	aabox2f m_rect[9];
	PicturePtr m_picture[9];
	f32 m_widthLeft, m_widthRight;
	f32 m_heightTop, m_heightBottom;
};

_vn_end

#endif
