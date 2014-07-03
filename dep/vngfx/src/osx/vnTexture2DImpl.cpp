//
//  vnTexture2DImpl.cpp
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013年 viichi.com. All rights reserved.
//

#include "vnTexture2DImpl.h"
#include <OpenGL/gl3.h>

_vn_begin

namespace internal {
    
    Texture2DImpl::Texture2DImpl()
    : m_object(0) {
        m_format = kTPF_RGBA;
    }
    
    Texture2DImpl::~Texture2DImpl() {
        if (m_object) {
            glDeleteTextures(1, &m_object);
            m_object = 0;
        }
    }
    
    bool Texture2DImpl::init(const vector2i &size, const void *pixels) {
        if (size.x <= 0 || size.y <= 0) {
            return false;
        }
        m_size = size;
        glGenTextures(1, &m_object);
        glBindTexture(GL_TEXTURE_2D, m_object);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        return true;
    }
    
    void Texture2DImpl::bind() {
        glBindTexture(GL_TEXTURE_2D, m_object);
    }
    
    bool Texture2DImpl::updatePixels(const vector2i &offset, const vector2i &size, const void *pixels) {
        glBindTexture(GL_TEXTURE_2D, m_object);
        glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        return true;
    }
}

_vn_end