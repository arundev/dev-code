#include "sdEnginePCH.h"
#include "sdSkyBox.h"

#include "IRenderDevice.h"
#include "IResourceSystem.h"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;;
//-------------------------------------------------------------------------------------------------
sdSkyBox::sdSkyBox()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	m_fSkyBoxWidth = 20000.f;
	m_fSkyBoxLength = 20000.f;
	m_fSkyBoxHeight = 10000.f;
	m_fSkyBoxBottom = -2500.f;
	m_bSktBoxNegZ = true;

	m_kSkyBoxPosXYPath = "edata\\world0\\texture\\weather\\sky\\skytest\\skytest10_01.dds";
	m_kSkyBoxNegXYPath = "edata\\world0\\texture\\weather\\sky\\skytest\\skytest10_02.dds";
	m_kSkyBoxPosZPath = "edata\\world0\\texture\\weather\\sky\\skytest\\skytest10_03.dds";

	m_spMaterial = pkRenderDevice->CreateMaterial("Environment_SkyBox");
	NIASSERT(m_spMaterial);

	CreateSkyBox();
}
//-------------------------------------------------------------------------------------------------
sdSkyBox::~sdSkyBox()
{

}
//-------------------------------------------------------------------------------------------------
void sdSkyBox::Render()
{
	D3DPERF_BeginEvent(0xff000000, L"Environment_SkyBox");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// 绘制
	// @{
	m_spPosXMesh->RenderImmediate(spRenderer);
	m_spNegXMesh->RenderImmediate(spRenderer);
	m_spPosYMesh->RenderImmediate(spRenderer);
	m_spNegYMesh->RenderImmediate(spRenderer);
	m_spPosZMesh->RenderImmediate(spRenderer);
	//m_spNegZMesh->RenderImmediate(spRenderer);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
NiMeshPtr sdSkyBox::CreateQuad(NiPoint3* spVertexData, NiPoint2* spUVSetData, ushort* pusIndiceData, 
	NiTexture* spTexture, NiMaterial* spMaterial)
{
	NiMeshPtr spMesh = NiNew NiMesh();
	NIASSERT(spMesh);

#ifdef SDDEBUG
	spMesh->SetName("SkyBox");
#endif

	spMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);
	
	// 顶点数据流
	NiDataStream* spVertexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		4,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX,
		spVertexData);
	NIASSERT(spVertexDataStream);
	spMesh->AddStreamRef(spVertexDataStream, NiCommonSemantics::POSITION(), 0);

	// 顶点纹理坐标数据流
	NiDataStream* spVertexUVDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_2,
		4,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX,
		spUVSetData);
	NIASSERT(spVertexUVDataStream);
	spMesh->AddStreamRef(spVertexUVDataStream, NiCommonSemantics::TEXCOORD(), 0);

	// 索引数据流
	NiDataStream* spIndexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		6,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX,
		pusIndiceData);
	NIASSERT(spIndexDataStream);
	spMesh->AddStreamRef(spIndexDataStream, NiCommonSemantics::INDEX(), 0);

	// 纹理属性
	NiTexturingPropertyPtr spTexturingProp = NiNew NiTexturingProperty;
	NIASSERT(spTexturingProp);
	spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(spTexture, 0));
	spMesh->AttachProperty(spTexturingProp);

	// 材质
	spMesh->ApplyAndSetActiveMaterial(spMaterial);

	// 更新
	spMesh->Update(0.f);
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();

	return spMesh;
}
//-------------------------------------------------------------------------------------------------
void sdSkyBox::CreateSkyBox()
{
	IResourceSystem* pkResourceSystem = IResourceSystem::InstancePtr();
	NIASSERT(pkResourceSystem);

	//
	float fWidth = m_fSkyBoxWidth;
	float fHeight = m_fSkyBoxHeight;
	float fLength = m_fSkyBoxLength;
	float fBottom = m_fSkyBoxBottom;
	float fHalfWidth = m_fSkyBoxWidth / 2.f;
	float fHalfLength = m_fSkyBoxLength / 2.f;
	float fHalfHeight = m_fSkyBoxHeight /2.f;

	float fF = 0.5f / 400.f;
	float fOne = 1.f - fF;
	float fZero = 0.f + fF;

	NiPoint3 akVertex[4];
	NiPoint2 akVertexUV[4];
	ushort ausIndex[6];

	// Pos-X
	// @{
	NiSourceTexturePtr spTexturePosX = pkResourceSystem->LoadTexture(m_kSkyBoxPosXYPath);
	NIASSERT(spTexturePosX);

	akVertex[0] = NiPoint3(fHalfWidth,  fHalfLength, fHeight);
	akVertex[1] = NiPoint3(fHalfWidth, -fHalfLength, fHeight);
	akVertex[2] = NiPoint3(fHalfWidth,  fHalfLength, fBottom);
	akVertex[3] = NiPoint3(fHalfWidth, -fHalfLength, fBottom);

	akVertexUV[0] = NiPoint2(fOne, fZero);
	akVertexUV[1] = NiPoint2(fZero, fZero);
	akVertexUV[2] = NiPoint2(fOne, fOne / 2.f);
	akVertexUV[3] = NiPoint2(fZero, fOne / 2.f);

	ausIndex[0] = 0; ausIndex[1] = 1; ausIndex[2] = 2;
	ausIndex[3] = 1; ausIndex[4] = 3; ausIndex[5] = 2;

	m_spPosXMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTexturePosX, m_spMaterial);
	NIASSERT(m_spPosXMesh);
	// @}

	// Neg-X
	// @{
	NiSourceTexturePtr spTextureNegX = pkResourceSystem->LoadTexture(m_kSkyBoxNegXYPath);
	NIASSERT(spTextureNegX);

	akVertex[0] = NiPoint3(-fHalfWidth,  fHalfLength, fHeight);
	akVertex[1] = NiPoint3(-fHalfWidth, -fHalfLength, fHeight);
	akVertex[2] = NiPoint3(-fHalfWidth,  fHalfLength, fBottom);
	akVertex[3] = NiPoint3(-fHalfWidth, -fHalfLength, fBottom);

	akVertexUV[0] = NiPoint2(fOne, fZero);
	akVertexUV[1] = NiPoint2(fZero, fZero);
	akVertexUV[2] = NiPoint2(fOne, fOne / 2.f);
	akVertexUV[3] = NiPoint2(fZero, fOne / 2.f);

	ausIndex[0] = 0; ausIndex[1] = 2; ausIndex[2] = 1;
	ausIndex[3] = 1; ausIndex[4] = 2; ausIndex[5] = 3;

	m_spNegXMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTextureNegX, m_spMaterial);
	NIASSERT(m_spNegXMesh);
	// @}

	// Pos-Y
	// @{
	NiSourceTexturePtr spTexturePosY = spTexturePosX;
	NIASSERT(spTexturePosY);

	akVertex[0] = NiPoint3(-fHalfWidth,  fHalfLength, fHeight);
	akVertex[1] = NiPoint3( fHalfWidth,  fHalfLength, fHeight);
	akVertex[2] = NiPoint3(-fHalfWidth,  fHalfLength, fBottom);
	akVertex[3] = NiPoint3( fHalfWidth,  fHalfLength, fBottom);

	akVertexUV[0] = NiPoint2(fOne, fOne);
	akVertexUV[1] = NiPoint2(fZero, fOne);
	akVertexUV[2] = NiPoint2(fOne, fOne / 2.f);
	akVertexUV[3] = NiPoint2(fZero, fOne / 2.f);

	ausIndex[0] = 0; ausIndex[1] = 1; ausIndex[2] = 2;
	ausIndex[3] = 1; ausIndex[4] = 3; ausIndex[5] = 2;

	m_spPosYMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTexturePosY, m_spMaterial);
	NIASSERT(m_spPosYMesh);
	// @}

	// Neg-Y
	// @{
	NiSourceTexturePtr spTextureNegY = spTextureNegX;
	NIASSERT(spTextureNegY);

	akVertex[0] = NiPoint3(-fHalfWidth, -fHalfLength, fHeight);
	akVertex[1] = NiPoint3( fHalfWidth, -fHalfLength, fHeight);
	akVertex[2] = NiPoint3(-fHalfWidth, -fHalfLength, fBottom);
	akVertex[3] = NiPoint3( fHalfWidth, -fHalfLength, fBottom);

	akVertexUV[0] = NiPoint2(fOne, fOne);
	akVertexUV[1] = NiPoint2(fZero, fOne);
	akVertexUV[2] = NiPoint2(fOne, fOne / 2.f);
	akVertexUV[3] = NiPoint2(fZero, fOne / 2.f);

	ausIndex[0] = 0; ausIndex[1] = 2; ausIndex[2] = 1;
	ausIndex[3] = 1; ausIndex[4] = 2; ausIndex[5] = 3;

	m_spNegYMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTextureNegY, m_spMaterial);
	NIASSERT(m_spNegYMesh);
	// @}

	// Pos-Z
	// @{
	NiSourceTexturePtr spTexturePosZ = pkResourceSystem->LoadTexture(m_kSkyBoxPosZPath);
	NIASSERT(spTexturePosZ);

	akVertex[0] = NiPoint3(-fHalfWidth, -fHalfLength, fHeight);
	akVertex[1] = NiPoint3( fHalfWidth, -fHalfLength, fHeight);
	akVertex[2] = NiPoint3(-fHalfWidth,  fHalfLength, fHeight);
	akVertex[3] = NiPoint3( fHalfWidth,  fHalfLength, fHeight);

	akVertexUV[0] = NiPoint2(fOne, fZero);
	akVertexUV[1] = NiPoint2(fZero, fZero);
	akVertexUV[2] = NiPoint2(fOne, fOne);
	akVertexUV[3] = NiPoint2(fZero, fOne);

	ausIndex[0] = 0; ausIndex[1] = 1; ausIndex[2] = 2;
	ausIndex[3] = 1; ausIndex[4] = 3; ausIndex[5] = 2;

	m_spPosZMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTexturePosZ, m_spMaterial);
	NIASSERT(m_spPosZMesh);
	// @}

	// Neg-Z
	// @{
	NiSourceTexturePtr spTextureNegZ = spTexturePosZ;
	NIASSERT(spTextureNegZ);

	akVertex[0] = NiPoint3(-fHalfWidth, -fHalfLength, fBottom);
	akVertex[1] = NiPoint3( fHalfWidth, -fHalfLength, fBottom);
	akVertex[2] = NiPoint3(-fHalfWidth,  fHalfLength, fBottom);
	akVertex[3] = NiPoint3( fHalfWidth,  fHalfLength, fBottom);

	akVertexUV[0] = NiPoint2(fZero, fZero);
	akVertexUV[1] = NiPoint2(fOne, fZero);
	akVertexUV[2] = NiPoint2(fZero, fOne);
	akVertexUV[3] = NiPoint2(fOne, fOne);

	ausIndex[0] = 0; ausIndex[1] = 1; ausIndex[2] = 2;
	ausIndex[3] = 1; ausIndex[4] = 3; ausIndex[5] = 2;

	m_spNegZMesh = CreateQuad(akVertex, akVertexUV, ausIndex, spTextureNegZ, m_spMaterial);
	NIASSERT(m_spNegZMesh);
	// @}
}
//-------------------------------------------------------------------------------------------------