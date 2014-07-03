//
//  vnTextureManager.h
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnTextureManager_h
#define vngfx_vnTextureManager_h

#include "vnTexture.h"
#include "vnSingleton.h"
#include "vnFilePath.h"

#include <map>

_vn_begin

class FileStream;

class _vn_gfx_api TextureManager : public Singleton<TextureManager> {
public:
    
    class Delegate {
    public:
        virtual bool loadTexture(FileStream &fs, TexturePtr &texture, vector2i &image_size) = 0;
    };
    
    TextureManager();
    
    const TexturePtr & loadTexture(const FilePath &path, vector2i *size = 0);
    void freeTexture(u32 fsId, const c8 *fileName);
    void freeTexture(const FilePath &path);
    void freeAllTextures();
    void freeUnusedTextures();
    
    void setDelegate(Delegate *delegate);
    
private:
    typedef std::map<FilePath, std::pair<TexturePtr, vector2i> > TextureMap;
    TextureMap m_textures;
    
    Delegate *m_delegate;
    
    //static bool _loadTexture(FileStream &fs, TexturePtr &texture);
    
};

_vn_end

#endif
