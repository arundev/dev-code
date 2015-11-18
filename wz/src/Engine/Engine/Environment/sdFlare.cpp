#include "sdEnginePCH.h"
#include "sdFlare.h"

#include "IRenderDevice.h"
#include "IResourceSystem.h"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
sdFlare::sdFlare()
{	
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	// 对象
	// @{
	uint uiVertexNum = 4 * 7;	// 7个Flare,每个Flare4个顶点
	uint uiIndexNum = 6 * 7;	// 7个Flare,每个Flare2个三角形6个索引

	NiPoint3 kArr[4];
	kArr[0]= NiPoint3(-0.2f, 0.2f, 0.f);
	kArr[1]= NiPoint3(0.2f, 0.2f, 1.f);
	kArr[2]= NiPoint3(-0.2f, -0.2f, 2.f);
	kArr[3]= NiPoint3(0.2f, -0.2f, 3.f);

	float fScale[7] = { 1.f, 0.7f, 0.3f, 0.15f, 0.45f, 1.0f, 2.f};
	
	// 顶点数据
	NiPoint3* spVertexArray = NiNew NiPoint3[uiVertexNum];
	for (uint ui = 0; ui < 7; ++ui)
	{
		for (uint uj = 0; uj < 4; ++uj)
		{
			spVertexArray[ui * 4 + uj] = kArr[uj] * fScale[ui];
			spVertexArray[ui * 4 + uj].z = (float)ui;
		}
	}

	// 索引数据
	uint uiRectNum = uiVertexNum / 4;
	ushort* pusIndexArray = NiAlloc(ushort, uiIndexNum);
	for (uint ui = 0; ui < uiRectNum; ++ui)
	{
		pusIndexArray[ui * 6 + 0] = ui * 4 + 0;
		pusIndexArray[ui * 6 + 1] = ui * 4 + 1;
		pusIndexArray[ui * 6 + 2] = ui * 4 + 2;
		pusIndexArray[ui * 6 + 3] = ui * 4 + 1;
		pusIndexArray[ui * 6 + 4] = ui * 4 + 2;
		pusIndexArray[ui * 6 + 5] = ui * 4 + 3;
	}

	// Mesh
	NiMesh* spMesh = NiNew NiMesh();
	NIASSERT(spMesh);

#ifdef SDDEBUG
	spMesh->SetName("Flare");
#endif

	spMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	// 顶点数据流
	NiDataStream* spVertexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiVertexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX,
		spVertexArray);
	NIASSERT(spVertexDataStream);
	spMesh->AddStreamRef(spVertexDataStream, NiCommonSemantics::POSITION(), 0);

	// 索引数据流
	NiDataStream* spIndexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiIndexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX,
		pusIndexArray);
	NIASSERT(spIndexDataStream);
	spMesh->AddStreamRef(spIndexDataStream, NiCommonSemantics::INDEX(), 0);

	// 纹理属性
	NiTexturePtr spTexture = pkResourceSystem->LoadTexture("dependence\\engine\\texture\\environment\\flare.dds");
	NIASSERT(spTexture);

	NiTexturingPropertyPtr spTexturingProp = NiNew NiTexturingProperty;
	NIASSERT(spTexturingProp);
	spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(spTexture, 0));
	spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	spMesh->AttachProperty(spTexturingProp);

	//// 透明属性
	//NiAlphaPropertyPtr spAlphaProp = NiNew NiAlphaProperty;
	//NIASSERT(spAlphaProp);
	//spAlphaProp->SetAlphaTesting(false);
	//spAlphaProp->SetAlphaBlending(true);
	//spAlphaProp->SetSrcBlendMode(NiAlphaProperty::ALPHA_ONE);
	//spAlphaProp->SetDestBlendMode(NiAlphaProperty::ALPHA_ONE);
	//spMesh->AttachProperty(spAlphaProp);

	//// Z属性
	//NiZBufferPropertyPtr spZBuffProp = NiNew NiZBufferProperty;
	//NIASSERT(spZBuffProp);
	//spZBuffProp->SetZBufferTest(false);
	//spZBuffProp->SetZBufferWrite(false);
	//spMesh->AttachProperty(spZBuffProp);

	// 材质
	NiMaterialPtr spMaterial = pkRenderDevice->CreateMaterial("Environment_Flare");
	NIASSERT(spMaterial);
	spMesh->ApplyAndSetActiveMaterial(spMaterial);

	// Extra数据
	float fExtraData[7] = {0.95f, 0.75f, 0.55f, 0.35f, 0.1f, -0.4f, 1.0f};
	spMesh->AddExtraData("a_fBoardDistance", NiNew NiFloatsExtraData(7, fExtraData));

	float fTexCol[7] = {3,3,1, 0, 1, 2, 3};
	spMesh->AddExtraData("a_fTexCol", NiNew NiFloatsExtraData(7, fTexCol));

	float fAlpha[7] = {0.3f, 0.4f, 0.5f, 1.0f, 0.9f, 0.4f, 0.2f};
	spMesh->AddExtraData("a_fAlpha", NiNew NiFloatsExtraData(7, fAlpha));

	// 
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();
	spMesh->Update(0.f);

	//
	NiDelete [] spVertexArray;
	spVertexArray = NULL;

	NiFree(pusIndexArray);
	pusIndexArray = NULL;

	//
	m_spMesh = spMesh;
	// @}
}
//-------------------------------------------------------------------------------------------------
sdFlare::~sdFlare()
{
	m_spMesh = 0;
}
//-------------------------------------------------------------------------------------------------
void sdFlare::SetDepthTexture(NiTexture* spTexture)
{
	NiProperty* spProp = m_spMesh->GetProperty(NiProperty::TEXTURING);
	NiTexturingProperty* spTexProp = (NiTexturingProperty*)spProp;
	if (spTexProp)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = spTexProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFlare::Render()
{
	D3DPERF_BeginEvent(0xff000000, L"Environment_Flare");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);
	pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// 绘制
	// @{
	m_spMesh->RenderImmediate(spRenderer);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------