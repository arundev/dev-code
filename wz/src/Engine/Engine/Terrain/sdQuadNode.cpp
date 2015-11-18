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
	// 父节点
	m_pkQuadNodePar = pkParent;

	// 起始点
	m_uiOriginX	= kRect.m_kX;
	m_uiOriginY	= kRect.m_kY;

	// 递归处理子节点(注意是先递归,再处理节点)
	// @{
	uint uiTileSize = m_pkTerrain->GetTileSize();
	if (kRect.m_kZ - kRect.m_kX != uiTileSize)
	{
		// 当前节点为中间节点
		//
		// 递归调用子树
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

		// 节点层级(从下往上递增+1)
		m_uiLevel = m_pkQuadNodeChild[E_LT_CHILD]->GetLevel() + 1;

		// 节点采样间距(从下往上递增*2)
		m_uiSpacing = m_pkQuadNodeChild[E_LT_CHILD]->GetSpacing() * 2;

		// 节点误差与累积误差(从下往上累积,子节点的最大累积误差+本节点相对子节点误差)
		m_fError = CalcError();

		m_fAccumError = 0.0f;
		m_fAccumError = max(m_pkQuadNodeChild[E_LT_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_RT_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_LB_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError = max(m_pkQuadNodeChild[E_RB_CHILD]->GetAccumError(), m_fAccumError);
		m_fAccumError += m_fError;

		// 节点包围盒(从下往上合并)
		CalcBound(m_kAABB);
		m_kAABB.Merge(m_pkQuadNodeChild[E_LT_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_RT_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_LB_CHILD]->GetBound());
		m_kAABB.Merge(m_pkQuadNodeChild[E_RB_CHILD]->GetBound());
	}
	else
	{
		// 当前节点为叶子节点
		//
		// 节点层级
		m_uiLevel = 0;	

		// 节点采样间距
		m_uiSpacing = 1;

		// 节点误差
		m_fError = 0.0f;
		m_fAccumError = 0.0f;

		// 计算节点包围盒
		CalcBound(m_kAABB);
	}
	// @}

	// 构建当前Node的Mesh
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
	// 计算圆形与AABB求交
	//	1.AABB中心与Circle中心距离在两个维度方向都不大于2者半径之和
	//	 (此处为Circle位于AABB上下左右四条边处判断)
	//	2.AABB四角与Circle中心连线距离平方不大于Circle半径平方
	//   (此处为Circle位于AABB四角)
	// @{
	bool bIntersect = false;
	const sdVector3& kMinimum = m_kAABB.GetMinimum();
	const sdVector3& kMaximum = m_kAABB.GetMaximum();
	float fAABBCenterX = (kMaximum.m_fX + kMinimum.m_fX) / 2.0f;	///< AABB中心
	float fAABBCenterY = (kMaximum.m_fY + kMinimum.m_fY) / 2.0f;
	float fAABBHalfSizeX = (kMaximum.m_fX - kMinimum.m_fX) / 2.0f;	///< AABB半长
	float fAABBHalfSizeY = (kMaximum.m_fY - kMinimum.m_fY) / 2.0f;
	float fMaxDistanceX = fAABBHalfSizeX + fRadius;		///< AABB与Circle之间能够相交时,中心的最大距离
	float fMaxDistanceY = fAABBHalfSizeY + fRadius;
	float fDistanceX = fabs(fCenterX - fAABBCenterX);	///< AABB与Circle的中心距离
	float fDistanceY = fabs(fCenterY - fAABBCenterY);
	if (fDistanceX <= fMaxDistanceX && fDistanceY <= fMaxDistanceY)
	{
		float fDeltaX = max(fDistanceX - fAABBHalfSizeX, 0.0f);
		float fDeltaY = max(fDistanceY - fAABBHalfSizeY, 0.0f);
		if (fDeltaX * fDeltaX + fDeltaY * fDeltaY <= fRadius * fRadius)
			bIntersect = true;
	}
	// @}

	// 处理更新
	//	1.注意是先递归,再处理节点,保证子节点被先处理
	//	1.笔刷一般不大,不需要区分全覆盖/部分覆盖/不相交
	// @{
	if (bIntersect)
	{	
		if (m_uiLevel != 0)
		{
			// 中间节点
			//
			// 向下遍历
			m_pkQuadNodeChild[E_LT_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_RT_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_LB_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			m_pkQuadNodeChild[E_RB_CHILD]->UpdateGeometry(fCenterX, fCenterY, fRadius);
			
			// 更新当前节点误差与累积误差
			m_fError = CalcError();

			m_fAccumError = 0.0f;
			m_fAccumError = max(m_pkQuadNodeChild[E_LT_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_RT_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_LB_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError = max(m_pkQuadNodeChild[E_RB_CHILD]->GetAccumError(), m_fAccumError);
			m_fAccumError += m_fError;

			// 更新当前节点GeometryMesh
			float fMinHeight = FLT_MAX;
			float fMaxHeight = -FLT_MAX;
			if (m_pkQuadMesh)
			{
				m_pkQuadMesh->UpdateGeometry(fMinHeight, fMaxHeight, m_fAccumError);
			}

			// 更新当前节点包围盒(从下往上合并)
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
			// 叶子节点
			//
			// 更新当前节点GeometryMesh
			float fMinHeight = FLT_MAX;
			float fMaxHeight = -FLT_MAX;
			m_pkQuadMesh->UpdateGeometry(fMinHeight, fMaxHeight, m_fAccumError);

			// 设置包围盒
			m_kAABB.SetMinimumZ(fMinHeight);
			m_kAABB.SetMaximumZ(fMaxHeight);
		}
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdQuadNode::Cull(const NiCamera& kCamera, NiFrustumPlanes& kFrustumPlanes, std::vector<NiMesh*>& kMeshVec)
{
	// 对当前节点进行裁剪,检测
	//	1.是否可见
	//	2.是否继续向下遍历
	//		(1)向下裁剪
	//		(2)不向下裁剪		
	//
	// 包围球
	sdVector3 kCenter, kHalfSize;
	m_kAABB.GetCenter(kCenter);
	m_kAABB.GetSize(kHalfSize);

	NiPoint3 kTCenter(kCenter.m_fX, kCenter.m_fY, kCenter.m_fZ);

	// 1.相机裁剪(参考自Ogre)
	//
	// 依次用视形体各个面进行裁剪,只有均不在各个面的背面才有可能可见
	// \TODO 考虑找一些更严密的AABB与Frustum求交算法
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
			// 平面到包围盒中心距离
			float fDistance = kPlane.Distance(kTCenter);

			// Calculate the maximise allows absolute distance for
			// the distance between box centre and plane
			// 包围盒在平面法线方向投影的半长
			const NiPoint3& kNormal = kPlane.GetNormal();
			float fMaxAbsDistance = fabsf(kNormal.x * kHalfSize.m_fX) + 
				fabsf(kNormal.y * kHalfSize.m_fY) +	fabsf(kNormal.z * kHalfSize.m_fZ);

			// 只要包围盒位于任一平面负面,直接剔除
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


	// 2.度量是否需要进一步向下遍历
	// @{
	bool bShouldSubdivide = true;
//	if (bVisible)
//	{
//		if (!pkTerrain->GetEnableLOD() || m_uiLevel >= pkTerrain->GetMeshLevel())
//		{
//			// 向下裁剪的原因:
//			//	1.地形禁用LOD
//			//	2.当前层级没有生成Mesh
//			bShouldSubdivide = true;
//		}
//		else
//		{
//			//
//			// 继续判断:
//			//	1.计算节点包围盒到视点的最近距离
//			//	2.根据节点对下一级误差误差计算允许观察距离
//			//	3.比较两个距离确定是否细分
//			//
//			// \TODO 有待于进一步完善切换
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


	// 处理
	// @{
	if (bVisible)
	{
		if (bShouldSubdivide)
		{
			// 向下遍历
			if (IsLeaf())
			{
				// 叶子节点直接提取Mesh
				if (m_pkQuadMesh)
					kMeshVec.push_back(m_pkQuadMesh);
			}
			else
			{
				// 中间节点,依照从近到远的次序依次处理子节点
				//
				// 计算子节点中心到观察点距离
				float vfDistance[NUM_CHILDREN] =  { 0 };
				sdVector3 kCamPosition(kCamera.GetWorldTranslate().x, kCamera.GetWorldTranslate().y, kCamera.GetWorldTranslate().z);
				for (int i = 0; i < NUM_CHILDREN; ++i)
				{
					vfDistance[i] = (m_pkQuadNodeChild[i]->GetBound().GetCenter() - kCamPosition).SquaredLength();
				}

				// 每次取出距离数组中最小值的Node处理, 并将其距离置为无穷大
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
			// 停止遍历提取Mesh
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

	// 叶子节点无误差
	if (m_uiLevel == 0)
		return 0.0f;

	// 获取所有精细级别采样点当对于当前级别采样点误差最大值
	float fError = 0.0f;
	uint uiSize = m_pkTerrain->GetTileSize();
	uint uiStartX = m_uiOriginX;
	uint uiStartY = m_uiOriginY;
	uint uiSpacing = m_uiSpacing / 2;		///< m_uiSpacing必定大于1
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

	// 计算高度范围
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

	// 设置包围盒范围
	kAABB.SetMinimum((float)m_uiOriginX, (float)m_uiOriginY, fMinHeight);
	kAABB.SetMaximum((float)(m_uiOriginX + uiSize * uiSpacing), (float)(m_uiOriginY + uiSize * uiSpacing), fMaxHeight);
}
//-------------------------------------------------------------------------------------------------