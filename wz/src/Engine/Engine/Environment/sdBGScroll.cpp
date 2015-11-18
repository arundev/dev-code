#include "sdEnginePCH.h"
#include "sdBGScroll.h"

#include "IRenderDevice.h"
#include "IResourceSystem.h"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
sdBGScroll::sdBGScroll()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	m_kInnerScrollTexPath = "edata\\world0\\texture\\weather\\yuanshan\\shan_02.dds";
	m_spInnerScrollTexture = pkResourceSystem->LoadTexture(m_kInnerScrollTexPath);
	NIASSERT(m_spInnerScrollTexture);

	m_kOuterScrollTexPath = "edata\\world0\\texture\\weather\\yuanshan\\shan_05.dds";
	m_spOutterScrollTexture = pkResourceSystem->LoadTexture(m_kOuterScrollTexPath);
	NIASSERT(m_spOutterScrollTexture);

	m_spMaterial = pkRenderDevice->CreateMaterial("Environment_BackgroundScroll");
	NIASSERT(m_spMaterial);

	// [alpha][height][u_wrap][z_scale][v_bias]
	float vExtraData[5] = {1.f, 10.f, 8.f, 0.999996f, 0.f};
	m_spScrollMeshExtraData = NiNew NiFloatsExtraData(5, vExtraData);
	NIASSERT(m_spScrollMeshExtraData);

	m_fInnerScrollAlpha = 1.f;
	m_fInnerScrollHeight = 0.5f;
	m_fInnerScrollUWrap = 1.f;
	m_fOuterScrollAlpha = 1.f;
	m_fOuterScrollHeight = 2.f;
	m_fOuterScrollUWrap = 0.f;

	CreateBackGroundScroll();
}
//-------------------------------------------------------------------------------------------------
sdBGScroll::~sdBGScroll()
{

}
//-------------------------------------------------------------------------------------------------
void sdBGScroll::Render()
{
	D3DPERF_BeginEvent(0xff000000, L"Environment_BackgroundScroll");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_INVSRCALPHA);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// 绘制
	// @{
	// 绘制OutterScroll
	// [alpha][height][u_wrap][z_scale][v_bias]
	m_spScrollMeshExtraData->SetValue(0, m_fOuterScrollAlpha);
	m_spScrollMeshExtraData->SetValue(1, m_fOuterScrollHeight);
	m_spScrollMeshExtraData->SetValue(2, m_fOuterScrollUWrap);
	m_spScrollMeshExtraData->SetValue(3, 0.999996f);
	m_spScrollMeshExtraData->SetValue(4, 1.f);
	m_spTexturingProp->GetShaderMap(0)->SetTexture(m_spOutterScrollTexture);
	m_spScrollMesh->RenderImmediate(spRenderer);

	// 绘制InnerScroll
	m_spScrollMeshExtraData->SetValue(0, m_fInnerScrollAlpha);
	m_spScrollMeshExtraData->SetValue(1, m_fInnerScrollHeight);
	m_spScrollMeshExtraData->SetValue(2, m_fInnerScrollUWrap);
	m_spScrollMeshExtraData->SetValue(3, 0.999994f);
	m_spScrollMeshExtraData->SetValue(4, 3.f);
	m_spTexturingProp->GetShaderMap(0)->SetTexture(m_spInnerScrollTexture);
	m_spScrollMesh->RenderImmediate(spRenderer);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdBGScroll::CreateBackGroundScroll()
{
	// 参数
	uint uiSegNum = 20;
	float fRadius = 10000.f;
	float fTop = 1000.f;
	float fBottom = -1000.f;

	//
	uint uiVertexNum = 2 * (uiSegNum + 1);
	uint uiTriNum = 2 * uiSegNum;
	uint uiIndexNum = uiTriNum * 3;

	// 构建顶点数据
	NiPoint3* aspVertexData = NiNew NiPoint3[uiVertexNum];
	NIASSERT(aspVertexData);

	NiPoint2* aspVertexUVData = NiNew NiPoint2[uiVertexNum];
	NIASSERT(aspVertexUVData);

	float fUnitAngle = 2.f * NI_PI / (float)uiSegNum;
	float fUVStep = 1.f / (float)uiSegNum;
	NiPoint3 kPos = NiPoint3::ZERO;
	NiPoint2 kUV = NiPoint2::ZERO;
	for (uint i = 0, uiVertexIndex = 0; i <= uiSegNum; ++i, uiVertexIndex+=2)
	{
		kPos.x = cos(fUnitAngle * i) * fRadius;
		kPos.y = sin(fUnitAngle * i) * fRadius;
		kPos.z = fBottom;	aspVertexData[uiVertexIndex] = kPos;
		kPos.z = fTop;		aspVertexData[uiVertexIndex+1] = kPos;

		kUV.x = i * fUVStep;
		kUV.y =	1.f;	aspVertexUVData[uiVertexIndex] = kUV;
		kUV.y =	0.f;	aspVertexUVData[uiVertexIndex+1] = kUV;
	}

	// 构建索引数据
	ushort* apusIndexData = NiAlloc(ushort, uiIndexNum);
	for (uint i = 0, uiIndexIndex = 0; i < uiSegNum; ++i, uiIndexIndex+=6)
	{
		apusIndexData[uiIndexIndex] = i*2;
		apusIndexData[uiIndexIndex+1] = (i+1)*2+1;
		apusIndexData[uiIndexIndex+2] = i*2+1;

		apusIndexData[uiIndexIndex+3] = i*2;
		apusIndexData[uiIndexIndex+4] = (i+1)*2;
		apusIndexData[uiIndexIndex+5] = (i+1)*2+1;
	}

	// 构建Mesh
	// @{
	NiMeshPtr spMesh = NiNew NiMesh();
	NIASSERT(spMesh);
	m_spScrollMesh = spMesh;

#ifdef SDDEBUG
	spMesh->SetName("BackGroundScroll");
#endif

	spMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	// 顶点数据流
	NiDataStream* spVertexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiVertexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX,
		aspVertexData);
	NIASSERT(spVertexDataStream);
	spMesh->AddStreamRef(spVertexDataStream, NiCommonSemantics::POSITION(), 0);

	// 顶点纹理坐标数据流
	NiDataStream* spVertexUVDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_2,
		uiVertexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX,
		aspVertexUVData);
	NIASSERT(spVertexUVDataStream);
	spMesh->AddStreamRef(spVertexUVDataStream, NiCommonSemantics::TEXCOORD(), 0);

	// 索引数据流
	NiDataStream* spIndexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiIndexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX,
		apusIndexData);
	NIASSERT(spIndexDataStream);
	spMesh->AddStreamRef(spIndexDataStream, NiCommonSemantics::INDEX(), 0);

	// 纹理属性
	m_spTexturingProp = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	spMesh->AttachProperty(m_spTexturingProp);

	// 材质
	spMesh->ApplyAndSetActiveMaterial(m_spMaterial);

	// 渲染数据
	spMesh->AddExtraData("a_akBGScrollParams", m_spScrollMeshExtraData);

	// 更新
	spMesh->Update(0.f);
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();
	// @}

	NiDelete [] aspVertexData;
	NiDelete [] aspVertexUVData;
	NiFree(apusIndexData);
}
//-------------------------------------------------------------------------------------------------