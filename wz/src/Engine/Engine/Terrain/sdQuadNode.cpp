#include "sdEnginePCH.h"
#include "sdQuadNode.h"
#include "sdTerrain.h"

using namespace Base::Math;
//-------------------------------------------------------------------------------------------------
sdQuadNode::sdQuadNode(sdTerrain* pkTerrain)
: m_pkTerrain(pkTerrain)
, m_pkQuadNodePar(NULL)
, m_uiLevel(0)
, m_uiSpacing(1)
, m_fError(0.0f)
, m_fAccumError(0.0f)
, m_uiOriginX(0)
, m_uiOriginY(0)
{

}
//-------------------------------------------------------------------------------------------------
sdQuadNode::~sdQuadNode()
{
	m_pkQuadMesh = 0;

	m_pkQuadNodeChild[E_LT_CHILD] = 0;
	m_pkQuadNodeChild[E_LB_CHILD] = 0;
	m_pkQuadNodeChild[E_RT_CHILD] = 0;
	m_pkQuadNodeChild[E_RB_CHILD] = 0;
}
//-------------------------------------------------------------------------------------------------
bool sdQuadNode::Build(sdVector4ui& kRect, sdQuadNode* pkParent)
{
	// ���ڵ�
	m_pkQuadNodePar = pkParent;

	// ��ʼ��
	m_uiOriginX	= kRect.m_kX;
	m_uiOriginY	= kRect.m_kY;

	// �ݹ鴦���ӽڵ�(ע�����ȵݹ�,�ٴ���ڵ�)
	// @{
	uint uiTileSize = m_pkTerrain->GetTileSize();
	if (kRect.m_kZ - kRect.m_kX != uiTileSize)
	{
		// ��ǰ�ڵ�Ϊ�м�ڵ�
		//
		// �ݹ��������
		sdVector4ui kChildRect;
		sdVector2ui kCenter((kRect.m_kX + kRect.m_kZ) / 2, (kRect.m_kY + kRect.m_kW) / 2);

		kChildRect.m_kX = kRect.m_kX;
		kChildRect.m_kY = kCenter.m_kY;
		kChildRect.m_kZ = kCenter.m_kX;
		kChildRect.m_kW = kRect.m_kW;
		m_pkQuadNodeChild[E_LT_CHILD] = NiNew sdQuadNode(m_pkTerrain);
		m_pkQuadNodeChild[E_LT_CHILD]->Build(kChildRect, this);

		kChildRect.m_kX = kCenter.m_kX;
		kChildRect.m_kY = kCenter.m_kY;
		kChildRect.m_kZ = kRect.m_kZ;
		kChildRect.m_kW = kRect.m_kW;
		m_pkQuadNodeChild[E_RT_CHILD] = NiNew sdQuadNode(m_pkTerrain);
		m_pkQuadNodeChild[E_RT_CHILD]->Build(kChildRect, this);

		kChildRect.m_kX = kRect.m_kX;
		kChildRect.m_kY = kRect.m_kY;
		kChildRect.m_kZ = kCenter.m_kX;
		kChildRect.m_kW = kCenter.m_kY;
		m_pkQuadNodeChild[E_LB_CHILD] = NiNew sdQuadNode(m_pkTerrain);
		m_pkQuadNodeChild[E_LB_CHILD]->Build(kChildRect, this);

		kChildRect.m_kX = kCenter.m_kX;
		kChildRect.m_kY = kRect.m_kY;
		kChildRect.m_kZ = kRect.m_kZ;
		kChildRect.m_kW = kCenter.m_kY;
		m_pkQuadNodeChild[E_RB_CHILD] = NiNew sdQuadNode(m_pkTerrain);
		m_pkQuadNodeChild[E_RB_CHILD]->Build(kChildRect, this);

		// �ڵ�㼶(�������ϵ���+1)
		m_uiLevel = m_pkQuadNodeChild[E_LT_CHILD]->GetLevel() + 1;

		// �ڵ�������(�������ϵ���*2)
		m_uiSpacing = m_pkQuadNodeChild[E_LT_CHILD]->GetSpacing() * 2;

		// �ڵ�������ۻ����(���������ۻ�,�ӽڵ������ۻ����+���ڵ�����ӽڵ����)
		m_fError = CalcError();

		m_fAccumError = 0.0f;
		m_fAccumError = max(m_pkQuadNodeChild[E_LT_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_RT_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_LB_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_RB_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError += m_fError;

		// �ڵ��Χ��(�������Ϻϲ�)
		CalcBound(m_kAABB);
		m_kAABB.Merge(m_pkQuadNodeChild[E_LT_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_RT_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_LB_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_RB_CHILD]->GetBound());
	}
	else
	{
		// ��ǰ�ڵ�ΪҶ�ӽڵ�
		//
		// �ڵ�㼶
		m_uiLevel = 0;	

		// �ڵ�������
		m_uiSpacing = 1;

		// �ڵ����
		m_fError = 0.0f;
		m_fAccumError = 0.0f;

		// ����ڵ��Χ��
		CalcBound(m_kAABB);
	}
	// @}

	// ������ǰNode��Mesh
	uint uiMeshLevel = m_pkTerrain->GetMeshLevel();
	if (m_uiLevel < uiMeshLevel)
	{
		m_pkQuadMesh = NiNew sdQuadMesh(m_uiOriginX, m_uiOriginY, m_uiSpacing, uiTileSize, m_fAccumError, m_pkTerrain);
		NIASSERT(m_pkQuadMesh);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdQuadNode::Destroy()
{
	m_pkQuadMesh = 0;
	if (!IsLeaf())
	{
		m_pkQuadNodeChild[E_LT_CHILD]->Destroy();
		m_pkQuadNodeChild[E_LT_CHILD] = 0;

		m_pkQuadNodeChild[E_LB_CHILD]->Destroy();
		m_pkQuadNodeChild[E_LB_CHILD] = 0;

		m_pkQuadNodeChild[E_RT_CHILD]->Destroy();
		m_pkQuadNodeChild[E_RT_CHILD] = 0;

		m_pkQuadNodeChild[E_RB_CHILD]->Destroy();
		m_pkQuadNodeChild[E_RB_CHILD] = 0;
	}
}
//-------------------------------------------------------------------------------------------------
void sdQuadNode::UpdateGeometry(float fCenterX, float fCenterY, float fRadius)
{
	// ����Բ����AABB��
	//	1.AABB������Circle���ľ���������ά�ȷ��򶼲�����2�߰뾶֮��
	//	 (�˴�ΪCircleλ��AABB�������������ߴ��ж�)
	//	2.AABB�Ľ���Circle�������߾���ƽ��������Circle�뾶ƽ��
	//   (�˴�ΪCircleλ��AABB�Ľ�)
	// @{
	bool bIntersect = false;
	const sdVector3& kMinimum = m_kAABB.GetMinimum();
	const sdVector3& kMaximum = m_kAABB.GetMaximum();
	float fAABBCenterX = (kMaximum.m_fX + kMinimum.m_fX) / 2.0f;	///< AABB����
	float fAABBCenterY = (kMaximum.m_fY + kMinimum.m_fY) / 2.0f;
	float fAABBHalfSizeX = (kMaximum.m_fX - kMinimum.m_fX) / 2.0f;	///< AABB�볤
	float fAABBHalfSizeY = (kMaximum.m_fY - kMinimum.m_fY) / 2.0f;
	float fMaxDistanceX = fAABBHalfSizeX + fRadius;		///< AABB��Circle֮���ܹ��ཻʱ,���ĵ�������
	float fMaxDistanceY = fAABBHalfSizeY + fRadius;
	float fDistanceX = fabs(fCenterX - fAABBCenterX);	///< AABB��Circle�����ľ���
	float fDistanceY = fabs(fCenterY - fAABBCenterY);
	if (fDistanceX <= fMaxDistanceX && fDistanceY <= fMaxDistanceY)
	{
		float fDeltaX = max(fDistanceX - fAABBHalfSizeX, 0.0f);
		float fDeltaY = max(fDistanceY - fAABBHalfSizeY, 0.0f);
		if (fDeltaX * fDeltaX + fDeltaY * fDeltaY <= fRadius * fRadius)
			bIntersect = true;
	}
	// @}

	// �������
	//	1.ע�����ȵݹ�,�ٴ���ڵ�,��֤�ӽڵ㱻�ȴ���
	//	1.��ˢһ�㲻��,����Ҫ����ȫ����/���ָ���/���ཻ
	// @{
	if (bIntersect)
	{	
		if (m_uiLevel != 0)
		{
			// �м�ڵ�
			//
			// ���±���
			m_pkQuadNodeChild[E_LT_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_RT_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_LB_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_RB_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			
			// ���µ�ǰ�ڵ�������ۻ����
			m_fError = CalcError();

			m_fAccumError = 0.0f;
			m_fAccumError = max(m_pkQuadNodeChild[E_LT_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_RT_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_LB_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_RB_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError += m_fError;

			// ���µ�ǰ�ڵ�GeometryMesh
			float fMinHeight = FLT_MAX;
			float fMaxHeight = -FLT_MAX;
			if (m_pkQuadMesh)
			{
				m_pkQuadMesh->UpdateGeometry(fMinHeight, fMaxHeight, m_fAccumError);
			}

			// ���µ�ǰ�ڵ��Χ��(�������Ϻϲ�)
			fMinHeight = min(fMinHeight, m_pkQuadNodeChild[E_LT_CHILD]->GetBound().GetMinimum().m_fZ);
			fMinHeight = min(fMinHeight, m_pkQuadNodeChild[E_RT_CHILD]->GetBound().GetMinimum().m_fZ);
			fMinHeight = min(fMinHeight, m_pkQuadNodeChild[E_LB_CHILD]->GetBound().GetMinimum().m_fZ);
			fMinHeight = min(fMinHeight, m_pkQuadNodeChild[E_RB_CHILD]->GetBound().GetMinimum().m_fZ);

			fMaxHeight = max(fMaxHeight, m_pkQuadNodeChild[E_LT_CHILD]->GetBound().GetMaximum().m_fZ);
			fMaxHeight = max(fMaxHeight, m_pkQuadNodeChild[E_RT_CHILD]->GetBound().GetMaximum().m_fZ);
			fMaxHeight = max(fMaxHeight, m_pkQuadNodeChild[E_LB_CHILD]->GetBound().GetMaximum().m_fZ);
			fMaxHeight = max(fMaxHeight, m_pkQuadNodeChild[E_RB_CHILD]->GetBound().GetMaximum().m_fZ);

			m_kAABB.SetMinimumZ(fMinHeight);
			m_kAABB.SetMaximumZ(fMaxHeight);
		}
		else
		{
			// Ҷ�ӽڵ�
			//
			// ���µ�ǰ�ڵ�GeometryMesh
			float fMinHeight = FLT_MAX;
			float fMaxHeight = -FLT_MAX;
			m_pkQuadMesh->UpdateGeometry(fMinHeight, fMaxHeight, m_fAccumError);

			// ���ð�Χ��
			m_kAABB.SetMinimumZ(fMinHeight);
			m_kAABB.SetMaximumZ(fMaxHeight);
		}
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdQuadNode::Cull(const NiCamera& kCamera, NiFrustumPlanes& kFrustumPlanes, std::vector<NiMesh*>& kMeshVec)
{
	// �Ե�ǰ�ڵ���вü�,���
	//	1.�Ƿ�ɼ�
	//	2.�Ƿ�������±���
	//		(1)���²ü�
	//		(2)�����²ü�		
	//
	// ��Χ��
	sdVector3 kCenter, kHalfSize;
	m_kAABB.GetCenter(kCenter);
	m_kAABB.GetSize(kHalfSize);

	NiPoint3 kTCenter(kCenter.m_fX, kCenter.m_fY, kCenter.m_fZ);

	// 1.����ü�(�ο���Ogre)
	//
	// �������������������вü�,ֻ�о����ڸ�����ı�����п��ܿɼ�
	// \TODO ������һЩ�����ܵ�AABB��Frustum���㷨
	//
	// @{
	bool bVisible = false;
	uint uiPlaneCount = 0;
	for (; uiPlaneCount < NiFrustumPlanes::MAX_PLANES; uiPlaneCount++)
	{
		if (kFrustumPlanes.IsPlaneActive(uiPlaneCount))
		{
			const NiPlane& kPlane = kFrustumPlanes.GetPlane(uiPlaneCount);
			
			// Calculate the distance between box centre and the plane
			// ƽ�浽��Χ�����ľ���
			float fDistance = kPlane.Distance(kTCenter);

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			// ��Χ����ƽ�淨�߷���ͶӰ�İ볤
			const NiPoint3& kNormal = kPlane.GetNormal();
			float fMaxAbsDistance = fabsf(kNormal.x * kHalfSize.m_fX) + 
				fabsf(kNormal.y * kHalfSize.m_fY) +	fabsf(kNormal.z * kHalfSize.m_fZ);

			// ֻҪ��Χ��λ����һƽ�渺��,ֱ���޳�
			if (fDistance < -fMaxAbsDistance)
			{
				// NiPlane::NEGATIVE_SIDE;
				break;
			}
			//else if (fDistance > fMaxAbsDistance)
			//{
			//	// NiPlane::POSITIVE_SIDE
			//}
			//else
			//{
			//	//NiPlane::NO_SIDE;
			//}
		}
	}

	if (uiPlaneCount == NiFrustumPlanes::MAX_PLANES)
		bVisible = true;
	// @}


	// 2.�����Ƿ���Ҫ��һ�����±���
	// @{
	bool bShouldSubdivide = true;
//	if (bVisible)
//	{
//		if (!pkTerrain->GetEnableLOD() || m_uiLevel >= pkTerrain->GetMeshLevel())
//		{
//			// ���²ü���ԭ��:
//			//	1.���ν���LOD
//			//	2.��ǰ�㼶û������Mesh
//			bShouldSubdivide = true;
//		}
//		else
//		{
//			//
//			// �����ж�:
//			//	1.����ڵ��Χ�е��ӵ���������
//			//	2.���ݽڵ����һ���������������۲����
//			//	3.�Ƚ���������ȷ���Ƿ�ϸ��
//			//
//			// \TODO �д��ڽ�һ�������л�
//			//
//			const NiPoint3& kCamPos = kCamera.GetWorldTranslate();
//			float fSqlDistance = m_kAABB.Distance(sdVector3(kCamPos.x, kCamPos.y, kCamPos.z));
//
//			//NiPoint3 kDistance;
//			//const NiPoint3& kCamPos = kCamera.GetWorldTranslate();
//			//const sdVector3& kAABBMinPos = m_kAABB.GetMinimum();
//			//const sdVector3& kAABBMaxPos = m_kAABB.GetMaximum();
//			//kDistance.x = min(min(kCamPos.x - kAABBMinPos.m_fX, kAABBMaxPos.m_fX - kCamPos.x), 0);
//			//kDistance.y = min(min(kCamPos.y - kAABBMinPos.m_fY, kAABBMaxPos.m_fY - kCamPos.y), 0);
//			//kDistance.z = min(min(kCamPos.z - kAABBMinPos.m_fZ, kAABBMaxPos.m_fZ - kCamPos.z), 0);
//			//float fSqlDistance = kDistance.SqrLength();
//
//			// 2
//			float fMinDistance = m_fError * pkTerrain->GetErrorToDistance();
//
//			// 3
//			if (fSqlDistance < fMinDistance * fMinDistance)
//				bShouldSubdivide = true;
//			else
//				bShouldSubdivide = false;
//		}
//	}
	// @}


	// ����
	// @{
	if (bVisible)
	{
		if (bShouldSubdivide)
		{
			// ���±���
			if (IsLeaf())
			{
				// Ҷ�ӽڵ�ֱ����ȡMesh
				if (m_pkQuadMesh)
					kMeshVec.push_back(m_pkQuadMesh);
			}
			else
			{
				// �м�ڵ�,���մӽ���Զ�Ĵ������δ����ӽڵ�
				//
				// �����ӽڵ����ĵ��۲�����
				float vfDistance[NUM_CHILDREN] =  { 0 };
				sdVector3 kCamPosition(kCamera.GetWorldTranslate().x, kCamera.GetWorldTranslate().y, kCamera.GetWorldTranslate().z);
				for (int i = 0; i < NUM_CHILDREN; ++i)
				{
					vfDistance[i] = (m_pkQuadNodeChild[i]->GetBound().GetCenter() - kCamPosition).SquaredLength();
				}

				// ÿ��ȡ��������������Сֵ��Node����, �����������Ϊ�����
				for (int i = 0; i < NUM_CHILDREN; ++i)
				{
					float fMinDistance = vfDistance[0];
					int iMinChild = 0;

					for (int j = 1; j < NUM_CHILDREN; ++j)
					{
						if (vfDistance[j] < fMinDistance)
						{
							fMinDistance = vfDistance[j];
							iMinChild = j;
						}
					}
					vfDistance[iMinChild] = sdMath::POS_INFINITY;

					m_pkQuadNodeChild[iMinChild]->Cull(kCamera, kFrustumPlanes, kMeshVec);
				}	
			}
		}
		else
		{
			// ֹͣ������ȡMesh
			if (m_pkQuadMesh)
				kMeshVec.push_back(m_pkQuadMesh);
		}
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
float sdQuadNode::CalcError()
{
	sdHeightMap* pkHeightMap = m_pkTerrain->GetHeightMap();
	NIASSERT(pkHeightMap);

	// Ҷ�ӽڵ������
	if (m_uiLevel == 0)
		return 0.0f;

	// ��ȡ���о�ϸ��������㵱���ڵ�ǰ���������������ֵ
	float fError = 0.0f;
	uint uiSize = m_pkTerrain->GetTileSize();
	uint uiStartX = m_uiOriginX;
	uint uiStartY = m_uiOriginY;
	uint uiSpacing = m_uiSpacing / 2;		///< m_uiSpacing�ض�����1
	for (uint uiY = 0; uiY <= uiSize; ++uiY)
	{
		for (uint uiX = 0; uiX <= uiSize; ++uiX)
		{
			float fCalcError = pkHeightMap->GetMorphDelta((int)uiStartX, (int)uiStartY, (int)uiX, (int)uiY, uiSpacing);
			fError = max(fError, fabsf(fCalcError));
		}
	}

	return fError;
}
//-------------------------------------------------------------------------------------------------
void sdQuadNode::CalcBound(sdAxisAlignedBox& kAABB)
{
	sdHeightMap* pkHeightMap = m_pkTerrain->GetHeightMap();
	NIASSERT(pkHeightMap);

	// ����߶ȷ�Χ
	float fHeight = pkHeightMap->GetHeight((int)m_uiOriginY, (int)m_uiOriginY);
	float fMinHeight = FLT_MAX;
	float fMaxHeight = -FLT_MAX;
	uint uiSize = m_pkTerrain->GetTileSize();
	uint uiSpacing = m_uiSpacing;
	for (uint uiY = 0, uiStartY = m_uiOriginY; uiY <= uiSize; ++uiY, uiStartY += uiSpacing)
	{
		for (uint uiX = 0, uiStartX = m_uiOriginX; uiX <= uiSize; ++uiX, uiStartX += uiSpacing)
		{
			fHeight = pkHeightMap->GetHeight((int)uiStartX, (int)uiStartY);
			if (fHeight < fMinHeight)	
				fMinHeight = fHeight;
			else if (fHeight > fMaxHeight)	
				fMaxHeight = fHeight;
		}
	}

	// ���ð�Χ�з�Χ
	kAABB.SetMinimum((float)m_uiOriginX, (float)m_uiOriginY, fMinHeight);
	kAABB.SetMaximum((float)(m_uiOriginX + uiSize * uiSpacing), (float)(m_uiOriginY + uiSize * uiSpacing), fMaxHeight);
}
//-------------------------------------------------------------------------------------------------