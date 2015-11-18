#include "sdEnginePCH.h"
#include "sdTail.h"

// 刀光有时候需要插值提高精度,需要实现分配一个较大的Buffer
#define MAX_VERTEX_NUM	4096

ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM

NiImplementRTTI(sdTail, NiNode);
//---------------------------------------------------------------------------------------------
sdTail::sdTail()
: m_bPlaying(false)
, m_spDummy0(NULL)
, m_spDummy1(NULL)
{

}
//---------------------------------------------------------------------------------------------
sdTail::~sdTail()
{

}
//---------------------------------------------------------------------------------------------
bool sdTail::Create(float fMaxTailLength, float fSegmentLength)
{
	if (fMaxTailLength < 0.f || fSegmentLength < 0.f)
		return false;

	m_fMaxTailLength = fMaxTailLength;
	m_fSegmentLength = fSegmentLength;

	m_spGeometry0 = CreateMesh(fMaxTailLength, fSegmentLength);
	NIASSERT(m_spGeometry0);

	m_spGeometry1 = CreateMesh(fMaxTailLength, fSegmentLength);
	NIASSERT(m_spGeometry1);

	return true;
}
//---------------------------------------------------------------------------------------------
void sdTail::Start()
{
	if (!m_bPlaying)
	{


		m_bPlaying = true;
	}
}
//---------------------------------------------------------------------------------------------
void sdTail::Stop()
{
	if (m_bPlaying)
	{
		m_fFadeOuting = 1.f;
		m_bPlaying = false;
	}
}
//---------------------------------------------------------------------------------------------
void sdTail::UpdateFrame(float fAccumTime, float fFrameTime)
{
	
}
//---------------------------------------------------------------------------------------------
void sdTail::SetDummy(NiNode* spDummy0, NiNode* spDummy1)
{
	m_spDummy0 = spDummy0;
	m_spDummy1 = spDummy1;
}
//---------------------------------------------------------------------------------------------
void sdTail::SetTexture(NiTexture* spTexture0, NiTexture* spTexture1)
{
	if (m_spGeometry0)
	{
		NiProperty* spProp = m_spGeometry0->GetProperty(NiProperty::TEXTURING);
		NiTexturingProperty* pkTexProp = (NiTexturingProperty*)spProp;
		if (pkTexProp)
			pkTexProp->SetBaseTexture(spTexture0);
	}

	if (m_spGeometry1)
	{
		NiProperty* spProp = m_spGeometry1->GetProperty(NiProperty::TEXTURING);
		NiTexturingProperty* pkTexProp = (NiTexturingProperty*)spProp;
		if (pkTexProp)
			pkTexProp->SetBaseTexture(spTexture1);
	}
}
//---------------------------------------------------------------------------------------------
NiMeshPtr sdTail::CreateMesh(float fMaxTailLength, float fSegmentLength)
{
	if (fMaxTailLength < 0.f || fSegmentLength < 0.f)
		return 0;

	// 构建Mesh
	NiMeshPtr spMesh = NiNew NiMesh;
	NIASSERT(spMesh);

#ifdef SDDEBUG
	spMesh->SetName("Tail");
#endif

	spMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRISTRIPS);

	// 顶点数据流
	NiDataStreamElementSet kElementSet;
	kElementSet.AddElement(NiDataStreamElement::F_FLOAT32_3);
	kElementSet.AddElement(NiDataStreamElement::F_FLOAT32_4);
	kElementSet.AddElement(NiDataStreamElement::F_FLOAT32_2);

	uint uiVertexNum = MAX_VERTEX_NUM;
	NiDataStream* spVertexDataStream = NiDataStream::CreateDataStream(
		kElementSet,
		uiVertexNum,
		NiDataStream::ACCESS_CPU_READ | NiDataStream::ACCESS_CPU_WRITE_MUTABLE | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX);
	NIASSERT(spVertexDataStream);

	NiDataStream::Region kRegion(0, 3);
	spVertexDataStream->AddRegion(kRegion);

	NiDataStreamRef* spVertexDataStreamRef = spMesh->AddStreamRef();
	NIASSERT(spVertexDataStreamRef);
	spVertexDataStreamRef->SetDataStream(spVertexDataStream);
	spVertexDataStreamRef->SetSubmeshCount(1);
	spVertexDataStreamRef->BindSemanticToElementDescAt(0, NiCommonSemantics::POSITION(), 0);
	spVertexDataStreamRef->BindSemanticToElementDescAt(1, NiCommonSemantics::COLOR(), 0);
	spVertexDataStreamRef->BindSemanticToElementDescAt(2, NiCommonSemantics::TEXCOORD(), 0);
	spVertexDataStreamRef->BindRegionToSubmesh(0, kRegion);
	spVertexDataStreamRef->SetActiveCount(0, 0);

	// 顶点数据流写入
	NiDataStream::LockType eLock = NiDataStream::LOCK_WRITE;
	void *pvVertexData = spVertexDataStream->Lock((NiUInt8)eLock);
	
	NiColorA kInitColorA(1.f, 1.f, 1.f, 0.f);
	ushort usMaxPoint = (uint)(fMaxTailLength / fSegmentLength);
	stTailVertex* pkVertexData = (stTailVertex*)pvVertexData;
	stTailVertex* pkNextVertexData = pkVertexData + 1;
	for (ushort i = 0; i < usMaxPoint; ++i)
	{
		pkVertexData->pos = NiPoint3::ZERO;
		pkNextVertexData->pos = NiPoint3::ZERO;

		pkVertexData->color = NiColorA::WHITE;
		pkNextVertexData->color = NiColorA::WHITE;

		pkVertexData->uv.x = i / (usMaxPoint - 1.f);
		pkVertexData->uv.y = 0.f;

		pkNextVertexData->uv.x = i / (usMaxPoint - 1.f);
		pkNextVertexData->uv.y = 0.f;

		pkVertexData +=2;
		pkNextVertexData +=2;
	}

	spVertexDataStream->Unlock((NiUInt8)eLock);

	// 索引数据流
	NiDataStream* spIndexDataStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiVertexNum,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_WRITE_STATIC,
		NiDataStream::USAGE_VERTEX_INDEX);
	NIASSERT(spIndexDataStream);
	
	NiDataStreamRef* spIndexDataStreamRef = spMesh->AddStreamRef(spIndexDataStream, NiCommonSemantics::INDEX(), 0);
	NIASSERT(spIndexDataStreamRef);
	spIndexDataStreamRef->SetActiveCount(0, 0);

	// 索引数据流写入
	NiDataStreamElementLock kIndexLock(spMesh,
		NiCommonSemantics::INDEX(), 0, 
		NiDataStreamElement::F_UINT16_1, 
		NiDataStream::LOCK_WRITE);
	NIASSERT(kIndexLock.IsLocked());

	NiTStridedRandomAccessIterator<ushort> kIndexItr = kIndexLock.begin<ushort>();
	for (ushort i = 0; i < usMaxPoint; ++i)
		kIndexItr[i] = i;

	// 纹理属性
	NiTexturingPropertyPtr spTexProp = NiNew NiTexturingProperty;
	NIASSERT(spTexProp);
	spTexProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
	spTexProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);
	spTexProp->SetBaseFilterMode(NiTexturingProperty::FILTER_TRILERP);
	spMesh->AttachProperty(spTexProp);

	// 顶点属性
	NiVertexColorPropertyPtr spVertexColorProp = NiNew NiVertexColorProperty;
	NIASSERT(spVertexColorProp);
	spVertexColorProp->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
	spVertexColorProp->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	spMesh->AttachProperty(spVertexColorProp);

	// 材质属性
	NiMaterialPropertyPtr spMatProp = NiNew NiMaterialProperty;
	NIASSERT(spMatProp);
	spMatProp->SetEmittance(NiColor::WHITE);
	spMesh->AttachProperty(spMatProp);

	// 模板属性
	NiStencilPropertyPtr spStencilProp = NiNew NiStencilProperty;
	NIASSERT(spStencilProp);
	spStencilProp->SetDrawMode(NiStencilProperty::DRAW_BOTH);
	spMesh->AttachProperty(spStencilProp);

	// 透明属性
	NiAlphaPropertyPtr spAlphaProp = NiNew NiAlphaProperty;
	NIASSERT(spAlphaProp);
	spAlphaProp->SetAlphaTesting(false);
	spAlphaProp->SetAlphaBlending(true);
	spAlphaProp->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	spAlphaProp->SetDestBlendMode(NiAlphaProperty::ALPHA_ONE);
	spMesh->AttachProperty(spAlphaProp);

	// Z属性
	NiZBufferPropertyPtr spZBuffProp = NiNew NiZBufferProperty;
	NIASSERT(spZBuffProp);
	spZBuffProp->SetZBufferTest(true);
	spZBuffProp->SetZBufferWrite(false);
	spMesh->AttachProperty(spZBuffProp);

	// 
	spMesh->Update(0.f);
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();


	spMesh->SetAppCulled(true);

	// 挂接到父节点
	AttachChild(spMesh);
	Update(0.f);
	UpdateProperties();
	UpdateEffects();
	UpdateNodeBound();

	return spMesh;
}
//---------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM