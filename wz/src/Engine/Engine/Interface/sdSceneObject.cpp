#include "sdEnginePCH.h"
#include "sdSceneObject.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------------
sdLightSceneObject::sdLightSceneObject()
	: sdSceneObject(E_SOT_LIGHT)
	, m_eLightType(E_LT_POINT)
	, m_fDimmer(1.f)
	, m_fAttenuRange(10.f)
	, m_fAttenuSoftness(1.f)
	, m_fSpotFovX(60.f)
	, m_fSpotFovY(60.f)
	, m_fSpotSoftness(1.f)
	, m_spFilterMap(NULL)
	, m_fAlphaRef(0.f)
{

}
//---------------------------------------------------------------------------------------------
sdProjectorSceneObject::sdProjectorSceneObject()
	: sdSceneObject(E_SOT_PROJECTOR)
	, m_eProjectType(E_PT_ORTHOGONAL)
	, m_eBlendType(E_BT_ADD)
	, m_bReceiveLighting(true)
	, m_bVisible(true)
	, m_uiLayer(0)
	, m_kPosition(NiPoint3::UNIT_Z)
	, m_kDirection(-NiPoint3::UNIT_Z)
	, m_kUp(NiPoint3::UNIT_Y)
	, m_kSize(NiPoint3::UNIT_ALL)
	, m_kUVScale(1.f, 1.f)
	, m_kUVOffset(NiPoint2::ZERO)
	, m_kUVSpeed(NiPoint2::ZERO)
	, m_fAlpha(1.f)
	, m_fAlphaTest(0.f)
	, m_kDiffuseMaterial(NiColor::WHITE)
	, m_kSpecularMaterial(NiColor::WHITE)
	, m_fShiness(16.f)
	, m_spDiffuseMap(NULL)
	, m_spNormalMap(NULL)
	, m_spFilterMap(NULL)
	, m_spMaterial(NULL)
{

}
//---------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE