//
//  vnRender2DDeviceImpl.h
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#ifndef __vngfx__vnRender2DDeviceImpl__
#define __vngfx__vnRender2DDeviceImpl__

#include "vnRender2DDevice.h"
#include "vnRender2DInterface.h"
#include "vnTextureManager.h"
#include "vnTexture2DImpl.h"

_vn_begin

namespace internal {
    
    class Render2DDeviceImpl;
    
    struct Render2DInterfaceImpl : Render2DInterface {
        Render2DInterfaceImpl();
        
        virtual Texture2D * createTexture(const vector2i &size);
        virtual void begin();
        virtual void end();
        virtual void setTexture(Texture2D *texture);
        virtual void setBlendMode(BlendMode mode);
        virtual void drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices);
        
        Render2DDeviceImpl *_device;
    };

    class Render2DDeviceImpl : public Render2DDevice, public TextureManager::Delegate {
    public:
        Render2DDeviceImpl();
        ~Render2DDeviceImpl();
        
        virtual void resize(u32 width, u32 height);
        virtual void setViewMatrix(const matrix3 &mat);
        virtual void clear();
        virtual bool begin();
        virtual void end();
        virtual void present();
        
        virtual bool loadTexture(FileStream &fs, TexturePtr &texture, vector2i &image_size);
        
        bool create(void *window, u32 width, u32 height);
        void setTexture(Texture2D *texture);
        void setBlendMode(Render2DInterface::BlendMode mode);
        void drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices);
        
        void _flush();
        
    private:

        
        enum { kMaxVertices = 2048, kMaxIndices = 3072 };

        struct Vertex {
            vector2f position;
            color_t color;
            vector2f texcoord;
        };

        Texture2DImplPtr m_nullTexture;
        Texture2DImplPtr m_currentTexture;
        Render2DInterface::BlendMode m_blendMode;
        u32 m_numVertices;
        u32 m_numIndices;
        Vertex *m_vertices;
        u16 *m_indices;

        u32 m_program;
        u32 m_projMatLocation;
        u32 m_viewMatLocation;
        u32 m_vertexBuffer;
        u32 m_indexBuffer;
        u32 m_vao;

        
        void _applyBlendMode();
        void _updateProjectionMatrix();
        void _initViewMatrix();
        
        vector2i m_size;
        
        void *m_view;
        
        Render2DInterfaceImpl m_interfaceImpl;
    };

}

_vn_end

#endif /* defined(__vngfx__vnRender2DDeviceImpl__) */