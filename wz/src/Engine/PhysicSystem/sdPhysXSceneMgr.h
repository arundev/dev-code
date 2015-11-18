//*************************************************************************************************
// ����:	������������
//---------------------------------------------------------
// ����:		
// ����:		2013-07-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_SCENE_MGR_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_SCENE_MGR_H__

#define SECTION_SIZE				64				///< Ĭ�Ϸ����ߴ�
#define INVALID_SECTION_ID			((ushort)-1)	///< ��Ч���
#define INVALID_CROSS_ID			((ushort)-1)	///< ��Ч���

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXQuadSection;
NiSmartPointer(sdPhysXQuadSection);

class sdPhysXMemoryWriteStream;
//-----------------------------------------------------------------------------
// ������������
//	1.ÿ��PhysXScene��ӦEngine��һ��sdMap
//	2.ÿ��PhysXScene��ȫ���Ϊ���PhysXQuadSection(����sdTerrain-sdTile)
//-----------------------------------------------------------------------------
class sdPhysXSceneMgr : public NiRefObject
{
public:
	sdPhysXSceneMgr();
	~sdPhysXSceneMgr();

	//bool	LoadScene(sdMap* pkMap);
	//void	DestroyScene();



	// ������Mesh���ൽPhysX������
	//	@param[out]	kStream		�������
	//	@param[in]	spMesh		����Mesh����
	//	@param[in]	fScale		��������
	//	@param[in]	bFlipNormal	�������Ƿ�ת
	//	@return					������ݳߴ�
	uint CookMesh(sdPhysXMemoryWriteStream& kStream, NiMesh* spMesh, float fScale = 1.f, bool bFlipNormal = false);

	// ������Mesh���ൽPhysX������
	//	@param[out]	kStream		�������
	//	@param[in]	spObject	���������
	//	@param[in]	fScale		��������
	//	@param[in]	bFlipNormal	�������Ƿ�ת
	//	@return					������ݳߴ�
	uint CookObject(sdPhysXMemoryWriteStream& kStream, NiAVObject* spAVObject, float fScale = 1.f, bool bFlipNormal = false);


	// ���Է���
	uint GetSceneSize() const { return m_uiSceneSize;}
	uint GetSectionSize() const { return m_uiSectionSize;}


protected:
	uint m_uiSceneSize;		///< һ��Ϊ256/512/1024/2048
	uint m_uiSectionSize;	///< һ��Ϊ64
	
	// �ռ��Ĳ����ָ�(ò��ûɶ��Ҫ�����Զ�̬�������)
	sdPhysXQuadSectionPtr m_pkRootQuad;

	// PhysX������
	// @{
	


	

	bool m_bUse16BitIndex;		///< true-��ǰʹ��16bit����, false-��ǰʹ��32bit����
	NiUInt8* m_pkIndexCache;	///< �������ݻ���
	NiPoint3* m_pkVertexCache;	///< �������ݻ���
	// @}
};
NiSmartPointer(sdPhysXSceneMgr);
//-----------------------------------------------------------------------------
// �������Ĳ����ڵ�
//	1.Ҷ�ӽڵ���դ����ʽ��ԭ�㿪ʼ����
//	2.��Ҷ���ޱ���
//-----------------------------------------------------------------------------
class sdPhysXQuadSection : public NiRefObject
{
public:
	// �Ĳ����ڵ�ö��
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

	// �����巶Χ�����Ĳ���
	//	@param[in]	pkPhysXSceneMgr				����������
	//	@param[in]	uiOriginX,uiOriginY,uiSize	��ǰ������ʼλ����ߴ�
	bool Build(sdPhysXSceneMgr* pkPhysXSceneMgr, uint uiOriginX, uint uiOriginY, uint uiSize);

	// ��ָ����Χ�вü��Ĳ���,��ȡ�ཻ��Ҷ��SectionID
	//	@param[in]	kBound			�����Χ��
	//	@param[out]	kSectionIDVec	�����ཻ������ľ���б�
	void Cull(const NiBound& kBound, std::vector<ushort>& kSectionIDVec);

protected:
	// ����ID
	ushort m_usSectionID;	

	// ���ӽڵ�
	sdPhysXQuadSectionPtr m_pkQuadNodeChild[NUM_CHILDREN];	

	// ����2D��Χ
	uint m_uiOriginX;	
	uint m_uiOriginY;	
	uint m_uiSize;

	// ����������
	sdPhysXSceneMgr* m_pkPhysXSceneMgr;
};
NiSmartPointer(sdPhysXQuadSection);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif