#include "sdPhysicsSystemPCH.h"
#include "sdPhysicsSystem.h"
#include "sdPhysXAllocator.h"
#include "sdPhysXStream.h"
#include "sdPhysXOutputStream.h"
#include "sdPhysXCollisionGroup.h"

#include <NxSceneDesc.h>
#include <NxScene.h>
#include <NxHeightField.h>
#include <NxHeightFieldDesc.h>
#include <NxHeightFieldShape.h>
#include <NxHeightFieldShapeDesc.h>
#include <NxHeightFieldSample.h>
#include <NxActor.h>
#include <NxMaterial.h>

#include <NiPhysXTypes.h>

#include <sdMap.h>
#include <sdTerrain.h>

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
void sdPhysXTriggerReport::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{

}
//-------------------------------------------------------------------------------------------------
NxControllerAction sdPhysXControllerHitReport::onShapeHit(const NxControllerShapeHit& hit)
{
	return NX_ACTION_NONE;
}
//-------------------------------------------------------------------------------------------------
NxControllerAction sdPhysXControllerHitReport::onControllerHit(const NxControllersHit& hit)
{
	return NX_ACTION_NONE;
}
//-------------------------------------------------------------------------------------------------
sdPhysicsSystem::sdPhysicsSystem()
: m_bInitialized(false)
, m_bSceneLoaded(false)
, m_pkPhysicsSDK(NULL)
, m_pkAllocator(NULL)
, m_pkOutputStream(NULL)
//, m_pkCooking(NULL)
, m_pkControllerManager(NULL)
, m_pkUserTriggerReport(NULL)
, m_pkUserControllerHitReport(NULL)
{
}
//-------------------------------------------------------------------------------------------------
sdPhysicsSystem::~sdPhysicsSystem()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdPhysicsSystem::Initialize()
{
	if (m_bInitialized)
	{
		NIASSERT(0);

		Destroy();
		m_bInitialized = false;
	}

	// �����ڴ������
	m_pkAllocator = NiNew sdPhysXAllocator;
	NIASSERT(m_pkAllocator);

	// �����������������
	m_pkOutputStream = NiNew sdPhysXOutputStream;
	NIASSERT(m_pkOutputStream);

	// ����PhysX,��ʼ������
	NxSDKCreateError kErrorCode;
	NxPhysicsSDKDesc kDesc;
	m_pkPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, m_pkOutputStream, kDesc, &kErrorCode);
	NIASSERT(m_pkPhysicsSDK);

	m_pkPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.1f);
	m_pkPhysicsSDK->setParameter(NX_BOUNCE_THRESHOLD, -0.5f);
	m_pkPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.f);
	m_pkPhysicsSDK->setParameter(NX_VISUALIZE_BODY_AXES, 0.2f);
	m_pkPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.f);
	m_pkPhysicsSDK->setParameter(NX_VISUALIZE_CLOTH_MESH, 1.f);
