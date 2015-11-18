#include "sdEnginePCH.h"
#include "sdLight.h"

using namespace Engine;
using namespace Engine::RenderSystem;

NiImplementRTTI(sdLight, sdEntity);
sdImplementDefaultStream(sdLight);

IMPL_PROPFUNC_MAP(sdLight, sdEntity)
IMPL_DYNAMIC(sdLight, sdEntity)
//-------------------------------------------------------------------------------------------------
sdLight::sdLight()
{
	BEGIN_PROPFUNC_MAP(sdLight)
		REGISTER_PROPFUNC(PROP_UINT, PROP_ATTR_DEFAULT, sdLight, LightType)
		
		REGISTER_PROPFUNC(PROP_NICOLOR, PROP_ATTR_DEFAULT, sdLight, AmbientColor)
		REGISTER_PROPFUNC(PROP_NICOLOR, PROP_ATTR_DEFAULT, sdLight, DiffuseColor)
		REGISTER_PROPFUNC(PROP_NICOLOR, PROP_ATTR_DEFAULT, sdLight, SpecularColor)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, Dimmer)

		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, AttenuRange)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, AttenuSoftness)

		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, SpotFovX)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, SpotFovY)
		REGISTER_PROPFUNC(PROP_FLOAT, PROP_ATTR_DEFAULT, sdLight, SpotSoftness)
	END_PROPFUNC_MAP(sdLight)

	SetName("Light");			///< ���ö�������
	SetType(E_ET_LIGHT);		///< ���ö�������
	SetLightType(E_LT_UNKNOWN);	///< ����Light��������
}
//-------------------------------------------------------------------------------------------------
sdLight::~sdLight()
{

}
//-------------------------------------------------------------------------------------------------
void sdLight::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	__super::Cull(kCamera, kRenderPath);

	if (GetAppCulled())
		return;
	
	if (m_eLightType == E_LT_POINT || m_eLightType == E_LT_SPOT || m_eLightType == E_LT_RECTSPOT)
	{
		// �������󲻴����򴴽�֮
		if (!m_spLSOLight)
		{
			m_spLSOLight = NiNew sdLightSceneObject;
			NIASSERT(m_spLSOLight);
		}

		// ��Entity�����ݵ���������
		m_spLSOLight->SetLightType((sdLightSceneObject::ELightType)m_eLightType);

		m_spLSOLight->SetPosition(GetWorldTranslate());
		m_spLSOLight->SetRotate(GetWorldRotate());

		m_spLSOLight->SetAmbientColor(m_kAmbientColor);
		m_spLSOLight->SetDiffuseColor(m_kDiffuseColor);
		m_spLSOLight->SetSpecularColor(m_kSpecularColor);
		m_spLSOLight->SetDimmer(m_fDimmer);

		if (m_eLightType == E_LT_POINT || m_eLightType == E_LT_SPOT)
		{
			m_spLSOLight->SetAttenuRange(m_fAttenuRange);
			m_spLSOLight->SetAttenuSoftness(m_fAttenuSoftness);
		}

		if (m_eLightType == E_LT_SPOT)
		{
			SetSpotFovX(m_fSpotFovX);
			SetSpotFovY(m_fSpotFovY);
			SetSpotSoftness(m_fSpotSoftness);
		}

		// �ü�(���ǸĽ�)
		const NiPoint3& kCameraPosition = kCamera.GetWorldTranslate();
		const NiPoint3& kEntityPosition = GetWorldTranslate();
		float fViewDistance = GetViewDistance();
		float fSqrCameraEntityDistance = (kEntityPosition - kCameraPosition).SqrLength();
		if (fSqrCameraEntityDistance > fViewDistance * fViewDistance)
			return;

		// ����RenderPath
		kRenderPath.RenderLight(m_spLSOLight);
	}
}
//-------------------------------------------------------------------------------------------------
void sdLight::SetLightType(const uint& uiLightType)
{
	if (m_eLightType == (ELightType)uiLightType)
		return;

	m_eLightType = (ELightType)uiLightType;	///< ���ö�������

	SetTranslate(NiPoint3::ZERO);
	SetRotate(NiMatrix3::IDENTITY);

	SetAmbientColor(NiColor::WHITE);
	SetDiffuseColor(NiColor::WHITE);
	SetSpecularColor(NiColor::WHITE);
	SetDimmer(1.f);

	if (m_eLightType == E_LT_POINT || m_eLightType == E_LT_SPOT)
	{
		SetAttenuRange(10.f);
		SetAttenuSoftness(0.f);
	}

	if (m_eLightType == E_LT_SPOT)
	{
		SetSpotFovX(60.f);
		SetSpotFovY(60.f);
		SetSpotSoftness(0.f);
	}

//	// ��ȡGB�ƹ�Ӱ��Ľڵ㣬��ɾ���ɵ�GB�ƹ����
//	NiTPrimitiveSet<NiNode*> kAffectedNodeSet;
//	if (m_spLight)
//	{
//		const NiNodeList& kAffectedNodeList = m_spLight->GetAffectedNodeList();
//		NiTListIterator kIter = kAffectedNodeList.GetHeadPos();
//		while (kIter)
//			kAffectedNodeSet.Add(kAffectedNodeList.GetNext(kIter));
//
//		m_spLight->DetachAllAffectedNodes();
//		for (uint ui = 0; ui < kAffectedNodeSet.GetSize(); ++ui)
//			kAffectedNodeSet.GetAt(ui)->UpdateEffects();
//
//		DetachChild(m_spLight);
//		m_spLight = 0;
//	}
//
//	// �����µ�GB�ƹ����
//	m_eLightType = (ELightType)uiLightType;
//	switch (uiLightType)
//	{
//	case E_LT_AMBIENT:
//			{
//				m_spLight = NiNew NiAmbientLight;
//				NIASSERT(m_spLight);
//				break;
//			}
//
//		case E_LT_POINT:
//			{
//				break;
//			}
//
//		case E_LT_DIR:
//			{
//				m_spLight = NiNew NiDirectionalLight;
//				NIASSERT(m_spLight);
//				m_spLight->SetAmbientColor(NiColor::BLACK);
//				break;
//			}	
//	}
//
//	// ������ӵƹ�Ӱ��ڵ�,���ҽӵ���������
//	if (m_spLight)
//	{
//		for (uint ui = 0; ui < kAffectedNodeSet.GetSize(); ++ui)
//		{
//			NiNode* pkNode = kAffectedNodeSet.GetAt(ui);
//			m_spLight->AttachAffectedNode(pkNode);
//;			pkNode->UpdateEffects();
//		}
//
//		AttachChild(m_spLight);
//	}
//
//	// ����
//	Update(0.f);
}
//-------------------------------------------------------------------------------------------------
void sdLight::UpdateWorldBound()
{
	switch(m_eLightType)
	{
	case E_LT_POINT:
	case E_LT_SPOT:
		{
			m_kWorldBound.SetCenter(GetWorldTranslate());
			m_kWorldBound.SetRadius(m_fAttenuRange);
			break;
		}

	case E_LT_AMBIENT:
	case E_LT_DIR:
		{
			m_kWorldBound.SetCenter(NiPoint3::ZERO);
			m_kWorldBound.SetRadius(FLT_MAX);
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdLight::UpdateDownwardPass(NiUpdateProcess &kUpdate)
{
	__super::UpdateDownwardPass(kUpdate);
	UpdateWorldBound();
}
//-------------------------------------------------------------------------------------------------