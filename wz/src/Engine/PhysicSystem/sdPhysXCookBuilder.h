//*************************************************************************************************
// 内容:	场景的CookBuilder
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_BUILDER_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_BUILDER_H__

#define PHYSX_BULID_FLOAT_TOLERANCE	0.01f		///< 浮点数容忍度

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPhysXSceneMgr;
class sdPhysXMemoryWriteStream;
//-------------------------------------------------------------------------------------------------
// 构建器,用于预处理场景的物理数据生成中间文件
//-------------------------------------------------------------------------------------------------
class sdPhysXCookBuilder : public NiRefObject
{
public:
	sdPhysXCookBuilder();
	~sdPhysXCookBuilder();

	// 添加Mesh对象(没有处理跨区域复用的问题)
	//	@param[in]	spMesh		Mesh对象
	//	@param[in]	nxGroup		Mesh对象所属分组
	//	@param[in]	bFlipNormal	是否反转法线
	bool AddMesh(NiMesh* spMesh, NxCollisionGroup nxGroup, bool bFlipNormal = false);

	// 添加Entity对象(跨区域对象复用)
	//	@param[in]	spNode		对象树
	//	@param[in]	kTransform	对象变换
	//	@param[in]	szNIFName	对象数据路径
	//	@param[in]	nxGroup		对象所属分组
	//	@param[in]	bFlipNormal	是否反转法线
	bool AddMesh(NiNode* spNode, 
		const NiTransform& kTransform, 
		const char* szNIFName,
		NxCollisionGroup nxGroup, 
		bool bFlipNormal = false);

	// 烘培为PhysX数据
	//	@param[out]	kStream		输出数据流
	//	@return					输出数据尺寸(in byte)
	size_t Cook(sdPhysXMemoryWriteStream& kStream);

protected:
	// 描述一个Cook实例
	class sdCookInstance : public NiRefObject
	{
	public:
		NiAVObjectPtr m_spObject;	///< 对象树
		NxMat34 m_nxTransform;		///< 对象刚体变换(旋转与平移)
		float m_fScale;				///< 对象缩放
		bool m_bFlipNormal;			///< 对象是否反转法线
		NxCollisionGroup m_nxGroup;	///< 对象所属分组
		ushort m_usCrossID;
	};

	typedef std::vector<sdCookInstance> CookInstanceVec;
	typedef std::vector<sdCookInstance>::iterator CookInstanceVecItr;

	typedef std::map<uint, CookInstanceVec> CookInstanceVecMap;
	typedef std::map<uint, CookInstanceVec> CookInstanceVecMapItr;

	// 原型对象的键
	class sdPrototypeKey : public NiMemObject
	{
	public:
		float m_fScale;	///< 实例缩放

	public:
		bool operator<(const sdPrototypeKey& rhs) const 
		{ 
			return rhs.m_fScale - m_fScale > PHYSX_BULID_FLOAT_TOLERANCE;
		}
	};

	// 原型对象的值
	class sdPrototypeValue : public NiMemObject
	{
	public:
		uint m_uiRef;					///< 实例被引用次数
		CookInstanceVecMap m_kInstMap;	///< 实例在场景各个区域的分布

	public:
		sdPrototypeValue(uint uiRef) : m_uiRef(uiRef){}
	};

	// 第二级映射,以缩放值为键
	class sdScaledPrototypeMap : public std::map<sdPrototypeKey, sdPrototypeValue>
	{
	public:
		uint m_uiTotalRef;			///< 该原型的总共引用次数
		bool m_bCrossSection;		///< ???
		NiAVObjectPtr m_spObject;	///< 

	public:
		sdScaledPrototypeMap() : m_uiTotalRef(0), m_bCrossSection(false){}
	};

	typedef sdScaledPrototypeMap::iterator ScaledPrototypeMapItr;

	// 第一级映射,以对象NIF路径为键
	typedef std::map<std::string, sdScaledPrototypeMap> NameToScaledPrototypeMapMap;
	typedef std::map<std::string, sdScaledPrototypeMap>::iterator NameToScaledPrototypeMapMapItr;

protected:

	// 物理场景图(对象进行了共享处理,内部为两级索引,第一级为NIF路径,第二级为对象缩放)
	NameToScaledPrototypeMapMap m_kSharedPrototypeMap;
	uint m_uiSharedPrototypeCount;
	
	// 物理场景图(对象没有进行共享处理)
	CookInstanceVecMap m_kNoShareMeshInstMap;


	sdPhysXSceneMgr* m_pkPhysXSceneMgr;	///< 当前场景的场景管理器
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif