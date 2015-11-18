#include "sdGameEditExPCH.h"
#include "sdTerrainDeformMode.h"
#include "sdEditBrush.h"

#include <sdRay.h>

#include <IRenderSystem.h>
#include <IRenderDevice.h>
#include <sdMap.h>
#include <sdTerrain.h>

#include <windef.h>

using namespace Base::Math;
using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
sdTerrainDeformMode::sdTerrainDeformMode()
: m_bLeftButtonDown(false)
{
	m_kMousePos.x = m_kMousePos.y = 0;
	SetStateID(E_EDITMODE_TERRAIN_DEFORM);
}
//-------------------------------------------------------------------------------------------------
sdTerrainDeformMode::~sdTerrainDeformMode()
{

}
//-------------------------------------------------------------------------------------------------
bool sdTerrainDeformMode::Initialize()
{
	__super::Initialize();

	// Mesh
	sdDualCircleMeshPtr pkDualCircleMesh = NiNew sdDualCircleMesh;
	NIASSERT(pkDualCircleMesh);
	pkDualCircleMesh->SetOuterRadius(10.0f);
	pkDualCircleMesh->SetInnerRadius(5.0f);

	// PullBrush
	sdDualCircleShapePtr pkDualCircleShape = NiNew sdDualCircleShape;
	NIASSERT(pkDualCircleShape);
	pkDualCircleShape->SetOuterRadius(10.f);
	pkDualCircleShape->SetInnerRadius(5.f);

	sdTerrainDeformPoolBrushPtr pkTerrainDeformPoolBrush = NiNew sdTerrainDeformPoolBrush;
	NIASSERT(pkTerrainDeformPoolBrush);
	pkTerrainDeformPoolBrush->SetBrushShape((sdDualCircleMesh*)pkDualCircleMesh);
	pkTerrainDeformPoolBrush->SetEditShape((sdDualCircleShape*)pkDualCircleShape);

	// SmoothBrush
	pkDualCircleShape = NiNew sdDualCircleShape;
	NIASSERT(pkDualCircleShape);
	pkDualCircleShape->SetOuterRadius(10.f);
	pkDualCircleShape->SetInnerRadius(5.f);

	sdTerrainDeformSmoothBrushPtr pkTerrainDeformSmoothBrush = NiNew sdTerrainDeformSmoothBrush;
	NIASSERT(pkTerrainDeformSmoothBrush);
	pkTerrainDeformSmoothBrush->SetBrushShape((sdDualCircleMesh*)pkDualCircleMesh);
	pkTerrainDeformSmoothBrush->SetEditShape((sdDualCircleShape*)pkDualCircleShape);

	// NoiseBrush
	pkDualCircleShape = NiNew sdDualCircleShape;
	NIASSERT(pkDualCircleShape);
	pkDualCircleShape->SetOuterRadius(10.f);
	pkDualCircleShape->SetInnerRadius(5.f);

	sdTerrainDeformNoiseBrushPtr pkTerrainDeformNoiseBrush = NiNew sdTerrainDeformNoiseBrush;
	NIASSERT(pkTerrainDeformNoiseBrush);
	pkTerrainDeformNoiseBrush->SetBrushShape((sdDualCircleMesh*)pkDualCircleMesh);
	pkTerrainDeformNoiseBrush->SetEditShape((sdDualCircleShape*)pkDualCircleShape);

	// FlatBrush
	pkDualCircleShape = NiNew sdDualCircleShape;
	NIASSERT(pkDualCircleShape);
	pkDualCircleShape->SetOuterRadius(10.f);
	pkDualCircleShape->SetInnerRadius(10.f);

	sdTerrainDeformFlatBrushPtr pkTerrainDeformFlatBrush = NiNew sdTerrainDeformFlatBrush;
	NIASSERT(pkTerrainDeformFlatBrush);
	pkTerrainDeformFlatBrush->SetBrushShape((sdDualCircleMesh*)pkDualCircleMesh);
	pkTerrainDeformFlatBrush->SetEditShape((sdDualCircleShape*)pkDualCircleShape);

	// AverageBrush
	pkDualCircleShape = NiNew sdDualCircleShape;
	NIASSERT(pkDualCircleShape);
	pkDualCircleShape->SetOuterRadius(10.f);
	pkDualCircleShape->SetInnerRadius(5.f);

	sdTerrainDeformAverageBrushPtr pkTerrainDeformAverageBrush = NiNew sdTerrainDeformAverageBrush;
	NIASSERT(pkTerrainDeformAverageBrush);
	pkTerrainDeformAverageBrush->SetBrushShape((sdDualCircleMesh*)pkDualCircleMesh);
	pkTerrainDeformAverageBrush->SetEditShape((sdDualCircleShape*)pkDualCircleShape);

	//
	m_pkkEditBrushArray[sdEditBrush::E_BRUSH_PULL]		= pkTerrainDeformPoolBrush;
	m_pkkEditBrushArray[sdEditBrush::E_BRUSH_SMOOTH]	= pkTerrainDeformSmoothBrush;
	m_pkkEditBrushArray[sdEditBrush::E_BRUSH_NOISE]		= pkTerrainDeformNoiseBrush;
	m_pkkEditBrushArray[sdEditBrush::E_BRUSH_FLAT]		= pkTerrainDeformFlatBrush;
	m_pkkEditBrushArray[sdEditBrush::E_BRUSH_AVERAGE]	= pkTerrainDeformAverageBrush;
	
	// 初始状态
	//SetActiveDeformBrush(sdEditBrush::E_BRUSH_PULL);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::Destroy()
{
	__super::Destroy();

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	// 销毁
	m_pkActiveBrush = 0;
	for (int i = 0; i < sdEditBrush::NUM_BRUSHES; ++i)
	{
		if (m_pkkEditBrushArray[i])
		{
			pkMap->GetDebugNode()->AttachChild(m_pkkEditBrushArray[i]->GetBrushShape());
			m_pkkEditBrushArray[i] = 0;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::Enter()
{
	__super::Enter();

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	//
	if (m_pkActiveBrush)
		pkMap->GetDebugNode()->AttachChild(m_pkActiveBrush->GetBrushShape());
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::Leave()
{
	__super::Leave();

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	//
	if (m_pkActiveBrush)
		pkMap->GetDebugNode()->DetachChild(m_pkActiveBrush->GetBrushShape());
}
//-------------------------------------------------------------------------------------------------
int sdTerrainDeformMode::Update()
{
	if (!m_pkActiveBrush)
		return __super::Update();;

	// 计算屏幕射线
	// @{
	// 获取屏幕尺寸
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	NiRenderTargetGroup* spRenderTargetGroup = pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(spRenderTargetGroup);

	uint uiWidth = spRenderTargetGroup->GetWidth(0);
	uint uiHeight = spRenderTargetGroup->GetHeight(0);

	// 获取相机
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiCamera* spCamera = pkRenderSystem->GetCamera();
	NIASSERT(spCamera);

	// 计算Pick射线
	NiPoint3 kOrigin;
	NiPoint3 kDir;
	NiViewMath::MouseToRay((float)m_kMousePos.x, (float)m_kMousePos.y, uiWidth, uiHeight, spCamera, kOrigin, kDir);
	
	sdRay kRay(sdVector3(kOrigin.x, kOrigin.y, kOrigin.z), sdVector3(kDir.x, kDir.y, kDir.z));
	// @}


	if (m_bLeftButtonDown)
	{
		// 左键按下
		//
		// 更新地形笔刷
		m_pkActiveBrush->Apply(kRay);
	}
	else
	{
		// 左键未按下
		//
		// 仅仅更新绘制笔刷
		sdMap* pkMap = pkRenderSystem->GetMap();
		NIASSERT(pkMap);

		sdTerrain* pkTerrain = pkMap->GetTerrain();
		NIASSERT(pkTerrain);

		sdBrushShape* pkBrushShape = m_pkActiveBrush->GetBrushShape();
		NIASSERT(pkBrushShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)m_pkActiveBrush->GetEditShape();
		NIASSERT(pkDualCircleShape);

		sdVector3 kIntersect;
		if (pkTerrain->Pick(kRay, kIntersect))	
		{
			pkDualCircleShape->SetCenter(kIntersect.m_fX, kIntersect.m_fY);
			pkBrushShape->SetTranslate(kIntersect.m_fX, kIntersect.m_fY, 0);
		}

		pkBrushShape->Update(0.0f);
		pkBrushShape->UpdateShape();
	}

	return __super::Update();
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	__super::OnLeftButtonDown(wParam,lParam);

	// 提取鼠标屏幕位置
	m_kMousePos.x = (int)(short)LOWORD(lParam);
	m_kMousePos.y = (int)(short)HIWORD(lParam);

	//
	m_bLeftButtonDown = true;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::OnLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	__super::OnLeftButtonUp(wParam, lParam);
	m_bLeftButtonDown = false;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	__super::OnMouseMove(wParam, lParam);

	// 提取鼠标屏幕位置
	m_kMousePos.x = (int)(short)LOWORD(lParam);
	m_kMousePos.y = (int)(short)HIWORD(lParam);
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDeformMode::SetActiveDeformBrush(sdEditBrush::EBrushType eType)
{
	NIASSERT(eType > sdEditBrush::E_BRUSH_INVALID);
	NIASSERT(eType < sdEditBrush::E_BRUSH_LAYER);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	sdMap* pkMap = pkRenderSystem->GetMap();
	NIASSERT(pkMap);

	//
	if (m_pkActiveBrush)
	{
		if (m_pkActiveBrush->GetType() == eType)
			return;

		// 移除旧笔刷
		pkMap->GetDebugNode()->DetachChild(m_pkActiveBrush->GetBrushShape());
	}

	// 挂接新笔刷
	m_pkActiveBrush = m_pkkEditBrushArray[eType];
	pkMap->GetDebugNode()->AttachChild(m_pkActiveBrush->GetBrushShape());

	// 设置新笔刷状态
	switch(eType)
	{
	case sdEditBrush::E_BRUSH_PULL:
	case sdEditBrush::E_BRUSH_SMOOTH:
	case sdEditBrush::E_BRUSH_NOISE:
	case sdEditBrush::E_BRUSH_AVERAGE:
		{
			sdDualCircleShape* pkDualCircleShape =  (sdDualCircleShape*)m_pkActiveBrush->GetEditShape();
			NIASSERT(pkDualCircleShape);
			float fOutterRadius = pkDualCircleShape->GetOuterRadius();
			float fInnerRadius = pkDualCircleShape->GetInnerRadius();

			sdDualCircleMesh* pkDualCirlceMesh = (sdDualCircleMesh*)m_pkActiveBrush->GetBrushShape();
			NIASSERT(pkDualCirlceMesh);
			pkDualCirlceMesh->SetOuterRadius(fOutterRadius);
			pkDualCirlceMesh->SetInnerRadius(fInnerRadius);
			pkDualCirlceMesh->SetInnerCircleVisible(true);
			pkDualCirlceMesh->SetOuterCircleVisible(true);

			break;
		}
	case sdEditBrush::E_BRUSH_FLAT:
		{
			sdDualCircleShape* pkDualCircleShape =  (sdDualCircleShape*)m_pkActiveBrush->GetEditShape();
			NIASSERT(pkDualCircleShape);
			float fOutterRadius = pkDualCircleShape->GetOuterRadius();
			float fInnerRadius = pkDualCircleShape->GetInnerRadius();

			sdDualCircleMesh* pkDualCirlceMesh = (sdDualCircleMesh*)m_pkActiveBrush->GetBrushShape();
			NIASSERT(pkDualCirlceMesh);
			pkDualCirlceMesh->SetOuterRadius(fOutterRadius);
			pkDualCirlceMesh->SetInnerRadius(fOutterRadius);
			pkDualCirlceMesh->SetInnerCircleVisible(false);
			pkDualCirlceMesh->SetOuterCircleVisible(true);

			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX