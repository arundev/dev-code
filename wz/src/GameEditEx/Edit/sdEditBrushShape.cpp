#include "sdGameEditExPCH.h"
#include "sdEditBrushShape.h"

#include <sdMap.h>
#include <sdTerrain.h>
#include <IRenderSystem.h>

using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
sdDualCircleMesh::sdDualCircleMesh()
: m_fInnerRadius(5.0f)
, m_fOuterRadius(10.0f)
{
	CreateInnerCircle();
	CreateOutterCirlce();
}
//-------------------------------------------------------------------------------------------------
sdDualCircleMesh::~sdDualCircleMesh()
{
	m_spInnerCircle = 0;
	m_spOuterCircle = 0;
}
//-------------------------------------------------------------------------------------------------
void sdDualCircleMesh::UpdateShape()
{
	UpdateInnerCircle();
	UpdateOutterCirlce();
}
//-------------------------------------------------------------------------------------------------
void sdDualCircleMesh::CreateInnerCircle()
{
	m_spInnerCircle = NiNew NiMesh;
	NIASSERT(m_spInnerCircle);

	m_spInnerCircle->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_LINESTRIPS);	// PRIMITIVE_LINES
	m_spInnerCircle->SetName("BrushInnerCircle");

	// 顶点数据流
	NiDataStreamElementLock kVertexLock = m_spInnerCircle->AddStreamGetLock(
		NiCommonSemantics::POSITION(), 0,
		NiDataStreamElement::F_FLOAT32_3,
		ms_uiCirclePointCount + 1,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_READ | NiDataStream::ACCESS_CPU_WRITE_MUTABLE,
		NiDataStream::USAGE_VERTEX,
		false, true);
	NiTStridedRandomAccessIterator<NiPoint3> kVertexItr = kVertexLock.begin<NiPoint3>();

	float fDeltaRadian = NI_PI * 2.0f / (float)ms_uiCirclePointCount;
	float fCurRadian = 0.0f;
	for (uint i = 0; i <= ms_uiCirclePointCount; ++i)
	{
		kVertexItr[i].x = sin(-fCurRadian);
		kVertexItr[i].y = cos(-fCurRadian);
		kVertexItr[i].z = 0.0f;

		fCurRadian += fDeltaRadian;
	}

	kVertexLock.Unlock();

	// 子Mesh
	m_spInnerCircle->SetSubmeshCount(1);

	// 设置包围盒为无穷大
	NiBound kBound;
	kBound.SetCenter(NiPoint3::ZERO);
	kBound.SetRadius(FLT_MAX);
	m_spInnerCircle->SetWorldBound(kBound);

	// 设置顶点颜色
	NiVertexColorPropertyPtr spVertexProp = NiNew NiVertexColorProperty;
	NIASSERT(spVertexProp);
	spVertexProp->SetSourceMode(NiVertexColorProperty::SOURCE_IGNORE);
	spVertexProp->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	m_spInnerCircle->AttachProperty(spVertexProp);

	// 设置自发光
	NiMaterialPropertyPtr spMaterialProp = NiNew NiMaterialProperty;
	NIASSERT(spMaterialProp);
	spMaterialProp->SetEmittance(NiColor(1.0f, 0.0f, 0.0f));
	m_spInnerCircle->AttachProperty(spMaterialProp);

	// 设置Z
	NiZBufferPropertyPtr spZBuffProp = NiNew NiZBufferProperty;
	NIASSERT(spZBuffProp);
	spZBuffProp->SetZBufferTest(true);
	spZBuffProp->SetZBufferWrite(true);
	m_spInnerCircle->AttachProperty(spZBuffProp);

	// 更新
	m_spInnerCircle->UpdateProperties();
	m_spInnerCircle->UpdateEffects();
	m_spInnerCircle->Update(0.0f);

	//
	AttachChild(m_spInnerCircle);
}
//-------------------------------------------------------------------------------------------------
void sdDualCircleMesh::CreateOutterCirlce()
{
	m_spOuterCircle = NiNew NiMesh;
	NIASSERT(m_spOuterCircle);

	m_spOuterCircle->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_LINESTRIPS);	// PRIMITIVE_LINES
	m_spOuterCircle->SetName("BrushInnerCircle");

	// 顶点数据流
	NiDataStreamElementLock kVertexLock = m_spOuterCircle->AddStreamGetLock(
		NiCommonSemantics::POSITION(), 0,
		NiDataStreamElement::F_FLOAT32_3,
		ms_uiCirclePointCount + 1,
		NiDataStream::ACCESS_GPU_READ | NiDataStream::ACCESS_CPU_READ | NiDataStream::ACCESS_CPU_WRITE_MUTABLE,
		NiDataStream::USAGE_VERTEX,
		false, true);
	NiTStridedRandomAccessIterator<NiPoint3> kVertexItr = kVertexLock.begin<NiPoint3>();

	float fDeltaRadian = NI_PI * 2.0f / (float)ms_uiCirclePointCount;
	float fCurRadian = 0.0f;
	for (uint i = 0; i <= ms_uiCirclePointCount; ++i)
	{
		kVertexItr[i].x = sin(-fCurRadian);
		kVertexItr[i].y = cos(-fCurRadian);
		kVertexItr[i].z = 0.0f;

		fCurRadian += fDeltaRadian;
	}

	kVertexLock.Unlock();

	// 子Mesh
	m_spOuterCircle->SetSubmeshCount(1);

	// 设置包围盒为无穷大
	NiBound kBound;
	kBound.SetCenter(NiPoint3::ZERO);
	kBound.SetRadius(FLT_MAX);
	m_spOuterCircle->SetWorldBound(kBound);

	// 设置顶点颜色
	NiVertexColorPropertyPtr spVertexProp = NiNew NiVertexColorProperty;
	NIASSERT(spVertexProp);
	spVertexProp->SetSourceMode(NiVertexColorProperty::SOURCE_IGNORE);
	spVertexProp->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	m_spOuterCircle->AttachProperty(spVertexProp);

	// 设置自发光
	NiMaterialPropertyPtr spMaterialProp = NiNew NiMaterialProperty;
	NIASSERT(spMaterialProp);
	spMaterialProp->SetEmittance(NiColor(0.0f, 1.0f, 0.0f));
	m_spOuterCircle->AttachProperty(spMaterialProp);

	// 设置Z
	NiZBufferPropertyPtr spZBuffProp = NiNew NiZBufferProperty;
	NIASSERT(spZBuffProp);
	spZBuffProp->SetZBufferTest(true);
	spZBuffProp->SetZBufferWrite(true);
	m_spOuterCircle->AttachProperty(spZBuffProp);

	// 更新
	m_spOuterCircle->UpdateProperties();
	m_spOuterCircle->UpdateEffects();
	m_spOuterCircle->Update(0.0f);

	//
	AttachChild(m_spOuterCircle);
}
//-------------------------------------------------------------------------------------------------
void sdDualCircleMesh::UpdateInnerCircle()
{
	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	if (!pkTerrain->HasScene())
		return;

	// 顶点数据流
	NiDataStreamElementLock kVertexLock(
		m_spInnerCircle, 
		NiCommonSemantics::POSITION(), 0,
		NiDataStreamElement::F_FLOAT32_3,
		NiDataStream::LOCK_WRITE);
	NiTStridedRandomAccessIterator<NiPoint3> kVertexItr = kVertexLock.begin<NiPoint3>();

	const NiPoint3& kCenterPos = GetTranslate();
	float fDeltaRadian = NI_PI * 2.0f / (float)ms_uiCirclePointCount;
	float fCurRadian = 0.0f;
	float fRadius = m_fInnerRadius;
	for (uint i = 0; i <= ms_uiCirclePointCount; ++i)
	{
		// 计算当前点相对坐标
		float fX = sin(-fCurRadian) * fRadius;
		float fY = cos(-fCurRadian) * fRadius;

		// 计算当前点绝对坐标
		float fAbsX = kCenterPos.x + fX;
		float fAbsY = kCenterPos.y + fY;
		float fAbsZ = pkTerrain->GetHeight(fAbsX, fAbsY);

		//
		kVertexItr[i].x = fX;
		kVertexItr[i].y = fY;
		kVertexItr[i].z = fAbsZ + 0.01f;

		//
		fCurRadian += fDeltaRadian;
	}

	kVertexLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
void sdDualCircleMesh::UpdateOutterCirlce()
{
	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	if (!pkTerrain->HasScene())
		return;

	// 顶点数据流
	NiDataStreamElementLock kVertexLock(
		m_spOuterCircle, 
		NiCommonSemantics::POSITION(), 0,
		NiDataStreamElement::F_FLOAT32_3,
		NiDataStream::LOCK_WRITE);
	NiTStridedRandomAccessIterator<NiPoint3> kVertexItr = kVertexLock.begin<NiPoint3>();

	const NiPoint3& kCenterPos = GetTranslate();
	float fDeltaRadian = NI_PI * 2.0f / (float)ms_uiCirclePointCount;
	float fCurRadian = 0.0f;
	float fRadius = m_fOuterRadius;
	for (uint i = 0; i <= ms_uiCirclePointCount; ++i)
	{
		// 计算当前点相对坐标
		float fX = sin(-fCurRadian) * fRadius;
		float fY = cos(-fCurRadian) * fRadius;

		// 计算当前点绝对坐标
		float fAbsX = kCenterPos.x + fX;
		float fAbsY = kCenterPos.y + fY;
		float fAbsZ = pkTerrain->GetHeight(fAbsX, fAbsY);

		//
		kVertexItr[i].x = fX;
		kVertexItr[i].y = fY;
		kVertexItr[i].z = fAbsZ + 0.01f;

		//
		fCurRadian += fDeltaRadian;
	}

	kVertexLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX