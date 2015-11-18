#include "sdPhysicsSystemPCH.h"
#include "sdPhysXCookBuilder.h"
#include "sdPhysXSceneMgr.h"

#include <NiPhysXTypes.h>

using namespace Base::Math;

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
sdPhysXCookBuilder::sdPhysXCookBuilder()
{

}
//-------------------------------------------------------------------------------------------------
sdPhysXCookBuilder::~sdPhysXCookBuilder()
{

}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookBuilder::AddMesh(NiMesh* spMesh, NxCollisionGroup nxGroup, bool bFlipNormal)
{
	NIASSERT(spMesh);

	// ����Mesh
	spMesh->Update(0.f);
	spMesh->UpdateProperties();
	spMesh->UpdateEffects();

	// ����Mesh�����ǵ�Section��Χ
	typedef std::vector<ushort> SectionIDVec;
	typedef std::vector<ushort>::iterator SectionIDVecItr;
	SectionIDVec kSectionIDVec;
	//m_pkRootQuad->Cull(spMesh->GetWorldBound(), kSectionIDVec);

	// ����ʵ��
	sdCookInstance kCookInstance;
	kCookInstance.m_nxTransform.id();
	kCookInstance.m_fScale = 1.f;
	kCookInstance.m_bFlipNormal = false;
	kCookInstance.m_nxGroup = nxGroup;
	kCookInstance.m_usCrossID = INVALID_CROSS_ID;

	// ����ÿ��Section���б�
	SectionIDVecItr itr = kSectionIDVec.begin();
	SectionIDVecItr itr_end = kSectionIDVec.end();
	for (; itr != itr_end; ++itr)
	{
		ushort usSectionID = *itr;
		//NIASSERT(usSectionID < uiTotalSectionCount);
		m_kNoShareMeshInstMap[usSectionID].push_back(kCookInstance);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookBuilder::AddMesh(NiNode* spNode, const NiTransform& kTransform, const char* szNIFName, NxCollisionGroup nxGroup, bool bFlipNormal)
{
	NIASSERT(spNode);
	NIASSERT(szNIFName);

	// ���뵽����ͼ
	std::pair<NameToScaledPrototypeMapMapItr, bool> kResult = m_kSharedPrototypeMap.insert(std::make_pair(szNIFName, sdScaledPrototypeMap()));
	sdScaledPrototypeMap& kScaledPrototypeMap = kResult.first->second;

	kScaledPrototypeMap.m_uiTotalRef++;		///< ���Ӽ���

	// ����CD�ļ�
	if (!kScaledPrototypeMap.m_spObject)	
		kScaledPrototypeMap.m_spObject = spNode;

	// ���������Χ��
	NiBound kWorldBound;
	kWorldBound.Update(kScaledPrototypeMap.m_spObject->GetWorldBound(), kTransform);

	// ���������ǵ�Section��Χ
	typedef std::vector<ushort> SectionIDVec;
	typedef std::vector<ushort>::iterator SectionIDVecItr;
	SectionIDVec kSectionIDVec;
	ushort usShareID;
	//m_pkRootQuad->Cull(spMesh->GetWorldBound(), kSectionIDVec);

	// 
	sdPrototypeKey kPrototypeKey;
	kPrototypeKey.m_fScale = kTransform.m_fScale;

	std::pair<ScaledPrototypeMapItr, bool> kScaledResult = kScaledPrototypeMap.insert(std::make_pair(kPrototypeKey, sdPrototypeValue(0)));
	sdPrototypeValue& kPrototypeValue = kScaledResult.first->second;

	kPrototypeValue.m_uiRef++;		///< ���Ӽ���

	NIASSERT(sdMath::FloatEqual(kScaledResult.first->first.m_fScale, kPrototypeKey.m_fScale, PHYSX_BULID_FLOAT_TOLERANCE));

	if (kScaledPrototypeMap.m_uiTotalRef == 1)
	{
		if (usShareID != INVALID_CROSS_ID)
		{
			// ������ǹ���ģ��
			kScaledPrototypeMap.m_bCrossSection = true;	///< ���ÿ�������
			++m_uiSharedPrototypeCount;					
		}
	}
	else if (kScaledPrototypeMap.m_uiTotalRef == 2)
	{
		if (kScaledPrototypeMap.m_bCrossSection)
		{

		}
		else
		{
			++m_uiSharedPrototypeCount;
		}
	}

	// ����ʵ��
	sdCookInstance kCookInstance;
	NiPhysXTypes::NiPoint3ToNxVec3(kTransform.m_Translate, kCookInstance.m_nxTransform.t);
	NiPhysXTypes::NiMatrix3ToNxMat33(kTransform.m_Rotate, kCookInstance.m_nxTransform.M);
	kCookInstance.m_fScale = kTransform.m_fScale;
	kCookInstance.m_bFlipNormal = false;
	kCookInstance.m_nxGroup = nxGroup;
	kCookInstance.m_usCrossID = INVALID_CROSS_ID;

	// ����ÿ��Section���б�
	SectionIDVecItr itr = kSectionIDVec.begin();
	SectionIDVecItr itr_end = kSectionIDVec.end();
	for (; itr != itr_end; ++itr)
	{
		ushort usSectionID = *itr;
		//NIASSERT(usSectionID < uiTotalSectionCount);
		
		CookInstanceVec& kCookInstanceVec = kPrototypeValue.m_kInstMap[usSectionID];
		kCookInstanceVec.push_back(kCookInstance);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
size_t sdPhysXCookBuilder::Cook(sdPhysXMemoryWriteStream& kStream)
{

	// ����������ͼ(�������)
	NameToScaledPrototypeMapMapItr kNTSPMMIter = m_kSharedPrototypeMap.begin();
	NameToScaledPrototypeMapMapItr kNTSPMMIter_End = m_kSharedPrototypeMap.end();
	for (; kNTSPMMIter != kNTSPMMIter_End; ++kNTSPMMIter)
	{
		sdScaledPrototypeMap& kScaledPrototypeMap = kNTSPMMIter->second;

		if (kScaledPrototypeMap.m_uiTotalRef == 0)
		{
			NIASSERT(0);
			continue;
		}

		if (kScaledPrototypeMap.m_uiTotalRef > 1 || kScaledPrototypeMap.m_bCrossSection)
		{

		}



	}



	return 0;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM