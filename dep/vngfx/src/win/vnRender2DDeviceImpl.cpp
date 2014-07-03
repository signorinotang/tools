#include "vnRender2DDeviceImpl.h"
#include "vnDataStream.h"

#ifdef _DEBUG
#include <iostream>
#endif


_vn_begin

static const c8 s_shader[] = 
"uniform float4x4 projMat;"
"uniform float4x4 viewMat;"
"sampler image : register(s0);"
"void vsMain( in float4 iPos : POSITION,"
"			  in float4 iColor : COLOR,"
"			  in float2 iTex : TEXCOORD0,"
"			  out float4 oPos : POSITION,"
"			  out float4 oColor : COLOR,"
"			  out float2 oTex : TEXCOORD0 ) {"
"	oPos = mul( mul( iPos, viewMat ), projMat );"
"	oColor = iColor;"
"	oTex = iTex;"
"}"
"void psMain( in float4 iColor : COLOR,"
"			  in float2 iTex : TEXCOORD0,"
"			  out float4 oColor : COLOR0 ) {"
"	oColor = iColor * tex2D( image, iTex );"
"}"
;


//mul( mul( iPos, viewMat ), projMat )

Render2DDevice * Render2DDevice::create(void *window, u32 width, u32 height) {
	Render2DDeviceImpl *device = vnnew Render2DDeviceImpl();
	if (!device->create(window, width, height)) {
		delete device;
		return 0;
	}
	return device;
}


static const u32 s_shaderLength = sizeof(s_shader)-1;

Render2DDeviceImpl::Render2DDeviceImpl()
: m_blendMode(Render2DInterface::kAlphaBlend)
, m_vertexShader(0)
, m_pixelShader(0)
, m_device(0)
, m_hProjMat(0)
, m_hViewMat(0)
, m_vertexFormat(0)
, m_nullTexture(0)
, m_currentTexture(0)
, m_numVertices(0)
, m_numIndices(0)
, m_vertices(0)
, m_indices(0)
, m_count(0)
{
	m_interfaceImpl._device = this;
	::D3DXMatrixIdentity(&m_projectionMatrix);
	::D3DXMatrixIdentity(&m_viewMatrix);
}

Render2DDeviceImpl::~Render2DDeviceImpl()
{
	//delete
	if (m_vertices)
	{
		vnfree(m_vertices);
	}
	if (m_indices)
	{
		vnfree(m_indices);
	}
	
}


bool Render2DDeviceImpl::create(void *window, u32 width, u32 height)
{
	IDirect3D9 *d3d9 = ::Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		printf("d3d9 init failed");
		return false;
	}
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	DWORD devBehaviorFlags = D3DCREATE_MULTITHREADED;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	m_deviceParameters.BackBufferWidth = (UINT)width;
	m_deviceParameters.BackBufferHeight = (UINT)height;
	m_deviceParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_deviceParameters.BackBufferCount = 1;
	m_deviceParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_deviceParameters.MultiSampleQuality = 0;
	m_deviceParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_deviceParameters.hDeviceWindow = (HWND)window;
	m_deviceParameters.Windowed = TRUE;
	m_deviceParameters.EnableAutoDepthStencil = FALSE;
	m_deviceParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	m_deviceParameters.Flags = 0;
	m_deviceParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_deviceParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// primary adapter
		D3DDEVTYPE_HAL,         // device type
		(HWND)window,			// window associated with device
		devBehaviorFlags,		// 
		&m_deviceParameters,    // present parameters
		&m_device			    // return created device
		);
	if (FAILED(hr))
	{
		d3d9->Release();
		return false;
	}
	d3d9->Release();

	if (m_device == 0)
	{
		printf("device create failed");
		return false;
	}
	ID3DXBuffer* sbuf = 0;
	ID3DXBuffer* errorMsg = 0;
	D3DXCompileShader(s_shader, s_shaderLength, NULL, NULL, "vsMain", "vs_2_0", 0, &sbuf, &errorMsg, &m_vsConstants);
	if (errorMsg) {
#ifdef _DEBUG
		std::cout << (const char *)errorMsg->GetBufferPointer() << std::endl;
#endif
		errorMsg->Release();
	}
	if (!sbuf) {
		m_device->Release();
		m_device = 0;
		return false;
	}

	m_device->CreateVertexShader((DWORD *)sbuf->GetBufferPointer(), &m_vertexShader);
	sbuf->Release();
	D3DXCompileShader(s_shader, s_shaderLength, NULL, NULL, "psMain", "ps_2_0", 0, &sbuf, NULL, NULL);
	if (errorMsg) {
#ifdef _DEBUG
		std::cout << (const char *)errorMsg->GetBufferPointer() << std::endl;
#endif
		errorMsg->Release();
	}
	if (!sbuf) {
		m_vertexShader->Release();
		m_vertexShader = 0;
		m_device->Release();
		m_device = 0;
		return false;
	}
	m_device->CreatePixelShader((DWORD *)sbuf->GetBufferPointer(), &m_pixelShader);
	sbuf->Release();

	m_hProjMat = m_vsConstants->GetConstantByName(0, "projMat");
	m_hViewMat = m_vsConstants->GetConstantByName(0, "viewMat");

	D3DVERTEXELEMENT9 elements[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION },
		{ 0, 8, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR },
		{ 0, 12, D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD },
		D3DDECL_END()
	};

	
	

	hr = m_device->CreateVertexDeclaration(elements, &m_vertexFormat);


	m_nullTexture = vnnew Texture2DImpl();
	m_nullTexture->createTexture(m_device, 1, 1);
	D3DLOCKED_RECT ret;
	m_nullTexture->m_texture->LockRect(0, &ret, NULL, 0);
	*(u32 *)ret.pBits = 0xFFFFFFFF;
	m_nullTexture->m_texture->UnlockRect(0);

	

	m_vertices = vnmalloc(Vertex, kMaxVertices);
	m_indices = vnmalloc(u16, kMaxIndices);

	TextureManager::instance().setDelegate(this);

	buildProjectionMatrix(width, height, m_projectionMatrix);

	applyRenderStates();

	return true;	
}

