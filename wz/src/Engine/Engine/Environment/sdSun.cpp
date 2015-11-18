#include "sdEnginePCH.h"
#include "sdSun.h"

#include "IRenderDevice.h"
#include "IResourceSystem.h"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
sdSun::sdSun()
{	
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	// 对象
	// @{
	// 顶点数据
	float fVertexArray[4 * (3 + 2)];
	fVertexArray[0] = -1;  fVertexArray[1] =  1;  fVertexArray[2] = 1;
	fVertexArray[5] =  1;  fVertexArray[6] =  1;  fVertexArray[7] = 1;
	fVertexArray[10] = -1; fVertexArray[11] = -1; fVertexArray[12] = 1;
	fVertexArray[15] =  1; fVertexArray[16] = -1; fVertexArray[17] = 1;

	fVertexArray[3] = 0;  fVertexArray[4] = 0;
	fVertexArray[8] = 1;  fVertexArray[9] = 0;
	fVertexArray[13] = 0; fVertexArray[14] = 1;
	fVertexArray[18] = 1; fVertexArray[19] = 1;

	// 索引数据
	ushort usIndex[6] = {0, 1, 2, 1, 2, 3};

	// Mesh
	NiMesh* spMesh = NiNew NiMesh();
	NIASSERT(spMesh);

#ifdef SDDEBUG
	spMesh->SetName("Sun");
#endif

	spMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	// 顶点数据流
	NiDataStreamElementSet kVertexElementSet;
	kVertexElementSet.AddElement(NiDataStreamElement::F_FLOAT32_3);
	kVertexElementSet.AddElement(NiDataStreamElement::F_FLOAT32_2);

	NiDataStream* spVertexDataStream = NiDataStream::CreateDataStream(
		kVertexElementSet,
		4, 
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX);
	NIASSERT(spVertexDataStream);

	NiDataStream::Region kRegion(0, 4);
	spVertexDataStream->AddRegion(kRegion);

	NiDataStream::LockType eLock = NiDataStream::LOCK_WRITE;
	void *pvData = spVertexDataStream->Lock((NiUInt8)eLock);
	NiMemcpy(pvData, fVertexArray, spVertexDataStream->GetSize());
	spVertexDataStream->Unlock((NiUInt8)eLock);

	NiDataStreamRef* spVertexDataStreamRef = spMesh->AddStreamRef();
	NIASSERT(spVertexDataStreamRef);
	spVertexDataStreamRef->SetDataStream(spVertexDataStream);
	spVertexDataStreamRef->SetSubmeshCount(1);
	spVertexDataStreamRef->BindSemanticToElementDescAt(0, NiCommonSemantics::POSITION(), 0);
	spVertexDataStreamRef->BindSemanticToElementDescAt(1, NiCommonSemantics::TEXCOORD(), 0);
	spVertexDataStreamRef->BindRegionToSubmesh(0, kRegion);

	// 索引数据流
	NiDataStream* spIndexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		6,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX,
		usIndex);
	NIASSERT(spIndexDataStream);
	spMesh->AddStreamRef(spIndexDataStream, NiCommonSemantics::INDEX(), 0);

	// 纹理属性
	NiTexturePtr spTexture = pkResourceSystem->LoadTexture("dependence\\engine\\texture\\environment\\sun.dds");
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
	//spAlphaProp->SetAlphaBlending(false);
	//spMesh->AttachProperty(spAlphaProp);

	//// Z属性
	//NiZBufferPropertyPtr spZBuffProp = NiNew NiZBufferProperty;
	//NIASSERT(spZBuffProp);
	//spZBuffProp->SetZBufferTest(false);
	//spZBuffProp->SetZBufferWrite(false);
	//spMesh->AttachProperty(spZBuffProp);

	// 材质
	NiMaterialPtr spMaterial = pkRenderDevice->CreateMaterial("Environment_Sun");
	NIASSERT(spMaterial);
	spMesh->ApplyAndSetActiveMaterial(spMaterial);

	// 
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();
	spMesh->Update(0.f);

	//
	m_spMesh = spMesh;
	// @}
}
//-------------------------------------------------------------------------------------------------
sdSun::~sdSun()
{
	m_spMesh = 0;
}
//-------------------------------------------------------------------------------------------------
void sdSun::SetDepthTexture(NiTexture* spTexture)
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
void sdSun::Render()
{
	D3DPERF_BeginEvent(0xff000000, L"Environment_Sun");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);;

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
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