//
//  vnPngLoader.cpp
//  vngfx
//
//  Created by Wayne on 13-12-13.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnMemory.h"
#include "vnDataStream.h"
#include "vnVector2.h"

#include "png.h"
#include "pngstruct.h"

_vn_begin

namespace internal {
    namespace {
        
        void PNGAPI pngRead( png_structp png, png_bytep data, png_size_t len ) {
            DataStream *ds = static_cast<DataStream *>(png->io_ptr);
            png_size_t ret = (png_size_t)ds->read((void *)data, len);
            if (ret != len) {
                png_error(png, "Read Error");
            }
        }
        
    }
    
    u32 * loadPNGFile(DataStream &ds, vector2i &size) {
        bool update = false;
        
        png_bytep *row_ptrs = NULL;
        png_byte header[8] = {};
        
        ds.read(header, sizeof(header));
        
        if (png_sig_cmp(header, 0, sizeof(header))) {
            return 0;
        }
        
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        
        if (!png) {
            return 0;
        }
        
        png_infop info = png_create_info_struct(png);
        if (!info) {
            png_destroy_read_struct(&png, 0, 0);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png))) {
            png_destroy_read_struct(&png, &info, 0);
            if (row_ptrs) {
                vnfree(row_ptrs);
            }
            return 0;
        }
        
        png_set_read_fn(png, &ds, pngRead);
        
        png_set_sig_bytes(png, sizeof(header));
        
        png_read_info(png, info);
        
        u32 width, height;
        s32 bitDepth;
        s32 colorType;
        png_get_IHDR( png, info, &width, &height, &bitDepth, &colorType, 0, 0, 0 );
        
        if (colorType == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png);
            update = true;
        }
        
        if (bitDepth < 8) {
            if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
                png_set_expand_gray_1_2_4_to_8(png);
            } else {
                png_set_packing(png);
            }
            update = true;
        } else if (bitDepth == 16) {
            png_set_strip_16(png);
            update = true;
        }
        
        if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb( png );
            update = true;
        }
        
        if (update) {
            png_read_update_info(png, info);
            png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, 0, 0, 0);
            update = false;
        }
        
        if (colorType == PNG_COLOR_TYPE_RGB) {
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
            update = true;
        }
        
        if (update) {
            png_read_update_info( png, info );
            png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, 0, 0, 0);
            update = false;
        }
        
        u32 texWidth, texHeight;
#if 0
        for ( int i = 0; i < 16; ++i ) {
            texWidth = 1 << i;
            if ( texWidth >= width ) {
                break;
            }
        }
        for ( int i = 0; i < 16; ++i ) {
            texHeight = 1 << i;
            if ( texHeight >= height ) {
                break;
            }
        }
#else
        texWidth = width;
        texHeight = height;
#endif
        
        u32 *data = vnmalloc(u32, texWidth * texHeight);
        u32 *pixels = data;
        
        row_ptrs = vnmalloc(png_bytep, height);
        
        for (u32 i = 0; i < height; ++i) {
            row_ptrs[i] = (png_bytep)pixels;
            pixels += texWidth;
        }
        
        if (setjmp(png_jmpbuf(png))) {
            png_destroy_read_struct(&png, &info, 0);
            vnfree(row_ptrs);
            vnfree(data);
            return 0;
        }
        
        png_read_image(png, row_ptrs);
        
        png_read_end(png, 0);
        
        vnfree(row_ptrs);
        
        png_destroy_read_struct(&png, &info, 0);
        
        size.set(texWidth, texHeight);
        
        return data;
    }
    
}

_vn_end
