#include "vnTexture2DImpl.h"


_vn_begin


Texture2DImpl::Texture2DImpl()
:m_texture(0)
, m_width(0)
, m_height(0)
{

}
Texture2DImpl::~Texture2DImpl()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

bool Texture2DImpl::updatePixels(const vector2i &offset, const vector2i &size, const void *pixels)
{	
	if (size.x <= 0 || size.y <= 0)
	{
		return false;
	}

	RECT rc = { offset.x, offset.y, offset.x + size.x, offset.y + size.y };

	D3DLOCKED_RECT rect;
	HRESULT hr = m_texture->LockRect(0, &rect, &rc, 0);

	if (FAILED(hr)) return false;

	u32 pitch = (u32)rect.Pitch / sizeof(u32);
	
	u32 *dest = (u32 *)rect.pBits;
	const u32 *src = (const u32 *)pixels;

	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			dest[x] = src[x];
		}
		dest += pitch;
		src += size.x;
	}

	m_texture->UnlockRect(0);
	return true;
}

bool Texture2DImpl::createTexture(IDirect3DDevice9* device, u32 width, u32 height)
{
	device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture, NULL);
	
	m_size = vector2i(width, height);

	if (!m_texture)
	{
		return false;
	}
	return true;
}

void Texture2DImpl::size(u32 &width, u32 &height)
{
	width = m_width;
	height = m_height;
}

u32 * Texture2DImpl::lock(u32& pitch, bool readOnly)
{
	D3DLOCKED_RECT locked;
	DWORD dwFlag = 0;
	if (readOnly) {
		dwFlag |= D3DLOCK_READONLY;
	}
	HRESULT hr = m_texture->LockRect(0, &locked, NULL, dwFlag);
	if (FAILED(hr))
		return 0;
	pitch = (u32)locked.Pitch / sizeof(u32);
	return (u32 *)locked.pBits;
}

u32* Texture2DImpl::lock(u32 left, u32 top, u32 right, u32 bottom, u32 &pitch, bool readOnly){
	D3DLOCKED_RECT locked;
	DWORD dwFlag = 0;
	if (readOnly) {
		dwFlag |= D3DLOCK_READONLY;
	}
	RECT rc = { left, top, right, bottom };
	HRESULT hr = m_texture->LockRect(0, &locked, &rc, dwFlag);
	if (FAILED(hr))
		return 0;
	pitch = (u32)locked.Pitch / sizeof(u32);
	return (u32 *)locked.pBits;
}

void Texture2DImpl::unlock() {
	m_texture->UnlockRect(0);
}

_vn_end