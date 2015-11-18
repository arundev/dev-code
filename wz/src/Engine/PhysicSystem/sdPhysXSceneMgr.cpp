#include "sdPhysicsSystemPCH.h"
#include "sdPhysXSceneMgr.h"
#include "sdPhysXCookUtility.h"
#include "sdPhysXStream.h"

#include <NiCollisionData.h>
#include <NiDataStreamPrimitiveLock.h>

#include <NxTriangleMeshDesc.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM

//-------------------------------------------------------------------------------------------------
sdPhysXSceneMgr::sdPhysXSceneMgr()
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXSceneMgr::~sdPhysXSceneMgr()
{

}
//-------------------------------------------------------------------------------------------------
uint sdPhysXSceneMgr::CookMesh(sdPhysXMemoryWriteStream& kStream, NiMesh* spMesh, float fScale, bool bFlipNormal)
{
	NIASSERT(spMesh);

	// 检查输入NiMesh图元
	NiPrimitiveType::Type ePriType = spMesh->GetPrimitiveType();
	if (ePriType != NiPrimitiveType::PRIMITIVE_TRIANGLES && ePriType != NiPrimitiveType::PRIMITIVE_TRIANGLES)
	{
		NIASSERT(0);
		return 0;
	}

	// 检查输入NiMesh三角形数量
	uint uiTriangleNum = spMesh->GetTotalPrimitiveCount();
	if (!uiTriangleNum)
	{
		NIASSERT(0);
		return 0;
	}

	// Only a triangle submesh and a single vertex region may exit for the collision system.
	NIASSERT(NiCollisionData::ValidateForCollision(spMesh, NiCollisionData::USE_OBB));

	// Iterate over all the triangles...
	NiDataStreamPrimitiveLock kTriLock(spMesh, NiDataStream::LOCK_TOOL_READ);
	
	uint uiUsedTriangles = 0;
	NIASSERT(kTriLock.IndexBufferExists());
	if (kTriLock.Has32BitIndexBuffer())
	{
		m_bUse16BitIndex = false;

		NiIndexedPrimitiveIterator32 kBegin = kTriLock.BeginIndexed32();
		NiIndexedPrimitiveIterator32 kEnd = kTriLock.EndIndexed32();
		NiIndexedPrimitiveIterator32 kIter = kBegin;
		NiUInt32* pkIndexData = (NiUInt32*)m_pkIndexCache;
		for (; kIter != kEnd; ++kIter)
		{
			NIASSERT(kIter->count() == 3);	///< Must be Tri-based

			NiUInt32 uiIndex0 = (*kIter)[0];
			NiUInt32 uiIndex1 = (*kIter)[1];
			NiUInt32 uiIndex2 = (*kIter)[2];

			// Check for degenerates, 检查零面积三角形
			if (uiIndex0 == uiIndex1 || uiIndex1 == uiIndex2 || uiIndex0 == uiIndex2)
				continue;

			// Possible lost of data as OBB only supports 16-bits
			*pkIndexData++ = (ushort)uiIndex0;
			*pkIndexData++ = (ushort)uiIndex1;
			*pkIndexData++ = (ushort)uiIndex2;

			++uiUsedTriangles;
		}
	}
	else if (kTriLock.Has16BitIndexBuffer())
	{
		m_bUse16BitIndex = true;			///< 当前使用16bit索引

		NiIndexedPrimitiveIterator16 kBegin = kTriLock.BeginIndexed16();
		NiIndexedPrimitiveIterator16 kEnd = kTriLock.EndIndexed16();
		NiIndexedPrimitiveIterator16 kIter = kBegin;
		NiUInt16* pkIndexData = (NiUInt16*)m_pkIndexCache;
		for (; kIter != kEnd; ++kIter)
		{
			NIASSERT(kIter->count() == 3);	///< Must be Tri-based

			NiUInt32 uiIndex0 = (*kIter)[0];
			NiUInt32 uiIndex1 = (*kIter)[1];
			NiUInt32 uiIndex2 = (*kIter)[2];

			// Check for degenerates, 检查零面积三角形
			if (uiIndex0 == uiIndex1 || uiIndex1 == uiIndex2 || uiIndex0 == uiIndex2)
				continue;

			// Possible lost of data as OBB only supports 16-bits
			*pkIndexData++ = uiIndex0;
			*pkIndexData++ = uiIndex1;
			*pkIndexData++ = uiIndex2;

			++uiUsedTriangles;
		}
	}


	// 读取顶点数据
	NiDataStreamElementLock kLockPos(spMesh, 
		NiCommonSemantics::POSITION(), 
		0, 
		NiDataStreamElement::F_FLOAT32_3, 
		NiDataStream::LOCK_TOOL_READ);

	// Note that each mesh-based object should consist of exactly one sub-mesh
	// and must have exactly one vertex region
	NiTStridedRandomAccessIterator<NiPoint3> kPosBegin = kLockPos.begin<NiPoint3>();
	NiTStridedRandomAccessIterator<NiPoint3> kPosEnd = kLockPos.begin<NiPoint3>();
	NiTStridedRandomAccessIterator<NiPoint3> kPosIter = kPosBegin;
	
	uint uiUsedVertices = 0;
	NiPoint3* pkVertexData = m_pkVertexCache;
	const NiTransform& kWorldTransform = spMesh->GetWorldTransform();
	for (; kPosIter != kPosEnd; ++kPosIter)
	{
		*pkVertexData++ = kWorldTransform * (*kPosIter) * fScale;	///< 变换到世界空间
		++uiUsedVertices;
	}


	// 调用PhysX来Cook三角形
	NxTriangleMeshDesc kTriMeshDesc;
	kTriMeshDesc.numVertices = uiUsedVertices;
	kTriMeshDesc.numTriangles = uiUsedTriangles;
	kTriMeshDesc.pointStrideBytes = sizeof(NxVec3);
	kTriMeshDesc.points = m_pkVertexCache;
	kTriMeshDesc.triangles = m_pkIndexCache;
	kTriMeshDesc.flags = m_bUse16BitIndex ? NX_MF_16_BIT_INDICES : 0;
	kTriMeshDesc.triangleStrideBytes = m_bUse16BitIndex ? 3 * sizeof(NxU16) : 3 * sizeof(NxU32);
	if (bFlipNormal) kTriMeshDesc.flags |= NX_MF_FLIPNORMALS;

	NxU32 kCurPos = kStream.GetCurrentPosition();
	sdPhysXCookUtility::Instance().CookTriangleMesh(kTriMeshDesc, kStream);
	NxU32 kSizeByBytes = kStream.GetCurrentPosition() - kCurPos;

	return kSizeByBytes;
}
//-------------------------------------------------------------------------------------------------
uint sdPhysXSceneMgr::CookObject(sdPhysXMemoryWriteStream& kStream, NiAVObject* spAVObject, float fScale, bool bFlipNormal)
{
	uint uiSizeByBytes = 0;
	if (NiIsKindOf(NiMesh, spAVObject))
	{
		NiMesh* spMesh = (NiMesh*)spAVObject;
		uiSizeByBytes += CookMesh(kStream, spMesh, fScale, bFlipNormal);
	}
	else if (spAVObject->IsNode())
	{
		NiNode* spNode = (NiNode*)spAVObject;
		for (uint ui = 0; ui < spNode->GetArrayCount(); ++ui)
		{
			NiAVObject* spChildAVObject = spNode->GetAt(ui);
			if (spChildAVObject)
				uiSizeByBytes += CookObject(kStream, spChildAVObject, fScale, bFlipNormal);
		}
	}
	return uiSizeByBytes;
}
//-------------------------------------------------------------------------------------------------
sdPhysXQuadSection::sdPhysXQuadSection(): m_uiOriginX(0), m_uiOriginY(0), m_uiSize(0), m_usSectionID(INVALID_SECTION_ID)
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXQuadSection::~sdPhysXQuadSection()
{
	m_pkQuadNodeChild[E_LT_CHILD] = 0;
	m_pkQuadNodeChild[E_RT_CHILD] = 0;
	m_pkQuadNodeChild[E_LB_CHILD] = 0;
	m_pkQuadNodeChild[E_RB_CHILD] = 0;
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXQuadSection::Build(sdPhysXSceneMgr* pkPhysXSceneMgr, uint uiOriginX, uint uiOriginY, uint uiSize)
{
	NIASSERT(m_pkPhysXSceneMgr);

	//
	m_pkPhysXSceneMgr = pkPhysXSceneMgr;

	// 起始点
	m_uiOriginX	= uiOriginX;
	m_uiOriginY	= uiOriginY;
	
	//
	uint uiSectionSize = pkPhysXSceneMgr->GetSectionSize();
	uint uiSectionPerSide = uiSize / uiSectionSize;
	NIASSERT((uiSectionPerSide & 0x3) == 0);
	if (uiSize != uiSectionSize)
	{
		// 当前节点为中间节点
		//
		// 递归调用子树
		uint uiChildSize = uiSize / 2;
		uint uiCenterX = uiOriginX + uiChildSize;
		uint uiCenterY = uiOriginY + uiChildSize;

		m_pkQuadNodeChild[E_LT_CHILD] = NiNew sdPhysXQuadSection;
		m_pkQuadNodeChild[E_LT_CHILD]->Build(pkPhysXSceneMgr, uiOriginX, uiCenterY, uiChildSize);

		m_pkQuadNodeChild[E_RT_CHILD] = NiNew sdPhysXQuadSection;
		m_pkQuadNodeChild[E_RT_CHILD]->Build(pkPhysXSceneMgr, uiCenterX, uiCenterY, uiChildSize);

		m_pkQuadNodeChild[E_LB_CHILD] = NiNew sdPhysXQuadSection;
		m_pkQuadNodeChild[E_LB_CHILD]->Build(pkPhysXSceneMgr, uiOriginX, uiOriginY, uiChildSize);

		m_pkQuadNodeChild[E_RB_CHILD] = NiNew sdPhysXQuadSection;
		m_pkQuadNodeChild[E_RB_CHILD]->Build(pkPhysXSceneMgr, uiCenterX, uiOriginX, uiChildSize);

		// 节点ID
		m_usSectionID = INVALID_SECTION_ID;
	}
	else
	{
		// 当前节点为叶子节点
		//
		// 节点ID
		uint uiIndexX = uiOriginX / uiSectionSize;
		uint uiIndexY = uiOriginY / uiSectionSize;
		m_usSectionID = uiIndexY * uiSectionPerSide + uiIndexX;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdPhysXQuadSection::Cull(const NiBound& kBound, std::vector<ushort>& kSectionIDVec)
{
	float fCenterX = kBound.GetCenter().x;
	float fCenterY = kBound.GetCenter().y;
	float fRadius = kBound.GetRadius();





}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM