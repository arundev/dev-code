#include "sdEnginePCH.h"
#include "sdCloud.h"

#include "IRenderDevice.h"
#include "IResourceSystem.h"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
sdCloud::sdCloud()
{
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	m_kCloudTexPath = "edata\\world0\\texture\\weather\\cloud\\cloud_01.dds";
	m_spCloudTexture = pkResourceSystem->LoadTexture(m_kCloudTexPath);
	NIASSERT(m_spCloudTexture);

	m_spMaterial = pkRenderDevice->CreateMaterial("Environment_SkyDomeCloud");
	NIASSERT(m_spMaterial);

	m_fCloudSpeed = 0.15f;
	m_fCloudWrap = 0.5f;
	m_fCloudDomeZBias = 0.f;
	m_fCloudDomeAlpha = 0.f;

	// [cloud_wrap][cloud_speed][z_bias][alpha]
	float vExtraData[4] = { 0 };
	vExtraData[0] = m_fCloudWrap;
	vExtraData[1] = m_fCloudSpeed;
	vExtraData[2] = m_fCloudDomeZBias;
	vExtraData[3] = m_fCloudDomeAlpha;
	m_spExtraData = NiNew NiFloatsExtraData(4, vExtraData);
	NIASSERT(m_spExtraData);

	CreateCloud();
}
//-------------------------------------------------------------------------------------------------
sdCloud::~sdCloud()
{

}
//-------------------------------------------------------------------------------------------------
void sdCloud::Render()
{
	D3DPERF_BeginEvent(0xff000000, L"Environment_CloudDome");

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
	m_spMesh->RenderImmediate(spRenderer);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdCloud::CreateCloud()
{
	// 参数
	float fCenterZ = -2500.f;
	float fRadius = 10000.f;
	float fHeight = 5000.f;
	uint uiLongitudeNum = 24;	// 经度
	uint uiLatitudeNum = 12;	// 维度
	float fUVScale = 0.001f;

	//
	uint uiVertexNum = uiLongitudeNum * uiLatitudeNum + 1;
	uint uiTriNum = (2 * uiLatitudeNum - 1) * uiLongitudeNum;
	uint uiIndexNum = uiTriNum * 3;

	// 构建顶点数据(使用简单平面纹理映射)
	NiPoint3* aspVertexData = NiNew NiPoint3[uiVertexNum];
	NIASSERT(aspVertexData);

	NiPoint3* aspVertexUVData = NiNew NiPoint3[uiVertexNum];
	NIASSERT(aspVertexUVData);

	float fBowLen = sqrt(fRadius * fRadius - (fRadius - fHeight) * (fRadius - fHeight)) / uiLatitudeNum;
	float fXYTheta = 2 * NI_PI / uiLongitudeNum;

	aspVertexData[0] = NiPoint3(0, 0, fHeight + fCenterZ);
	aspVertexUVData[0] = NiPoint3(0, 0, 1.f);
	for (uint i = 1, uiVertexIndex = 1; i <= uiLatitudeNum; ++i)
	{
		float fR = i * fBowLen;
		float fSinBow = fR / fRadius;
		float fTexBias = 1.f / sin((NI_PI - asin(fSinBow)) / 2.f);

		for (uint j = 0; j < uiLongitudeNum; ++j)
		{
			float fLocalHeight = sqrt(fRadius * fRadius - fR * fR) - (fRadius - fHeight);

			aspVertexData[uiVertexIndex].x = cos(fXYTheta * j) * fR;
			aspVertexData[uiVertexIndex].y = sin(fXYTheta * j) * fR;
			aspVertexData[uiVertexIndex].z = fLocalHeight + fCenterZ;

			aspVertexUVData[uiVertexIndex].x = aspVertexData[uiVertexIndex].x * fUVScale * fTexBias;
			aspVertexUVData[uiVertexIndex].y = aspVertexData[uiVertexIndex].y * fUVScale * fTexBias;
			aspVertexUVData[uiVertexIndex].z = fLocalHeight / fHeight;

			++uiVertexIndex;
		}
	}

	// 构建索引数据
	ushort* apusIndexData = NiAlloc(ushort, uiIndexNum);

	uint uiIndexIndex = 0;
	for (uint i = 0; i < uiLongitudeNum; ++i)
	{
		apusIndexData[uiIndexIndex] = 0;
		apusIndexData[uiIndexIndex+1] = i+1;
		if (i != uiLongitudeNum - 1)
			apusIndexData[uiIndexIndex+2] = i+2;
		else
			apusIndexData[uiIndexIndex+2] = 1;

		uiIndexIndex += 3;
	}

	for (uint i = 1; i < uiLatitudeNum; ++i)
	{
		for (uint j = 1; j < uiLongitudeNum; ++j)
		{
			apusIndexData[uiIndexIndex] = (uiLongitudeNum*i) + (j%uiLongitudeNum) - (uiLongitudeNum-1);
			apusIndexData[uiIndexIndex+1] = (uiLongitudeNum*(i+1)) + (j%uiLongitudeNum) - (uiLongitudeNum-1);
			apusIndexData[uiIndexIndex+2] = (uiLongitudeNum*i) + ((j+1)%uiLongitudeNum) - (uiLongitudeNum-1);

			apusIndexData[uiIndexIndex+3] = (uiLongitudeNum*(i+1)) + (j%uiLongitudeNum) - (uiLongitudeNum-1);
			apusIndexData[uiIndexIndex+4] = (uiLongitudeNum*i) + ((j+1)%uiLongitudeNum) - (uiLongitudeNum-1);
			apusIndexData[uiIndexIndex+5] = (uiLongitudeNum*(i+1)) + ((j+1)%uiLongitudeNum) - (uiLongitudeNum-1);
			
			uiIndexIndex += 6;
		}
	}

	// 构建Mesh
	// @{
	NiMeshPtr spMesh = NiNew NiMesh();
	NIASSERT(spMesh);
	m_spMesh = spMesh;

#ifdef SDDEBUG
	spMesh->SetName("DomeCloud");
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
		NiDataStreamElement::F_FLOAT32_3,
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
	NiTexturingPropertyPtr spTexturingProp = NiNew NiTexturingProperty;
	NIASSERT(spTexturingProp);
	spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spCloudTexture, 0));
	spMesh->AttachProperty(spTexturingProp);

	// 材质
	spMesh->ApplyAndSetActiveMaterial(m_spMaterial);

	// 渲染数据
	spMesh->AddExtraData("a_vSkyDomeCloudParams", m_spExtraData);

	// 更新
	spMesh->Update(0.f);
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();
	// @}

	NiDelete [] aspVertexData;
	NiDelete [] aspVertexUVData;
	NiFree(apusIndexData);
}
//-------------------------------------------------------------------------------------------------#include "sdEnginePCH.h"