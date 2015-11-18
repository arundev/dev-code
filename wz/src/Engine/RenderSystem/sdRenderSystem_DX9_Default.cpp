#include "sdRenderSystemPCH.h"
#include "sdRenderSystem_DX9.h"

#include <NiMeshScreenElements.h>

using namespace Base::Math;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
NiMesh* sdRenderSystem_DX9::GetDefaultMesh(eDefaultMesh eMesh)
{
	if (eMesh < 0 || eMesh >= NUM_DEFAULT_MESHES)
	{
		NIASSERT(0);
		return NULL;
	}

	switch(eMesh)
	{
	case IRenderSystem::E_DM_SCREEN_QUAD:
		return m_spScreenQuadMesh;

	case IRenderSystem::E_DM_UNIT_CUBE:
		return m_spUnitCubeMesh;

	case IRenderSystem::E_DM_UNIT_CONE:
		return m_spUnitConeMesh;

	case IRenderSystem::E_DM_UNIT_PYRAMID:
		return m_spUnitPyramidMesh;

	case IRenderSystem::E_DM_UNIT_SPHERE:
		return m_spUnitSphereMesh;	
	}

	return m_spErrorMesh;
}
//-------------------------------------------------------------------------------------------------
NiMaterial*	sdRenderSystem_DX9::GetDefaultMaterial(eDefaultMaterial eMaterial)
{
	if (eMaterial < 0 || eMaterial >= NUM_DEFAULT_MATERIALS)
	{
		NIASSERT(0);
		return NULL;
	}

	return m_spDefaultMaterials[eMaterial];
}
//-------------------------------------------------------------------------------------------------
NiTexture* sdRenderSystem_DX9::GetDefaultTexture(eDefaultTexture eTexture)
{
	if (eTexture < 0 || eTexture >= NUM_DEFAULT_TEXTURES)
	{
		NIASSERT(0);
		return NULL;
	}	

	return m_spDefaultTextures[eTexture];	
}
//-------------------------------------------------------------------------------------------------
NiMesh*	sdRenderSystem_DX9::CreateScreenQuadMesh()
{
	NiMeshScreenElements* pkMesh = NiNew NiMeshScreenElements(false, false, 1);
	NIASSERT(pkMesh);

	pkMesh->Insert(4);
	pkMesh->SetRectangle(0, -1.0f, 1.0f, 2.0f, -2.0f);
	pkMesh->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
	pkMesh->Update(0.0f);
	pkMesh->UpdateEffects();
	pkMesh->UpdateProperties();

	return pkMesh;
}
//-------------------------------------------------------------------------------------------------
NiMesh*	sdRenderSystem_DX9::CreateUnitCubeMesh()
{
	NiPoint3 akVertice[8];
	ushort ausIndice[36];

	NiPoint3 kMin(-0.5f, -0.5f, -0.5f);
	NiPoint3 kMax(0.5f, 0.5f, 0.5f);

	//    0---1		
	//   / \ /|		
	//	3---2-5		z y
	//	| \ |/		|/
	//  7---6		O--x
	akVertice[0] = NiPoint3(kMin.x, kMax.y, kMax.z);
	akVertice[1] = NiPoint3(kMax.x, kMax.y, kMax.z);
	akVertice[2] = NiPoint3(kMax.x, kMin.y, kMax.z);
	akVertice[3] = NiPoint3(kMin.x, kMin.y, kMax.z);

	akVertice[4] = NiPoint3(kMin.x, kMax.y, kMin.z);
	akVertice[5] = NiPoint3(kMax.x, kMax.y, kMin.z);
	akVertice[6] = NiPoint3(kMax.x, kMin.y, kMin.z);
	akVertice[7] = NiPoint3(kMin.x, kMin.y, kMin.z);

	//
	uint uiIndex = 0;

	ausIndice[uiIndex++] = 0;		///< 上
	ausIndice[uiIndex++] = 3;
	ausIndice[uiIndex++] = 2;

	ausIndice[uiIndex++] = 0;	
	ausIndice[uiIndex++] = 2;
	ausIndice[uiIndex++] = 1;

	ausIndice[uiIndex++] = 7;		///< 下
	ausIndice[uiIndex++] = 4;
	ausIndice[uiIndex++] = 6;

	ausIndice[uiIndex++] = 6;	
	ausIndice[uiIndex++] = 4;
	ausIndice[uiIndex++] = 5;

	ausIndice[uiIndex++] = 0;		///< 左
	ausIndice[uiIndex++] = 4;
	ausIndice[uiIndex++] = 7;

	ausIndice[uiIndex++] = 0;	
	ausIndice[uiIndex++] = 7;
	ausIndice[uiIndex++] = 3;

	ausIndice[uiIndex++] = 2;		///< 右
	ausIndice[uiIndex++] = 6;
	ausIndice[uiIndex++] = 5;

	ausIndice[uiIndex++] = 2;	
	ausIndice[uiIndex++] = 5;
	ausIndice[uiIndex++] = 1;

	ausIndice[uiIndex++] = 3;		///< 前
	ausIndice[uiIndex++] = 7;
	ausIndice[uiIndex++] = 6;

	ausIndice[uiIndex++] = 3;	
	ausIndice[uiIndex++] = 6;
	ausIndice[uiIndex++] = 2;

	ausIndice[uiIndex++] = 1;		///< 后
	ausIndice[uiIndex++] = 5;
	ausIndice[uiIndex++] = 4;

	ausIndice[uiIndex++] = 1;	
	ausIndice[uiIndex++] = 4;
	ausIndice[uiIndex++] = 0;

	//
	NiMesh* pkMesh = NiNew NiMesh;
	pkMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	uint uiNumVertice = 8;
	NiDataStream* pkVerticesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiNumVertice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX,
		akVertice);
	pkMesh->AddStreamRef(pkVerticesStream, NiCommonSemantics::POSITION(), 0);

	uint uiNumIndice = 36;
	NiDataStream* pkIndicesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiNumIndice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX_INDEX,
		ausIndice);
	pkMesh->AddStreamRef(pkIndicesStream, NiCommonSemantics::INDEX(), 0);

	pkMesh->Update(0.f);
	pkMesh->UpdateEffects();
	pkMesh->UpdateProperties();

	return pkMesh;
}
//-------------------------------------------------------------------------------------------------
NiMesh* sdRenderSystem_DX9::CreateUnitSphereMesh()
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
NiMesh* sdRenderSystem_DX9::CreateUnitConeMesh(uint uiNumSegment)
{
	uint uiNumVertice = uiNumSegment + 2;
	uint uiNumIndice = uiNumSegment * 3 * 2;

	typedef std::vector<sdVector3> VectorVec;
	typedef std::vector<sdVector3>::iterator VectorVecItr;
	VectorVec kVerticeVec(uiNumVertice);

	typedef std::vector<ushort> UShortVec;
	typedef std::vector<ushort>::iterator UShortVecItr;
	UShortVec kIndiceVec(uiNumIndice);

	float fSegmentRadian = sdMath::PI * 2.f / uiNumSegment;
	float fRadius = 1.f / cosf(fSegmentRadian * 0.5f);

	//
	kVerticeVec[0] = sdVector3::ZERO;
	for (uint ui = 0; ui < uiNumSegment; ++ui)
	{
		kVerticeVec[ui+1].m_fX = 1.f;
		kVerticeVec[ui+1].m_fY = sinf(fSegmentRadian * ui) * fRadius;
		kVerticeVec[ui+1].m_fZ = cosf(fSegmentRadian * ui) * fRadius;
	}
	kVerticeVec[uiNumSegment+1] = sdVector3(1.f, 0.f, 0.f);

	//
	for (uint ui = 0; ui < uiNumSegment; ++ui)
	{
		kIndiceVec[ui * 3]		= 0;
		kIndiceVec[ui * 3 + 1] = ui + 1;
		kIndiceVec[ui * 3 + 2] = ui + 2;

		kIndiceVec[(uiNumSegment + ui) * 3]	 = uiNumSegment + 1;
		kIndiceVec[(uiNumSegment + ui) * 3 + 1] = ui + 2;
		kIndiceVec[(uiNumSegment + ui) * 3 + 2] = ui + 1;
	}

	kIndiceVec[uiNumSegment * 3 - 1] = 1;		///< fix exception
	kIndiceVec[uiNumSegment * 3 * 2 - 2] = 1;	///<

	//
	NiMesh* pkMesh = NiNew NiMesh;
	pkMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	NiDataStream* pkVerticesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiNumVertice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX,
		&(kVerticeVec[0]));
	pkMesh->AddStreamRef(pkVerticesStream, NiCommonSemantics::POSITION(), 0);

	NiDataStream* pkIndicesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiNumIndice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX_INDEX,
		&(kIndiceVec[0]));
	pkMesh->AddStreamRef(pkIndicesStream, NiCommonSemantics::INDEX(), 0);

	pkMesh->Update(0.f);
	pkMesh->UpdateEffects();
	pkMesh->UpdateProperties();

	return pkMesh;
}
//-------------------------------------------------------------------------------------------------
NiMesh* sdRenderSystem_DX9::CreatePyramidMesh()
{
	uint uiNumVertice = 5;
	uint uiNumIndice = 6 * 3;

	typedef std::vector<sdVector3> VectorVec;
	typedef std::vector<sdVector3>::iterator VectorVecItr;
	VectorVec kVerticeVec(uiNumVertice);

	typedef std::vector<ushort> UShortVec;
	typedef std::vector<ushort>::iterator UShortVecItr;
	UShortVec kIndiceVec(uiNumIndice);

	//           1
	//	        /|
	//	       / |
	// 0------4  2
	//	      | /
	//	      |/
	//	      3      
	kVerticeVec[0] = sdVector3(0.f,  0.f,  0.f);
	kVerticeVec[1] = sdVector3(1.f,  1.f,  1.f);
	kVerticeVec[2] = sdVector3(1.f,  1.f, -1.f);
	kVerticeVec[3] = sdVector3(1.f, -1.f, -1.f);
	kVerticeVec[4] = sdVector3(1.f, -1.f,  1.f);

	//
	uint uiIndex = 0;

	kIndiceVec[uiIndex++] = 0;	///< 侧面
	kIndiceVec[uiIndex++] = 1;
	kIndiceVec[uiIndex++] = 2;

	kIndiceVec[uiIndex++] = 0;
	kIndiceVec[uiIndex++] = 2;
	kIndiceVec[uiIndex++] = 3;

	kIndiceVec[uiIndex++] = 0;
	kIndiceVec[uiIndex++] = 3;
	kIndiceVec[uiIndex++] = 4;

	kIndiceVec[uiIndex++] = 0;
	kIndiceVec[uiIndex++] = 4;
	kIndiceVec[uiIndex++] = 1;

	kIndiceVec[uiIndex++] = 1;	///< 底面
	kIndiceVec[uiIndex++] = 3;
	kIndiceVec[uiIndex++] = 2;

	kIndiceVec[uiIndex++] = 1;
	kIndiceVec[uiIndex++] = 4;
	kIndiceVec[uiIndex++] = 3;

	//
	NiMesh* pkMesh = NiNew NiMesh;
	pkMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	NiDataStream* pkVerticesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiNumVertice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX,
		&(kVerticeVec[0]));
	pkMesh->AddStreamRef(pkVerticesStream, NiCommonSemantics::POSITION(), 0);

	NiDataStream* pkIndicesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiNumIndice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX_INDEX,
		&(kIndiceVec[0]));
	pkMesh->AddStreamRef(pkIndicesStream, NiCommonSemantics::INDEX(), 0);

	pkMesh->Update(0.f);
	pkMesh->UpdateEffects();
	pkMesh->UpdateProperties();

	return pkMesh;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE