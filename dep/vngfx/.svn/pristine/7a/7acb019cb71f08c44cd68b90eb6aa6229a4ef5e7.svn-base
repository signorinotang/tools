#ifndef vngfx_vnDeviceImp_h
#define vngfx_vnDeviceImp_h

#include "vnGfxDef.h"
#include "vnRender2DDevice.h"
#include "vnRender2DInterface.h"
#include "vnTexture2DImpl.h"
#include "vnTextureManager.h"
#include <d3d9.h>
#include <d3dx9.h>






_vn_begin



class Render2DDeviceImpl;

struct Render2DInterfaceImpl : public Render2DInterface
{
	Render2DInterfaceImpl();
	virtual Texture2D* createTexture(const vector2i &size);
	virtual void begin();
	virtual void end();
	virtual void setTexture(Texture2D *texture);
	virtual void setBlendMode(BlendMode mode);
	virtual void drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices);

	Render2DDeviceImpl *_device;
};




class Render2DDeviceImpl : public Render2DDevice, public TextureManager::Delegate
{
public:
	Render2DDeviceImpl();
	~Render2DDeviceImpl();

	bool create(void *window, u32 width, u32 height);
	virtual void resize(u32 width, u32 height);
	virtual void setViewMatrix(const matrix3 &mat);
	virtual void clear();
	virtual bool begin();
	virtual void end();
	virtual void present();
	virtual bool loadTexture(FileStream &fs, TexturePtr &texture, vector2i &image_size) ;
	Texture2DImpl* createTexture(u32 width, u32 height);

	
	//add by ts 
	void buildProjectionMatrix(u32 width, u32 height, D3DXMATRIX &matrix);
	void applyRenderStates();
	void setBlendMode(Render2DInterface::BlendMode mode);
	void drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices);
	void _flush();
	bool isDeviceLost();
	void resetDevice();
	void setTexture(Texture2D* texture);


	
private:
	enum { kMaxVertices = 2048, kMaxIndices = 3072 };

	struct Vertex {
		vector2f position;
		u32 color;
		vector2f texcoord;
	};

	IDirect3DVertexShader9*		 m_vertexShader;
	IDirect3DPixelShader9*		 m_pixelShader;

	D3DPRESENT_PARAMETERS		 m_deviceParameters;
	IDirect3DDevice9*			 m_device;
	D3DXHANDLE					 m_hProjMat;
	D3DXHANDLE					 m_hViewMat;
	IDirect3DVertexDeclaration9* m_vertexFormat;

	Texture2DImplPtr			 m_nullTexture;
	Texture2DImplPtr             m_currentTexture;

	D3DXMATRIX					 m_projectionMatrix;
	D3DXMATRIX					 m_viewMatrix;

	ID3DXConstantTable*          m_vsConstants;

	Render2DInterface::BlendMode m_blendMode;

	u32         m_count;
	u32			m_numVertices;
	u32			m_numIndices;
	Vertex *	m_vertices;
	u16 *		m_indices;

	void _applyBlendMode();

	Render2DInterfaceImpl m_interfaceImpl;



};
_vn_end

#endif