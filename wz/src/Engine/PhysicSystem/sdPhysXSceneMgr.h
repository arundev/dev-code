//*************************************************************************************************
// 内容:	物理场景管理器
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_SCENE_MGR_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_SCENE_MGR_H__

#define SECTION_SIZE				64				///< 默认分区尺寸
#define INVALID_SECTION_ID			((ushort)-1)	///< 无效句柄
#define INVALID_CROSS_ID			((ushort)-1)	///< 无效句柄

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXQuadSection;
NiSmartPointer(sdPhysXQuadSection);

class sdPhysXMemoryWriteStream;
//-----------------------------------------------------------------------------
// 物理场景管理器
//	1.每个PhysXScene对应Engine的一个sdMap
//	2.每个PhysXScene完全拆分为多个PhysXQuadSection(类似sdTerrain-sdTile)
//-----------------------------------------------------------------------------
class sdPhysXSceneMgr : public NiRefObject
{
public:
	sdPhysXSceneMgr();
	~sdPhysXSceneMgr();

	//bool	LoadScene(sdMap* pkMap);
	//void	DestroyScene();



	// 将输入Mesh烘培到PhysX数据流
	//	@param[out]	kStream		输出数据
	//	@param[in]	spMesh		输入Mesh对象
	//	@param[in]	fScale		对象缩放
	//	@param[in]	bFlipNormal	对象法线是否反转
	//	@return					输出数据尺寸
	uint CookMesh(sdPhysXMemoryWriteStream& kStream, NiMesh* spMesh, float fScale = 1.f, bool bFlipNormal = false);

	// 将输入Mesh烘培到PhysX数据流
	//	@param[out]	kStream		输出数据
	//	@param[in]	spObject	输入对象树
	//	@param[in]	fScale		对象缩放
	//	@param[in]	bFlipNormal	对象法线是否反转
	//	@return					输出数据尺寸
	uint CookObject(sdPhysXMemoryWriteStream& kStream, NiAVObject* spAVObject, float fScale = 1.f, bool bFlipNormal = false);


	// 属性访问
	uint GetSceneSize() const { return m_uiSceneSize;}
	uint GetSectionSize() const { return m_uiSectionSize;}


protected:
	uint m_uiSceneSize;		///< 一般为256/512/1024/2048
	uint m_uiSectionSize;	///< 一般为64
	
	// 空间四叉树分割(貌似没啥必要，可以动态计算出来)
	sdPhysXQuadSectionPtr m_pkRootQuad;

	// PhysX物理场景
	// @{
	


	

	bool m_bUse16BitIndex;		///< true-当前使用16bit索引, false-当前使用32bit索引
	NiUInt8* m_pkIndexCache;	///< 索引数据缓存
	NiPoint3* m_pkVertexCache;	///< 顶点数据缓存
	// @}
};
NiSmartPointer(sdPhysXSceneMgr);
//-----------------------------------------------------------------------------
// 物理场景四叉树节点
//	1.叶子节点以栅格形式从原点开始编码
//	2.非叶子无编码
//-----------------------------------------------------------------------------
class sdPhysXQuadSection : public NiRefObject
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
	sdPhysXQuadSection();
	~sdPhysXQuadSection();

	// 从整体范围构建四叉树
	//	@param[in]	pkPhysXSceneMgr				场景管理器
	//	@param[in]	uiOriginX,uiOriginY,uiSize	当前区块起始位置与尺寸
	bool Build(sdPhysXSceneMgr* pkPhysXSceneMgr, uint uiOriginX, uint uiOriginY, uint uiSize);

	// 用指定包围盒裁剪四叉树,获取相交的叶子SectionID
	//	@param[in]	kBound			对象包围盒
	//	@param[out]	kSectionIDVec	对象相交的区块的句柄列表
	void Cull(const NiBound& kBound, std::vector<ushort>& kSectionIDVec);

protected:
	// 区块ID
	ushort m_usSectionID;	

	// 孩子节点
	sdPhysXQuadSectionPtr m_pkQuadNodeChild[NUM_CHILDREN];	

	// 区块2D范围
	uint m_uiOriginX;	
	uint m_uiOriginY;	
	uint m_uiSize;

	// 所属管理器
	sdPhysXSceneMgr* m_pkPhysXSceneMgr;
};
NiSmartPointer(sdPhysXQuadSection);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif