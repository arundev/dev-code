//*************************************************************************************************
// ����:	������CookBuilder
//---------------------------------------------------------
// ����:		
// ����:		2013-07-09
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_BUILDER_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_BUILDER_H__

#define PHYSX_BULID_FLOAT_TOLERANCE	0.01f		///< ���������̶�

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPhysXSceneMgr;
class sdPhysXMemoryWriteStream;
//-------------------------------------------------------------------------------------------------
// ������,����Ԥ���������������������м��ļ�
//-------------------------------------------------------------------------------------------------
class sdPhysXCookBuilder : public NiRefObject
{
public:
	sdPhysXCookBuilder();
	~sdPhysXCookBuilder();

	// ���Mesh����(û�д���������õ�����)
	//	@param[in]	spMesh		Mesh����
	//	@param[in]	nxGroup		Mesh������������
	//	@param[in]	bFlipNormal	�Ƿ�ת����
	bool AddMesh(NiMesh* spMesh, NxCollisionGroup nxGroup, bool bFlipNormal = false);

	// ���Entity����(�����������)
	//	@param[in]	spNode		������
	//	@param[in]	kTransform	����任
	//	@param[in]	szNIFName	��������·��
	//	@param[in]	nxGroup		������������
	//	@param[in]	bFlipNormal	�Ƿ�ת����
	bool AddMesh(NiNode* spNode, 
		const NiTransform& kTransform, 
		const char* szNIFName,
		NxCollisionGroup nxGroup, 
		bool bFlipNormal = false);

	// ����ΪPhysX����
	//	@param[out]	kStream		���������
	//	@return					������ݳߴ�(in byte)
	size_t Cook(sdPhysXMemoryWriteStream& kStream);

protected:
	// ����һ��Cookʵ��
	class sdCookInstance : public NiRefObject
	{
	public:
		NiAVObjectPtr m_spObject;	///< ������
		NxMat34 m_nxTransform;		///< �������任(��ת��ƽ��)
		float m_fScale;				///< ��������
		bool m_bFlipNormal;			///< �����Ƿ�ת����
		NxCollisionGroup m_nxGroup;	///< ������������
		ushort m_usCrossID;
	};

	typedef std::vector<sdCookInstance> CookInstanceVec;
	typedef std::vector<sdCookInstance>::iterator CookInstanceVecItr;

	typedef std::map<uint, CookInstanceVec> CookInstanceVecMap;
	typedef std::map<uint, CookInstanceVec> CookInstanceVecMapItr;

	// ԭ�Ͷ���ļ�
	class sdPrototypeKey : public NiMemObject
	{
	public:
		float m_fScale;	///< ʵ������

	public:
		bool operator<(const sdPrototypeKey& rhs) const 
		{ 
			return rhs.m_fScale - m_fScale > PHYSX_BULID_FLOAT_TOLERANCE;
		}
	};

	// ԭ�Ͷ����ֵ
	class sdPrototypeValue : public NiMemObject
	{
	public:
		uint m_uiRef;					///< ʵ�������ô���
		CookInstanceVecMap m_kInstMap;	///< ʵ���ڳ�����������ķֲ�

	public:
		sdPrototypeValue(uint uiRef) : m_uiRef(uiRef){}
	};

	// �ڶ���ӳ��,������ֵΪ��
	class sdScaledPrototypeMap : public std::map<sdPrototypeKey, sdPrototypeValue>
	{
	public:
		uint m_uiTotalRef;			///< ��ԭ�͵��ܹ����ô���
		bool m_bCrossSection;		///< ???
		NiAVObjectPtr m_spObject;	///< 

	public:
		sdScaledPrototypeMap() : m_uiTotalRef(0), m_bCrossSection(false){}
	};

	typedef sdScaledPrototypeMap::iterator ScaledPrototypeMapItr;

	// ��һ��ӳ��,�Զ���NIF·��Ϊ��
	typedef std::map<std::string, sdScaledPrototypeMap> NameToScaledPrototypeMapMap;
	typedef std::map<std::string, sdScaledPrototypeMap>::iterator NameToScaledPrototypeMapMapItr;

protected:

	// ������ͼ(��������˹�����,�ڲ�Ϊ��������,��һ��ΪNIF·��,�ڶ���Ϊ��������)
	NameToScaledPrototypeMapMap m_kSharedPrototypeMap;
	uint m_uiSharedPrototypeCount;
	
	// ������ͼ(����û�н��й�����)
	CookInstanceVecMap m_kNoShareMeshInstMap;


	sdPhysXSceneMgr* m_pkPhysXSceneMgr;	///< ��ǰ�����ĳ���������
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif