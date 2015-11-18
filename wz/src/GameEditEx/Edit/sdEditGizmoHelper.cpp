#include "sdGameEditExPCH.h"
#include "sdEditGizmoHelper.h"

#include <IResourceSystem.h>
#include <IRenderSystem.h>
#include <sdWorld.h>
#include <sdRenderHelper.h>

#include <sdCameraFSM.h>

#define SELECTION_GIZMO_FILE_NAME		"dependence\\editor\\select.nif"

using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
using namespace GameCore;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
NiNodePtr sdEditGizmoHelper::LoadGizmoNif(const char* szFileName)
{
	NIASSERT(szFileName);

	char szAbsFileName[MAX_PATH_LEN];
	sdWorldManager::Instance().Convert2ResPath(szFileName, szAbsFileName);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	NiNodePtr spNode = pkResourceSystem->SyncLoadNif(szAbsFileName);
	if (spNode)
	{
		spNode->UpdateProperties();
		spNode->UpdateEffects();
		spNode->UpdateNodeBound();
		spNode->Update(0.f);	

		DisableZTest(spNode);


		NiMesh::CompleteSceneModifiers(spNode);
	}

	return spNode;
}
//-------------------------------------------------------------------------------------------------
void sdEditGizmoHelper::DisableZTest(NiAVObject* pkAVObject)
{
	NIASSERT(pkAVObject);

	NiProperty* pkProperty = pkAVObject->GetProperty(NiZBufferProperty::GetType());
	if (pkProperty)
	{
		NiZBufferProperty* pkZBufferProperty = (NiZBufferProperty*)(pkProperty);
		NIASSERT(pkZBufferProperty);

		pkZBufferProperty->SetZBufferTest(false);
		pkZBufferProperty->SetZBufferWrite(true);
	}

	if (pkAVObject->IsNode())
	{
		NiNode* pkNode = (NiNode*)pkAVObject;
		NIASSERT(pkNode);

		for (uint ui = 0; ui < pkNode->GetArrayCount(); ++ui)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
				DisableZTest(pkChild);
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditGizmoHelper::EnableDrawBothMode(NiAVObject* pkAVObject)
{
	NIASSERT(pkAVObject);

	NiProperty* pkProperty = pkAVObject->GetProperty(NiZBufferProperty::GetType());
	if (pkProperty)
	{
		NiStencilProperty* pkStencilProperty = (NiStencilProperty*)(pkProperty);
		NIASSERT(pkStencilProperty);
		pkStencilProperty->SetDrawMode(NiStencilProperty::DRAW_BOTH);
	}

	if (pkAVObject->IsNode())
	{
		NiNode* pkNode = (NiNode*)pkAVObject;
		NIASSERT(pkNode);

		for (uint ui = 0; ui < pkNode->GetArrayCount(); ++ui)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
				EnableDrawBothMode(pkChild);
		}
	}
}
//-------------------------------------------------------------------------------------------------
sdEditGizmoHelper::sdEditGizmoHelper()
{
	//  Screen Space   
	// 	     0.f
	//		  |
	//  0.f-------1.f
	//	      |
	//	     1.f
	m_spOrthScreenCamera = NiNew NiCamera;
	NIASSERT(m_spOrthScreenCamera);
	m_spOrthScreenCamera->SetViewFrustum(NiFrustum(0.f, 1.f, 0.f, 1.f, 3.f, 10.f, true));
	m_spOrthScreenCamera->SetMinNearPlaneDist(3.f);
	m_spOrthScreenCamera->SetViewPort(NiRect<float>(0, 1.f, 1.f, 0));
	m_spOrthScreenCamera->Update(0.f);
	m_spOrthScreenCamera->UpdateProperties();

	//    Screen Space
	//	      1.f
	// 		   |
	//  -1.f-------1.f
	//	       |
	//	     -1.f
	m_spOrthCamera = NiNew NiCamera;
	NIASSERT(m_spOrthCamera);
	m_spOrthCamera->SetViewFrustum(NiFrustum(-1.f, 1.f, 1.f, -1.f, 0.f, 10.f, true));
	m_spOrthCamera->SetMinNearPlaneDist(0.f);
	m_spOrthCamera->AdjustAspectRatio(1.f);
	m_spOrthCamera->SetViewPort(NiRect<float>(0, 0.2f, 0.2f, 0));	///< 绘制到左下角
	m_spOrthCamera->Update(0.f);
	m_spOrthCamera->UpdateProperties();

	// 加载坐标系显示
	m_spAxisObject = LoadGizmoNif(SELECTION_GIZMO_FILE_NAME);
	NIASSERT(m_spAxisObject);
}
//-------------------------------------------------------------------------------------------------
sdEditGizmoHelper::~sdEditGizmoHelper()
{
	
}
//-------------------------------------------------------------------------------------------------
void sdEditGizmoHelper::Update()
{

}
//-------------------------------------------------------------------------------------------------
void sdEditGizmoHelper::RenderGizmo()
{
	NiCamera* pkMainCamera = sdCameraFSM::Instance().GetCamera();
	NIASSERT(pkMainCamera);

	// 设定正交观察相机
	const NiPoint3& kWorldDirection = pkMainCamera->GetWorldDirection();
	const NiMatrix3& kWorldRotate = pkMainCamera->GetWorldRotate();
	m_spOrthCamera->SetRotate(kWorldRotate);				///< 观察相机旋转到与主相机相同的姿态
	m_spOrthCamera->SetTranslate(kWorldDirection * -1.0f);	///《观察相机后退1个单位
	m_spOrthCamera->Update(0.f);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	// 绘制
	pkRenderSystem->BeginDirectDraw(m_spOrthCamera);
	pkRenderSystem->DirectDraw(m_spAxisObject);
	pkRenderSystem->EndDirectDraw();
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX