//
//  vnTextureManager.cpp
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnTextureManager.h"
#include "vnFileManager.h"
#include "vnTexture2D.h"

_vn_begin

namespace internal {
    class TextureManagerDefaultDelegate : public TextureManager::Delegate {
    public:
        virtual bool loadTexture(FileStream &, TexturePtr &, vector2i &) { return false; }
    };
    static TextureManagerDefaultDelegate s_defaultDelegate;
}

TextureManager::TextureManager()
: m_delegate(&internal::s_defaultDelegate) {
    
}

void TextureManager::setDelegate(Delegate *delegate) {
    vnassert(delegate);
    m_delegate = delegate;
}

const TexturePtr & TextureManager::loadTexture(const vn::FilePath &path, vector2i *size) {
    std::pair<TextureMap::iterator, bool> ret = m_textures.insert(std::make_pair(path, std::make_pair(TexturePtr::kNull, vector2i::kZero)));
    if (ret.second) {
        FileStream *pfs = FileManager::instance().open(path);
        if (!pfs) {
            m_textures.erase(ret.first);
            return TexturePtr::kNull;
        }
        if (!m_delegate->loadTexture(*pfs, ret.first->second.first, ret.first->second.second)) {
            pfs->close();
            m_textures.erase(ret.first);
            return TexturePtr::kNull;
        }
        pfs->close();
    }
	if (size) {
		*size = ret.first->second.second;
	}
    return ret.first->second.first;
}

void TextureManager::freeTexture(u32 fsId, const c8 *fileName) {
    freeTexture(FilePath(fsId, fileName));
}

void TextureManager::freeTexture(const FilePath &path) {
    m_textures.erase(path);
}

void TextureManager::freeAllTextures() {
    m_textures.clear();
}

void TextureManager::freeUnusedTextures() {
    for (TextureMap::iterator it = m_textures.begin(); it != m_textures.end(); ) {
        if (it->second.first.ref_count() == 1) {
            it = m_textures.erase(it);
        } else {
            ++it;
        }
    }
}


_vn_end
