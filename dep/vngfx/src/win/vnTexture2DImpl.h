#include "vnTexture2D.h"
#include <d3d9.h>

_vn_begin

class Texture2DImpl : public Texture2D
{
public:	
	Texture2DImpl();

	virtual ~Texture2DImpl();

	//virtual Type type() const;
	virtual bool updatePixels(const vector2i &offset, const vector2i &size, const void *pixels);

	bool createTexture(IDirect3DDevice9* device, u32 width, u32 height);

	void size(u32 &width, u32& height);
	u32* lock(u32 &pitch, bool readOnly = false);
	u32* lock(u32 left, u32 top, u32 right, u32 bottom, u32 &pitch, bool readOnly);
	void unlock();
	IDirect3DTexture9 *m_texture;

	u32 m_width;
	u32 m_height;
};

typedef RefCountedPtr<Texture2DImpl> Texture2DImplPtr;

_vn_end