//	m_pkPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1.f);
//	m_pkPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXESS, 1.f);

	// ����Cook��
	//m_pkCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	//NIASSERT(m_pkCooking);
	//m_pkCooking->NxInitCooking();

	//
	m_pkUserTriggerReport = NiNew sdPhysXTriggerReport;
	NIASSERT(m_pkUserTriggerReport);

	return m_bInitialized = true;
}
//-------------------------------------------------------------------------------------------------
void sdPhysicsSystem::Destroy()
{
	if (m_bInitialized)
	{
		//NiDelete m_pkAllocator;
		//m_pkAllocator = NULL;

		//NiDelete m_pkOutputStream;
		//m_pkOutputStream = NULL;
	
		//m_pkCooking->NxCloseCooking();
		//m_pkCooking = NULL;

		//NxReleasePhysicsSDK(m_pkPhysicsSDK);
		//m_pkPhysicsSDK = NULL;

		//m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdPhysicsSystem::LoadScene(sdMap* pkMap)
{
	if (!m_bInitialized || !pkMap)
		return false;

	sdTerrain* pkTerrain = pkMap->GetTerrain();
	NIASSERT(pkTerrain);

	// �����հ�NxScene
	CreateEmptyScene(pkTerrain->GetTerrainSize());
	NIASSERT(m_pkScene);

	float fTimeStep = 1.f / 60.f;
	m_pkScene->setTiming(fTimeStep, 6, NX_TIMESTEP_FIXED);
	m_pkScene->setUserTriggerReport(m_pkUserTriggerReport);

	NxMaterial* pkDefaultMaterial = m_pkScene->getMaterialFromIndex(0);
	NIASSERT(pkDefaultMaterial);
	pkDefaultMaterial->setRestitution(0.1f);
	pkDefaultMaterial->setStaticFriction(0.5f);
	pkDefaultMaterial->setDynamicFriction(0.5f);

	// ���ص������ݵ�NxScene
	CreateTerrain(pkTerrain);

	// ����Ԥ���ɵ�������ݵ�NxScene
	

	// ���ɵ��εı߽�
	CreateTerrainBound(pkTerrain);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdPhysicsSystem::CreateEmptyScene(uint uiSize)
{
	NIASSERT(!m_pkScene);
	NIASSERT(!m_pkControllerManager);

	float fSize = (float)uiSize;
	float fWalkHeight = 4000.f;
	NxBounds3 nxBound;
	nxBound.setCenterExtents(NxVec3(fSize * 0.5f, fSize * 0.5f, 0.f), NxVec3(fSize * 0.5f, fSize * 0.5f, fWalkHeight *0.5f));

	NxSceneDesc nxSceneDesc;
	nxSceneDesc.gravity = NxVec3(0.f, 0.f, -9.8f);
	nxSceneDesc.simType = NX_SIMULATION_SW;
	nxSceneDesc.maxBounds = &nxBound;
	nxSceneDesc.upAxis = 2;							///< Z up
	nxSceneDesc.bpType = NX_BP_TYPE_SAP_MULTI;
	nxSceneDesc.nbGridCellsX = 8u;
	nxSceneDesc.nbGridCellsY = 8u;
	nxSceneDesc.subdivisionLevel = 5;

	m_pkScene = m_pkPhysicsSDK->createScene(nxSceneDesc);
	NIASSERT(m_pkScene);

	m_pkControllerManager = NxCreateControllerManager(m_pkAllocator);
	NIASSERT(m_pkControllerManager);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdPhysicsSystem::CreateTerrain(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);
	NIASSERT(m_pkHeightField == NULL)

	sdHeightMap* pkHeightMap = pkTerrain->GetHeightMap();
	NIASSERT(pkHeightMap);

	uint uiBlockSize = pkHeightMap->GetSize() + 1;	///< ÿ�߶�����(??Ϊʲô��һ??)
	uint uiNumVert = uiBlockSize * uiBlockSize;		///< �ܶ�����

	float fMinHeight = pkHeightMap->GetMinHeight();
	float fMaxHeight = pkHeightMap->GetMaxHeight();
	float fMidHeight = (fMaxHeight + fMinHeight) * 0.5f;
	float fHalfDeltaHeight = (fMaxHeight - fMinHeight) * 0.5f;
	float fThirtyTwoKb = 32766.f;
	fHalfDeltaHeight = fabs(fHalfDeltaHeight) < 0.0001f ? 0.0001f : fHalfDeltaHeight;	///< ��ֵֹ��С

	NxHeightFieldSample* pnxHeightFieldSample = NiAlloc2(NxHeightFieldSample, uiNumVert, NiMemHint::NONE);
	NIASSERT(pnxHeightFieldSample);

	// �����߶�������
	NxHeightFieldDesc nxHeightFieldDesc;
	nxHeightFieldDesc.nbColumns = uiBlockSize;	
	nxHeightFieldDesc.nbRows = uiBlockSize;
	nxHeightFieldDesc.convexEdgeThreshold = 0.f;
	nxHeightFieldDesc.thickness = -1000.f;
	nxHeightFieldDesc.samples = pnxHeightFieldSample;
	nxHeightFieldDesc.sampleStride = sizeof(NxU32);

	uchar* pucData = (uchar*)pnxHeightFieldSample;
	for (int i = 0; i < (int)uiBlockSize; ++i)
	{
		for (int j = 0; j < (int)uiBlockSize; ++j)
		{
			float fHeight = pkHeightMap->GetRawHeight(i, j);
			NxI16 nxHeight = (NxI16)(fThirtyTwoKb * ((fHeight - fMinHeight) / fHalfDeltaHeight));

			NxHeightFieldSample* pkHeightFieldSample = (NxHeightFieldSample*)(pucData);
			pkHeightFieldSample->height = nxHeight;
			pkHeightFieldSample->materialIndex0 = 1;
			pkHeightFieldSample->materialIndex1 = 1;
			pkHeightFieldSample->tessFlag = 1;

			pucData += nxHeightFieldDesc.sampleStride;
		}
	}

	// �����߶���
	m_pkHeightField = m_pkPhysicsSDK->createHeightField(nxHeightFieldDesc);
	NIASSERT(m_pkHeightField);

	//
	NiFree(pnxHeightFieldSample);
	pnxHeightFieldSample = NULL;

	// ������������
	NiPoint3 kWorldCenter(0, 0, fMidHeight);	///< ??Ϊʲô�����ֵ??
	NxVec3 nxWorldCenter;
	NiPhysXTypes::NiPoint3ToNxVec3(kWorldCenter, nxWorldCenter);

	NiMatrix3 kWorldRotation(NiPoint3(0.f, 1.f, 0.f), NiPoint3(0.f, 0.f, 1.f), NiPoint3(1.f, 0.f, 0.f));
	NxMat33 nxWorldRotation;
	NiPhysXTypes::NiMatrix3ToNxMat33(kWorldRotation, nxWorldRotation);

	NxMat34 kPose;
	kPose.t = nxWorldCenter;
	kPose.M = nxWorldRotation;

	NxHeightFieldShapeDesc nxHeightFieldShapeDesc;
	nxHeightFieldShapeDesc.heightField = m_pkHeightField;
	nxHeightFieldShapeDesc.heightScale = fHalfDeltaHeight / fThirtyTwoKb;
	nxHeightFieldShapeDesc.rowScale = 1.f;
	nxHeightFieldShapeDesc.columnScale = 1.f;
	nxHeightFieldShapeDesc.materialIndexHighBits = 0;
	nxHeightFieldShapeDesc.holeMaterial = 100;
	nxHeightFieldShapeDesc.meshFlags = NX_MESH_SMOOTH_SPHERE_COLLISIONS;
	nxHeightFieldShapeDesc.shapeFlags |= NX_SF_FEATURE_INDICES;
//	nxHeightFieldShapeDesc.shapeFlags |= NX_SF_VISUALIZATION;
	nxHeightFieldShapeDesc.shapeFlags &= ~NX_SF_VISUALIZATION;
	nxHeightFieldShapeDesc.group = E_COLLISION_GROUP_MASK_TERRAIN;
	nxHeightFieldShapeDesc.name = "PhysX_Terrain";

	NxActorDesc nxActorDesc;
	nxActorDesc.body = NULL;
	nxActorDesc.name = NULL;
	nxActorDesc.shapes.push_back(&nxHeightFieldShapeDesc);
	nxActorDesc.globalPose = kPose;
	nxActorDesc.group = E_COLLISION_GROUP_MASK_TERRAIN;

	// ��������
	NxActor* pkActor = m_pkScene->createActor(nxActorDesc);
	NIASSERT(pkActor);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdPhysicsSystem::CreateTerrainBound(sdTerrain* pkTerrain)
{
	NIASSERT(pkTerrain);

	float fTerrainSize = (float)pkTerrain->GetHeightMap()->GetSize();
	float fWallHeight = 1000.f;						///< д��

	//
	NxActorDesc nxActorDesc;

	//
	NxBoxShapeDesc nxBoxShapeDesc;
	nxBoxShapeDesc.dimensions.set(0.1f, fTerrainSize * 0.5f, fWallHeight);
	nxBoxShapeDesc.localPose.t.set(0.f, fTerrainSize * 0.5f, fWallHeight * 0.5f);
	nxBoxShapeDesc.shapeFlags &= ~NX_SF_VISUALIZATION;
	nxBoxShapeDesc.shapeFlags |= NX_SF_DISABLE_COLLISION;	///< ������������Χ�����ײ���
	nxBoxShapeDesc.group = E_COLLISION_GROUP_MASK_NOCAMERAHITTEST;
	nxActorDesc.shapes.push_back(&nxBoxShapeDesc);

	nxBoxShapeDesc.dimensions.set(fTerrainSize * 0.5f, 0.1f, fWallHeight);
	nxBoxShapeDesc.localPose.t.set(fTerrainSize * 0.5f, 0.f, fWallHeight * 0.5f);
	nxActorDesc.shapes.push_back(&nxBoxShapeDesc);

	nxBoxShapeDesc.dimensions.set(0.1f, fTerrainSize * 0.5f, fWallHeight);
	nxBoxShapeDesc.localPose.t.set(fTerrainSize, fTerrainSize * 0.5f, fWallHeight * 0.5f);
	nxActorDesc.shapes.push_back(&nxBoxShapeDesc);

	nxBoxShapeDesc.dimensions.set(fTerrainSize * 0.5f, 0.1f, fWallHeight);
	nxBoxShapeDesc.localPose.t.set(fTerrainSize * 0.5f, fTerrainSize, fWallHeight * 0.5f);
	nxActorDesc.shapes.push_back(&nxBoxShapeDesc);

	//
	while (!m_pkScene->isWritable())	///< ??Ϊ�ζ�γ���??
		m_pkScene->createActor(nxActorDesc);

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM