//
//  vnPictureManager.cpp
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnPictureManager.h"
#include "vnTextureManager.h"
#include "vnFileManager.h"

_vn_begin

PictureManager::~PictureManager() {
    for (PictureSetInfoMap::iterator it = m_info.begin(); it != m_info.end(); ++it) {
        if (it->second) {
            delete it->second;
        }
    }
}

Picture * PictureManager::createPicture(const c8 *name) {
    FilePath psname;
    c8 buf0[256];
    c8 buf1[128];
    if (sscanf(name, "%u:/%[^|]|%[^\n]", &psname.fsId, buf0, buf1) != 3) {
        return 0;
    }
    psname.fileName = buf0;
    
    std::pair<PictureSetInfoMap::iterator, bool> ret = m_info.insert(std::make_pair(psname, (PictureSetInfo *)0));
    
    if (ret.second) {
        str8 infofile = psname.fileName;
        infofile.append("/info");
        // TODO: create PictureSetInfo
    }
    
    // packed.
    if (ret.first->second) {
        PictureSetInfo::PictureMap::iterator it = ret.first->second->pictures.find(buf1);
        if (it == ret.first->second->pictures.end()) {
            return 0;
        }
        Picture *pic = vnnew Picture();
        pic->texture = TextureManager::instance().loadTexture(*it->second.texture).dyn_cast<Texture2D>();
		pic->size = it->second.rect.size();
        if (!pic->texture.null()) {
            pic->buildTexcoord(pic->texture->size(), it->second.rect);
        }
        return pic;
    }
    
    // unpacked.
    psname.fileName.push_back('/');
    psname.fileName.append(buf1);
	vector2i size;
    Texture2DPtr tex = TextureManager::instance().loadTexture(psname, &size).dyn_cast<Texture2D>();
    if (tex.null()) {
        return 0;
    }
    Picture *pic = vnnew Picture();
    pic->texture = tex;
	pic->size = size;
    pic->buildTexcoord(pic->texture->size(), aabox2f(vector2f::kZero, size));
    return pic;
}

bool PictureManager::list(const FilePath &file, std::list<str8> &result) {
	std::pair<PictureSetInfoMap::iterator, bool> ret = m_info.insert(std::make_pair(file, (PictureSetInfo *)0));
    
    if (ret.second) {
        str8 infofile = file.fileName;
        infofile.append("/info");
        // TODO: create PictureSetInfo
    }
    
    // packed.
    if (ret.first->second) {
		result.clear();
		for (PictureSetInfo::PictureMap::iterator it = ret.first->second->pictures.begin(); it != ret.first->second->pictures.end(); ++it) {
			result.push_back(it->first);
		}
		return true;
	}
	
	// unpacked.
	return FileManager::instance().list(file, result);
}


_vn_end