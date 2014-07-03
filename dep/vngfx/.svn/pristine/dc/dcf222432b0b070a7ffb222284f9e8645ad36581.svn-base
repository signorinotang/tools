// Created by Wayne.
// 2011-11-17


#include "png.h"
#include "pngstruct.h"
#include "vnRender2DDevice.h"
#include "vnRender2DDeviceImpl.h"
#include "vnTexture.h"
#include "vnFileSystem.h"


_vn_begin

namespace {
	//void pngError( png_structp png, png_const_charp msg ) {
	//	// TODO: log error msg
	//		
	//	longjmp( png->png_jmpbuf, 1 );
	//}

	void PNGAPI pngRead(png_structp png, png_bytep data, png_size_t len) {
		DataStream *ds = static_cast<DataStream *>(png->io_ptr);
		png_size_t ret = (png_size_t)ds->read((void *)data, len);
		if (ret != len) {
			png_error(png, "Read Error");
		}
	}
}

Texture * createTextureFromPNG(Render2DDeviceImpl *device, DataStream *ds, unsigned int &width, unsigned int &height) {

	bool update = false;
	// Point to image rows.
	png_bytep *row_ptrs = NULL;

	png_byte header[8] = { 0 };

	ds->read(header, sizeof(header));

	if (png_sig_cmp(header, 0, sizeof(header))) {
		return 0;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png) {
		return 0;
	}

	png_infop info = png_create_info_struct(png);
	if (!info) {
		png_destroy_read_struct(&png, 0, 0);
		return 0;
	}


	if (setjmp(png_jmpbuf(png))) {
		png_destroy_read_struct(&png, &info, NULL);
		if (row_ptrs) {
			delete[]row_ptrs;
		}
		return 0;
	}

	png_set_read_fn(png, ds, pngRead);

	png_set_sig_bytes(png, sizeof(header));

	png_read_info(png, info);

	//u32 width;
	//u32 height;
	s32 bitDepth;
	s32 colorType;
	png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);

	if (colorType == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png);
		update = true;
	}

	if (bitDepth < 8) {
		if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
			png_set_expand_gray_1_2_4_to_8(png);
		}
		else {
			png_set_packing(png);
		}
		update = true;
	}
	else if (bitDepth == 16) {
		png_set_strip_16(png);
		update = true;
	}

	if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png);
		update = true;
	}


	if (update) {
		png_read_update_info(png, info);
		png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);
		update = false;
	}

	if (colorType == PNG_COLOR_TYPE_RGB) {
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	png_set_bgr(png);
	png_read_update_info(png, info);
	png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL);

	u32 texWidth, texHeight;
#if 0
	for (int i = 0; i < 16; ++i) {
		texWidth = 1 << i;
		if (texWidth >= width) {
			break;
		}
	}
	for (int i = 0; i < 16; ++i) {
		texHeight = 1 << i;
		if (texHeight >= height) {
			break;
		}
	}
#else
	texWidth = width;
	texHeight = height;
#endif

	Texture2DImpl *tex = device->createTexture(texWidth, texHeight);
	if (!tex) {
		png_destroy_read_struct(&png, &info, NULL);
		return 0;
	}

	u32 pitch;
	u32 *pixels = tex->lock(pitch);
	if (!pixels) {
		tex->drop();
		png_destroy_read_struct(&png, &info, NULL);
		return 0;
	}

	row_ptrs = new png_bytep[height];

	for (u32 i = 0; i < height; ++i) {
		row_ptrs[i] = (png_bytep)pixels;
		pixels += pitch;
	}

	if (setjmp(png_jmpbuf(png))) {
		png_destroy_read_struct(&png, &info, NULL);
		delete[]row_ptrs;
		tex->unlock();
		tex->drop();
		return 0;
	}

	if (update) {
		png_set_interlace_handling(png);
	}

	png_read_image(png, row_ptrs);

	png_read_end(png, NULL);
	delete[]row_ptrs;
	tex->unlock();
	png_destroy_read_struct(&png, &info, NULL);

	return tex;
}

_vn_end