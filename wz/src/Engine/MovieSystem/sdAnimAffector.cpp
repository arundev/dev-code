#include "sdMovieSystemPCH.h"
#include "sdAnimAffector.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
// sdAnimAffector
//-------------------------------------------------------------------------------------------------
std::string sdAnimAffector::ms_kEffectTypeStringTable []= 
{
	"E_ANIM_AFFECTOR_DEFAULT",
	"E_ANIM_AFFECTOR_POSITION",
	"E_ANIM_AFFECTOR_ROTATION",
	"E_ANIM_AFFECTOR_SCALE",
	"E_ANIM_AFFECTOR_VISIBLE",
	"E_ANIM_AFFECTOR_ACTION",
	"E_ANIM_AFFECTOR_PICK",
	"E_ANIM_AFFECTOR_SUBTITLE",
	"E_ANIM_AFFECTOR_CAMERA",
	"E_ANIM_AFFECTOR_FADE",
	"E_ANIM_AFFECTOR_ENABLE",
	"E_ANIM_AFFECTOR_DOF",
	"E_ANIM_AFFECTOR_CLONE",
	"E_ANIM_AFFECTOR_OFFSET",
	"E_ANIM_AFFECTOR_LOOKAT",
	"E_ANIM_AFFECTOR_ALPHA",
	"E_ANIM_AFFECTOR_ATTACH",
	"E_ANIM_AFFECTOR_BLENDACTION",
	"E_ANIM_AFFECTOR_FLYSPEED",
	"E_ANIM_AFFECTOR_PLAYSPEED",
	"E_ANIM_AFFECTOR_COLOR",
	"E_ANIM_AFFECTOR_SPEFFECT",
	"E_ANIM_AFFECTOR_SPEFFECT_GHOST",
	"E_ANIM_AFFECTOR_DECAL_SCALE",
	"E_ANIM_AFFECTOR_DECAL_ROTATION",
	"E_ANIM_AFFECTOR_VOLUME",
};
//-------------------------------------------------------------------------------------------------
const std::string& sdAnimAffector::GetStringFromType(EAnimAffectorType eType)
{
	return ms_kEffectTypeStringTable[eType];
}
//-------------------------------------------------------------------------------------------------
EAnimAffectorType sdAnimAffector::GetTypeFromString(const std::string &strType)
{
	for (int i = 0; i < NUM_ANIM_AFFECTOR; ++i)
	{
		if (!stricmp(strType.c_str(), ms_kEffectTypeStringTable[i].c_str()))
			return (EAnimAffectorType)i;
	}

	return E_ANIM_AFFECTOR_DEFAULT;
}
//-------------------------------------------------------------------------------------------------
IAnimAffector* sdAnimAffector::CreateAffector(EAnimAffectorType eType)
{
	IAnimAffector* pkAnimAffector = NULL;

	return pkAnimAffector;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimAffector::IsChangingKey(IAnimTrack* pkAnimTrack, float fStartTime, float fEndTime)
{
	if (!pkAnimTrack || !pkAnimTrack->GetNumKeys())
		return false;

	if (fStartTime >= fEndTime)
		return true;

	// TODO:考虑使用二分查找改进
	for (int i = 0; i< pkAnimTrack->GetNumKeys(); ++i)
	{
		float fTime = pkAnimTrack->GetKeyTime(i);
		if (fStartTime < fTime && fEndTime >= fTime)
			return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
sdAnimAffector::sdAnimAffector()
: m_fStartTime(0.f)
, m_fEndTime(0.f)
, m_bLastKeyUpdated(false)
{

}
//-------------------------------------------------------------------------------------------------
sdAnimAffector::~sdAnimAffector()
{
	m_pkAnimTrack = 0;
	m_pkAnimAffectorControl = 0;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimAffector::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimAffector::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{


	return true;
}
//-------------------------------------------------------------------------------------------------
void sdAnimAffector::Reset()
{
	m_fStartTime = 0.f;
	m_fEndTime = 0.f;

	if (m_pkAnimAffectorControl)
	{
		for (int i = 0; i < m_pkAnimAffectorControl->GetAffectorNum(); ++i)
		{
			IAnimAffector* pkAnimAffector = m_pkAnimAffectorControl->GetAffectorByIndex(i);
			pkAnimAffector->Reset();
		
			IAnimTrack* pkAnimTrack = pkAnimAffector->GetAnimTrack();
			if (pkAnimTrack && pkAnimTrack->GetNumKeys())
			{
				int iKeyNum = pkAnimTrack->GetNumKeys();
				float fStartTime = pkAnimTrack->GetKeyTime(0);
				float fEndTime = pkAnimTrack->GetKeyTime(iKeyNum - 1);
				m_fStartTime = std::min(m_fStartTime, fStartTime);
				m_fEndTime = std::max(m_fEndTime, fEndTime);
			}
		}
	}

	if (m_pkAnimTrack && m_pkAnimTrack->GetNumKeys())
	{
		int iKeyNum = m_pkAnimTrack->GetNumKeys();
		float fStartTime = m_pkAnimTrack->GetKeyTime(0);
		float fEndTime = m_pkAnimTrack->GetKeyTime(iKeyNum - 1);
		m_fStartTime = std::min(m_fStartTime, fStartTime);
		m_fEndTime = std::max(m_fEndTime, fEndTime);
	}
}

//-------------------------------------------------------------------------------------------------
// sdAnimPositionAffector
//-------------------------------------------------------------------------------------------------
sdAnimPositionAffector::sdAnimPositionAffector()
{
	//m_pkAnimTrack = 
}
//-------------------------------------------------------------------------------------------------
void sdAnimPositionAffector::UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode)
{
	if (!pkAnimNode || !m_pkAnimTrack || !m_pkAnimTrack->GetNumKeys())
		return;

	// 检查是否已经播放完毕
	if (fAccumTime > m_fEndTime)
	{
		if (m_bLastKeyUpdated)
			return;
		else
			m_bLastKeyUpdated = true;
	}

	// 提取插值结果
	NiPoint3 kPosition;
	m_pkAnimTrack->GetValue(fAccumTime, kPosition);

	// 子节点
	if (m_pkAnimAffectorControl)
	{
		for (int i = 0; i < m_pkAnimAffectorControl->GetAffectorNum(); ++i)
		{
			IAnimAffector* pkAnimAffector = m_pkAnimAffectorControl->GetAffectorByIndex(i);
			if (!pkAnimAffector) 
				continue;

			IAnimTrack* pkAnimTrack = pkAnimAffector->GetAnimTrack();
			if (!pkAnimTrack || !pkAnimTrack->GetNumKeys())
				continue;

			//pkAnimAffector->UpdateFrame(fAccumTime, fFrameTime, kPosition);
		}
	}

	// 设置给节点
	// pkAnimNode->SetPosition(kPosition);
	//((sdAnimNode*)pkAnimNode)->SetValue(&sdANimNode::InitPosProperty, kPosition);
}

//-------------------------------------------------------------------------------------------------
// sdAnimRotationAffector
//-------------------------------------------------------------------------------------------------
sdAnimRotationAffector::sdAnimRotationAffector()
{
	//m_pkAnimTrack = 
}
//-------------------------------------------------------------------------------------------------
void sdAnimRotationAffector::UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode)
{
	if (!pkAnimNode || !m_pkAnimTrack || !m_pkAnimTrack->GetNumKeys())
		return;

	// 检查是否已经播放完毕
	if (fAccumTime > m_fEndTime)
	{
		if (m_bLastKeyUpdated)
			return;
		else
			m_bLastKeyUpdated = true;
	}

	NiQuaternion kRotation;
	m_pkAnimTrack->GetValue(fAccumTime, kRotation);
	kRotation.Normalize();

	// 设置给节点
	//((sdAnimNode*)pkAnimNode)->SetValue(&sdANimNode::InitPosProperty, kPosition);
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM