//*************************************************************************************************
// 内容:	地形Mesh绘制对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-01
// 最后修改:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_QUAD_NODE_H__
#define _TERRAIN_QUAD_NODE_H__
#include <sdVector2.h>
#include <sdVector4.h>
#include <sdAxisAlignedBox.h>
#include "sdQuadMesh.h"

class sdTerrain;
class sdQuadNode;
NiSmartPointer(sdQuadNode);
//-------------------------------------------------------------------------------------------------
// 地形QuadTree节点,每个节点只有一个Mesh,暂时木有支持节点内LOD.
//
//	\TODO 考虑预分配节点以优化内存
//	\TODO 考虑到以后实施无限地图需要多棵四叉树,所以最好不要有树相关的类静态成员变量
//	\TODO 考虑以后实现每个节点内多级LOD
//	\TODO 考虑实现更平滑的LOD，目前LOD没有过滤，直接采样
//-------------------------------------------------------------------------------------------------
class sdQuadNode : public NiRefObject
{
public:
	// 四叉树节点枚举
	enum EQuadNodeIndex
	{
		E_LT_CHILD = 0,
		E_RT_CHILD,
		E_LB_CHILD,
		E_RB_CHILD,
		NUM_CHILDREN,
	};

public:
	sdQuadNode(sdTerrain* pkTerrain = NULL);
	~sdQuadNode();

	// 从高度图构建四叉树,从指定范围向下细分,直到尺寸等于QuadSize
	bool	Build(Base::Math::sdVector4ui& kRect, sdQuadNode* pkParent);

	// 销毁
	void	Destroy();

	// 更新四叉树指定区域的几何体(一般是高度图被修改了)
	void	UpdateGeometry(float fCenterX, float fCenterY, float fRadius);

	// 用指定相机裁剪四叉树
	void	Cull(const NiCamera& kCamera, NiFrustumPlanes& kFrustumPlanes,std::vector<NiMesh*>& kMeshVec);

	// 属性访问
	bool	IsLeaf() const { return (m_uiLevel == 0);}
	uint	GetLevel() const { return m_uiLevel;}
	uint	GetSpacing() const { return m_uiSpacing;}
	float	GetError() const { return m_fError;}
	float	GetAccumError() const { return m_fAccumError;}

	const Base::Math::sdAxisAlignedBox& GetBound() const { return m_kAABB;}

protected:
	// 计算当前节点与下一节点采样高度图之间的最大误差绝对值
	float	CalcError();

	// 计算当前节点高度图的包围盒
	void	CalcBound(Base::Math::sdAxisAlignedBox& kAABB);

protected:
	// 所属地形对象
	sdTerrain* m_pkTerrain;

	// 父节点与孩子节点(注意避免互相引用)
	sdQuadNodePtr	m_pkQuadNodeChild[NUM_CHILDREN];
	sdQuadNode*		m_pkQuadNodePar;
	
	// 当前节点的Mesh
	sdQuadMeshPtr	m_pkQuadMesh;

	uint	m_uiLevel;		// 当前节点深度(叶节点为0,向上递增,根节点最大)
	uint	m_uiSpacing;	// 当前节点栅格尺寸(叶节点为1,向上几何级数增加)
	float	m_fError;		// 当前节点与下一节点最大误差,作为地形Tile裙子高度
	float	m_fAccumError;	// 当前节点累积最大误差(叶节点为0,从叶节点向上累积)

	// 当前节点起始点位置
	uint	m_uiOriginX;	
	uint	m_uiOriginY;		

	// 当前节点包围盒(该节点往下整个四叉树对应高度图的包围盒)
	Base::Math::sdAxisAlignedBox	m_kAABB;	
};
//-------------------------------------------------------------------------------------------------
#endif