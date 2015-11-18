#include "sdEnginePCH.h"
#include "sdDecal.h"

#include "IResourceSystem.h"

using namespace Engine;
using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;

NiImplementRTTI(sdDecal, sdEntity);
sdImplementDefaultStream(sdDecal);

IMPL_PROPFUNC_MAP(sdDecal, sdEntity)
IMPL_DYNAMIC(sdDecal, sdEntity)
//-------------------------------------------------------------------------------------------------
sdDecal::sdDecal()
: m_bDiffuseMapDirty(false)
, m_bNormalMapDirty(false)
, m_bFilterMapDirty(false)
{
	BEGIN_PROPFUNC_MAP(sdDecal)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdDecal, Width)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdDecal, Height)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdDecal, Length)

		REGISTER_PROPFUNC(PROP_NICOLOR, PROP_ATTR_DEFAULT, sdDecal, DiffuseMaterial)
		REGISTER_PROPFUNC(PROP_NICOLOR, PROP_ATTR_DEFAULT, sdDecal, SpecularMaterial)

		REGISTER_PROPFUNC(PROP_NIPT2, PROP_ATTR_DEFAULT, sdDecal, UVScale)
		REGISTER_PROPFUNC(PROP_NIPT2, PROP_ATTR_DEFAULT, sdDecal, UVOffset)
		REGISTER_PROPFUNC(PROP_NIPT2, PROP_ATTR_DEFAULT, sdDecal, UVSpeed)

		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT | PROP_ATTR_FILEPATH , sdDecal, DiffuseMap)
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT | PROP_ATTR_FILEPATH , sdDecal, NormalMap)
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT | PROP_ATTR_FILEPATH , sdDecal, FilterMap)

		REGISTER_PROPFUNC(PROP_BOOL, PROP_ATTR_DEFAULT, sdDecal, RecvLighting)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdDecal, Alpha)
		REGISTER_PROPFUNC(PROP_INT, PROP_ATTR_DEFAULT, sdDecal, BlendType)
	END_PROPFUNC_MAP(sdDecal)

	SetName("Decal");		///< 设置对象名称
	SetType(E_ET_DECAL);	///< 设置对象类型

	SetWidth(1.f);
	SetHeight(1.f);
	SetLength(10.f);

	SetDiffuseMaterial(NiColor::WHITE);
	SetSpecularMaterial(NiColor::WHITE);

	SetUVScale(NiPoint2(1.f, 1.f));
	SetUVOffset(NiPoint2::ZERO);
	SetUVSpeed(NiPoint2::ZERO);

	SetDiffuseMap("world0:texture\\misc\\decal\\self.dds");
	
	SetRecvLighting(false);
	SetAlpha(1.f);
	SetBlendType(2);
}
//-------------------------------------------------------------------------------------------------
sdDecal::~sdDecal()
{

}
//-------------------------------------------------------------------------------------------------
void sdDecal::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	__super::Cull(kCamera, kRenderPath);

	if (GetAppCulled())
		return;

	// 场景对象不存在则创建之
	if (!m_spPSOProjector)
	{
		m_spPSOProjector = NiNew sdProjectorSceneObject;
		NIASSERT(m_spPSOProjector);
	}

	// 从Entity拉数据到场景对象
	m_spPSOProjector->SetPosition(GetWorldTranslate());

	NiPoint3 kDirection;
	NiPoint3 kUp;
	m_kWorld.m_Rotate.GetCol(0, kDirection);
	m_kWorld.m_Rotate.GetCol(2, kUp);
	kDirection.Unitize();
	kUp.Unitize();

	m_spPSOProjector->SetDirection(kDirection);
	m_spPSOProjector->SetUpDirection(kUp);

	m_spPSOProjector->SetWidth(m_fWidth * GetWorldScale());
	m_spPSOProjector->SetHeight(m_fHeight * GetWorldScale());
	m_spPSOProjector->SetLength(m_fLength * GetWorldScale());

	m_spPSOProjector->SetDiffuseMaterial(m_kDiffuseMaterial);
	m_spPSOProjector->SetSpecularMaterial(m_kSpecularMaterial);
	m_spPSOProjector->SetShiness(10.f);

	m_spPSOProjector->SetUVScale(m_kUVScale);
	m_spPSOProjector->SetUVOffset(m_kUVOffset);
	m_spPSOProjector->SetUVSpeed(m_kUVSpeed);

	if (m_bDiffuseMapDirty)
	{
		if (m_kDiffuseMap.Exists())
		{
			IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
			NIASSERT(pkResourceSystem);

			m_spDiffuseMap = pkResourceSystem->SyncLoadTexture(m_kDiffuseMap);
		}
		else
		{
			m_spDiffuseMap = 0;
		}

		m_bDiffuseMapDirty = false;
	}

	m_spPSOProjector->SetDiffuseMap(m_spDiffuseMap);

	m_spPSOProjector->SetAlpha(m_fAlpha);
	m_spPSOProjector->SetReceiveLighting(m_bReceiveLighting);
	m_spPSOProjector->SetBlendType((sdProjectorSceneObject::EBlendType)m_iBlendType);

	// 裁剪(考虑改进)
	const NiPoint3& kCameraPosition = kCamera.GetWorldTranslate();
	const NiPoint3& kEntityPosition = GetWorldTranslate();
	float fViewDistance = GetViewDistance();
	float fSqrCameraEntityDistance = (kEntityPosition - kCameraPosition).SqrLength();
	if (fSqrCameraEntityDistance > fViewDistance * fViewDistance)
		return;

	// 加入RenderPath
	kRenderPath.RenderProjector(m_spPSOProjector);
}
//-------------------------------------------------------------------------------------------------
void sdDecal::UpdateWorldBound()
{
	float fWidth = m_fWidth * GetWorldScale();
	float fHeight = m_fHeight * GetWorldScale();
	float fLength = m_fLength * GetWorldScale();

	m_kWorldBound.SetCenter(GetWorldTranslate());
	m_kWorldBound.SetRadius(efd::Max(efd::Max(fWidth, fHeight), fWidth));
}
//-------------------------------------------------------------------------------------------------
void sdDecal::UpdateDownwardPass(NiUpdateProcess &kUpdate)
{
	__super::UpdateDownwardPass(kUpdate);
	UpdateWorldBound();
}
//-------------------------------------------------------------------------------------------------