void Render2DDeviceImpl::resize(u32 width, u32 height)
{
	if (!width || !height)
	{
		return ;
	}
	m_deviceParameters.BackBufferWidth = width;
	m_deviceParameters.BackBufferHeight = height;
	buildProjectionMatrix(width, height, m_projectionMatrix);
	resetDevice();
}

void Render2DDeviceImpl::setViewMatrix(const matrix3 &mat)
{
	_flush();
	float m[16] = {
		mat.m11, mat.m12, 0, mat.m13,
		mat.m21, mat.m22, 0, mat.m23,
		0, 0, 0, 0,
		mat.m31, mat.m32, 0, mat.m33
	};
	m_vsConstants->SetMatrix(m_device, m_hViewMat, (D3DXMATRIX*)m);
}


void Render2DDeviceImpl::clear()
{
	HRESULT hr = m_device->Clear(0, 0, D3DCLEAR_TARGET, (D3DCOLOR)(0x00000000), 1.0f, 0);
}

bool Render2DDeviceImpl::begin()
{
	if (isDeviceLost())
	{
		return false; 
	}
	HRESULT hr = m_device->BeginScene();
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

void Render2DDeviceImpl::end()
{
	_flush();
	m_device->EndScene();
}

void Render2DDeviceImpl::present()
{
	m_device->Present(0, 0, 0, 0);
}


void Render2DDeviceImpl::drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices) {
	u32 idx_count = count * 3;
	if (m_numIndices + idx_count >= kMaxIndices || m_numVertices + numVertices >= kMaxVertices) {
		_flush();
	}
	m_count += count;
	vnassert(numVertices < kMaxVertices && idx_count < kMaxIndices);
	Vertex *pv = m_vertices + m_numVertices;
	for (u32 i = 0; i < numVertices; ++i, ++pv) {
		pv->position = *positions++;
		pv->color = (colors++)->convertToBGRA();
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

void Render2DDeviceImpl::setTexture(Texture2D* texture)
{
	Texture2DImpl *tex = dynamic_cast<Texture2DImpl *>(texture);
	if (!tex) {
		tex = m_nullTexture.ptr();
	}
	if (tex != m_currentTexture.ptr()) {
		_flush();
		tex->grab();
		m_currentTexture = tex;
	}
	return;
}

void Render2DDeviceImpl::_flush()
{
	if (!m_numIndices) {
		return;
	}

	m_device->SetTexture(0, m_currentTexture->m_texture);
	HRESULT hr = m_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_numVertices, m_count, m_indices, D3DFMT_INDEX16, m_vertices, sizeof(Vertex));
	if (FAILED(hr)) {
		//::DebugBreak();
	}
	m_numIndices = 0;
	m_numVertices = 0;
	m_count = 0;
}


void Render2DDeviceImpl::buildProjectionMatrix(u32 width, u32 height, D3DXMATRIX &matrix)
{

	/*float m[16] = {
	2.f / width, 0, 0, 0,
	0, -2.f / height, 0, 0,
	0, 0, 0, 0,
	-1, 1, 0, 1
	};*/
	//m_projectionMatrix = (D3DXMATRIX)m;
	D3DXMATRIX tmp, proj;
	D3DXMatrixScaling(&proj, 1.0, -1.0f, 1.0f);
	D3DXMatrixTranslation(&tmp, -0.5f, (f32)height + 0.5f, 0.0f);
	D3DXMatrixMultiply(&proj, &proj, &tmp);
	D3DXMatrixOrthoOffCenterLH(&tmp, 0.0f, (f32)width, 0.0f, (f32)height, 0, 1);
	D3DXMatrixMultiply(&matrix, &proj, &tmp);
	m_vsConstants->SetMatrix(m_device, m_hProjMat, (D3DXMATRIX*)&matrix);
	
}


void Render2DDeviceImpl::resetDevice()
{
	m_device->Reset(&m_deviceParameters);
	applyRenderStates();
}



void Render2DDeviceImpl::applyRenderStates()
{
	m_device->SetVertexDeclaration(m_vertexFormat);
	m_device->SetVertexShader(m_vertexShader);
	m_device->SetPixelShader(m_pixelShader);



	m_vsConstants->SetMatrix(m_device, m_hProjMat, &m_projectionMatrix);
	m_vsConstants->SetMatrix(m_device, m_hViewMat, &m_viewMatrix);

	//disable Zorder
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//disable cull face
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_currentTexture.null())
	{
		m_device->SetTexture(0, m_nullTexture->m_texture);
	}
	else
	{
		m_device->SetTexture(0, m_currentTexture->m_texture);
	}

	_applyBlendMode();

	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
}

void Render2DDeviceImpl::setBlendMode(Render2DInterface::BlendMode mode)
{
	if (m_blendMode != mode)
	{
		_flush();
		m_blendMode = mode;
		_applyBlendMode();
	}
}

void Render2DDeviceImpl::_applyBlendMode()
{
	switch (m_blendMode)
	{
	case Render2DInterface::BlendMode::kAlphaBlend:
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case Render2DInterface::BlendMode::kAlphaAdditive:
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	}

}

//*****************************load texture*******************************
Texture * createTextureFromPNG(Render2DDeviceImpl *device, DataStream *ds, u32 &width, u32 &height);

bool Render2DDeviceImpl::loadTexture(FileStream &fs, TexturePtr &texture, vector2i &image_size)
{
	u32 width, height;
	texture = createTextureFromPNG(this , (DataStream*)&fs, width, height);
	
	image_size.set(width, height);

	if (!texture.ptr())
	{
		return false;
	}

	return true;
}



Texture2DImpl* Render2DDeviceImpl::createTexture(u32 width, u32 height)
{
	Texture2DImpl *tex = new Texture2DImpl();
	if (!tex->createTexture(m_device, width, height))
	{
		tex->drop();
		tex = 0;
	}
	return tex;
}
//**************************************************************************
bool Render2DDeviceImpl::isDeviceLost()
{
	HRESULT hr = m_device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST)
	{
		::Sleep(50); 
		return true;
	}
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		::PostQuitMessage(0);
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		resetDevice();
		return false;
	}
	return false;
}
//********************************Render2DInterfaceImpl************************************************

Render2DInterfaceImpl::Render2DInterfaceImpl()
:_device(0)
{

}
void Render2DInterfaceImpl::begin(){}

void Render2DInterfaceImpl::end()
{
	_device->_flush();
}
void Render2DInterfaceImpl::setTexture(Texture2D* texture)
{
	_device->setTexture(texture);
}

void Render2DInterfaceImpl::setBlendMode(BlendMode mode)
{
	_device->setBlendMode(mode);
}

void Render2DInterfaceImpl::drawTriangles(u32 count, const vector2f *positions, const color_t *colors, const vector2f *texcoords, u32 numVertices, u16 *indices) {
	_device->drawTriangles(count, positions, colors, texcoords, numVertices, indices);
}

Texture2D* Render2DInterfaceImpl::createTexture(const vector2i &size)
{
	return _device->createTexture(size.x, size.y);
}


//********************************Render2DInterfaceImpl***************************************************
_vn_end