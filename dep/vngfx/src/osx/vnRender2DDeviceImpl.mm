//
//  vnRender2DDeviceImpl.cpp
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnRender2DDeviceImpl.h"
#include "vnGfxApplicationImpl.h"

#import "vnRender2DView.h"
#include "vnFileSystem.h"

#include <OpenGL/gl3.h>

_vn_begin

Render2DDevice * Render2DDevice::create(void *window, u32 width, u32 height) {
    internal::Render2DDeviceImpl *device = vnnew internal::Render2DDeviceImpl();
    if (!device->create(window, width, height)) {
        delete device;
        return 0;
    }
    return device;
}

namespace internal {
    
    u32 * loadPNGFile(DataStream &ds, vector2i &size);
    
    Render2DDeviceImpl::Render2DDeviceImpl()
    : m_view(0)
    , m_blendMode(Render2DInterface::kAlphaBlend)
    , m_program(0)
    , m_projMatLocation(0)
    , m_viewMatLocation(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
    , m_vao(0)
    , m_vertices(0)
    , m_indices(0)
    , m_numVertices(0)
    , m_numIndices(0) {
        m_interfaceImpl._device = this;
    }
    
    Render2DDeviceImpl::~Render2DDeviceImpl() {
        if (m_vertexBuffer) {
            glDeleteBuffers(1, &m_vertexBuffer);
        }
        if (m_indexBuffer) {
            glDeleteBuffers(1, &m_indexBuffer);
        }
        if (m_vao) {
            glDeleteVertexArrays(1, &m_vao);
        }

        if (m_vertices) {
            vnfree(m_vertices);
        }
        if (m_indices) {
            vnfree(m_indices);
        }
        if (m_program) {
            glDeleteProgram(m_program);
        }
    }
    
    bool Render2DDeviceImpl::create(void *window, u32 width, u32 height) {
        Render2DView *view = [[Render2DView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
        if (!view) {
            return false;
        }
        m_view = (void *)CFBridgingRetain(view);
        [(__bridge NSWindow *)window setContentView:view];
        
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        const c8 *vsShader =
        "#version 150\n"
        "in vec4 position;"
        "in vec4 color;"
        "in vec2 texcoord;"
        "uniform mat4 projMat;"
        "uniform mat4 viewMat;"
        "out vec4 v_color;"
        "out vec2 v_texcoord;"
        "void main() {"
        "	gl_Position = projMat * (viewMat * position);"
        "	v_color = color;"
        "	v_texcoord = texcoord;"
        "}"
        "";

        const c8 *psShader =
        "#version 150\n"
        "uniform sampler2D image;"
        "in vec4 v_color;"
        "in vec2 v_texcoord;"
        "out vec4 fragColor;"
        "void main() {"
        "	fragColor = v_color * texture(image, v_texcoord);"
        "}"
        "";

        GLint status = 0;

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsShader, 0);
        glCompileShader(vs);
        glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            //char buf[128];
            //GLsizei length;
            //glGetShaderInfoLog(vs, sizeof(buf), &length, buf);
            //std::cout << buf << std::endl;
            glDeleteShader(vs);
            return false;
        }

        GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(ps, 1, &psShader, 0);
        glCompileShader(ps);
        glGetShaderiv(ps, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            //char buf[128];
            //GLsizei length;
            //glGetShaderInfoLog(ps, sizeof(buf), &length, buf);
            //std::cout << buf << std::endl;
            glDeleteShader(ps);
            return false;
        }

        m_program = glCreateProgram();
        glAttachShader(m_program, vs);
        glAttachShader(m_program, ps);

        glBindAttribLocation(m_program, 0, "position");
        glBindAttribLocation(m_program, 1, "color");
        glBindAttribLocation(m_program, 2, "texcoord");

        glLinkProgram(m_program);


        glGetProgramiv(m_program, GL_LINK_STATUS, &status);
        if ( status == GL_FALSE ) {
            glDeleteProgram(m_program);
            m_program = 0;
            return false;
        }

        glDeleteShader(vs);
        glDeleteShader(ps);

        m_projMatLocation = glGetUniformLocation(m_program, "projMat");
        m_viewMatLocation = glGetUniformLocation(m_program, "viewMat");

        glUseProgram(m_program);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        glGenBuffers(1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)8);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)12);
        
        m_nullTexture = vnnew Texture2DImpl();
        m_nullTexture->init(vector2i(1, 1), "\xFF\xFF\xFF\xFF");
        m_currentTexture = m_nullTexture;
        
        m_vertices = vnmalloc(Vertex, kMaxVertices);
        m_indices = vnmalloc(u16, kMaxIndices);
        m_size.set(width, height);

        _updateProjectionMatrix();
        _initViewMatrix();

        TextureManager::instance().setDelegate(this);
        
        return true;
    }
    
    
    void Render2DDeviceImpl::resize(u32 width, u32 height) {
        _flush();
        m_size.set(width, height);
        glViewport(0, 0, width, height);
        _updateProjectionMatrix();
    }
    
    void Render2DDeviceImpl::setViewMatrix(const matrix3 &mat) {
        _flush();
        GLfloat m[16] = {
            mat.m11, mat.m12, 0, mat.m13,
            mat.m21, mat.m22, 0, mat.m23,
            0, 0, 0, 0,
            mat.m31, mat.m32, 0, mat.m33
        };
        glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, m);
    }
    
    void Render2DDeviceImpl::clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    bool Render2DDeviceImpl::begin() {
        return true;
    }
    
    void Render2DDeviceImpl::end() {
        _flush();
    }
    
    void Render2DDeviceImpl::present() {
        [(__bridge Render2DView *)m_view present];
    }
    
    bool Render2DDeviceImpl::loadTexture(FileStream &fs, TexturePtr &texture, vector2i &image_size) {
        u32 *pixels = loadPNGFile(fs, image_size);
        if (!pixels) {
            return false;
        }
        Texture2DImpl *tex = vnnew Texture2DImpl();
        tex->init(image_size, pixels);
        vnfree(pixels);
        texture = tex;
        return true;
    }
    
    void Render2DDeviceImpl::setTexture(Texture2D *texture) {
        Texture2DImpl *tex = dynamic_cast<Texture2DImpl *>(texture);
        if (!tex) {
            tex = m_nullTexture.ptr();
        }
        if (tex != m_currentTexture.ptr()) {
            _flush();
            tex->grab();
            m_currentTexture = tex;
        }
    }
    
    void Render2DDeviceImpl::setBlendMode(Render2DInterface::BlendMode mode) {
        if (m_blendMode != mode) {
            _flush();
        }
        m_blendMode = mode;
        _applyBlendMode();
    }
    
    void Render2DDeviceImpl::drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices) {
        u32 idx_count = count * 3;
        if (m_numIndices + idx_count >= kMaxIndices || m_numVertices + numVertices >= kMaxVertices) {
            _flush();
        }
        vnassert(numVertices < kMaxVertices && idx_count < kMaxIndices);
        Vertex *pv = m_vertices + m_numVertices;
        for (u32 i = 0; i < numVertices; ++i, ++pv) {
            pv->position = *positions++;
            pv->color = *colors++;
            pv->texcoord = *texcoords++;
        }
        u16 base = (u16)m_numVertices;
        u16 *pi = m_indices + m_numIndices;
        for (u32 i = 0; i < idx_count; ++i) {
            *pi++ = base + *indices++;
        }
        m_numVertices += numVertices;
        m_numIndices += idx_count;
    }
    
    void Render2DDeviceImpl::_flush() {
        if (!m_numIndices) {
            return ;
        }
        m_currentTexture->bind();

        glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), m_vertices, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(u16), m_indices, GL_STREAM_DRAW);
        glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_SHORT, 0);

        m_numIndices = 0;
        m_numVertices = 0;
    }

    void Render2DDeviceImpl::_applyBlendMode() {
        switch (m_blendMode) {
            case Render2DInterface::kAlphaBlend:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            case Render2DInterface::kAlphaAdditive:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
        }
    }

    void Render2DDeviceImpl::_updateProjectionMatrix() {
        GLfloat m[16] = {
            2.f / m_size.x, 0, 0, 0,
            0, -2.f / m_size.y, 0, 0,
            0, 0, 0, 0,
            -1, 1, 0, 1
        };
        glUniformMatrix4fv(m_projMatLocation, 1, GL_FALSE, m);
    }
    
    void Render2DDeviceImpl::_initViewMatrix() {
        GLfloat m[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 1
        };
        glUniformMatrix4fv(m_viewMatLocation, 1, GL_FALSE, m);
    }
    
#pragma mark -
    
    Render2DInterfaceImpl::Render2DInterfaceImpl() : _device(0) {}
    
    Texture2D * Render2DInterfaceImpl::createTexture(const vector2i &size) {
        Texture2DImpl *tex = vnnew Texture2DImpl();
        if (!tex->init(size, 0)) {
            delete tex;
            return 0;
        }
        return tex;
    }
    void Render2DInterfaceImpl::begin() {}
    void Render2DInterfaceImpl::end() {
        _device->_flush();
    }
    void Render2DInterfaceImpl::setTexture(Texture2D *texture) {
        _device->setTexture(texture);
    }
    
    void Render2DInterfaceImpl::setBlendMode(BlendMode mode) {
        _device->setBlendMode(mode);
    }
    
    void Render2DInterfaceImpl::drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices) {
        _device->drawTriangles(count, positions, colors, texcoords, numVertices, indices);
    }

}

_vn_end