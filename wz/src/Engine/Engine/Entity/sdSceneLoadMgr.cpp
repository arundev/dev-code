#include "sdEnginePCH.h"
#include "sdSceneLoadMgr.h"
#include "IRenderSystem.h"
#include "sdBuilding.h"

using namespace Engine::RenderSystem;
//-------------------------------------------------------------------------------------------------
sdSceneLoadMgr::sdSceneLoadMgr()
: m_eLoadMode(E_SCENE_LOAD_ASYNC)
, m_fMinLoadRadius(100.f)
{
	
}
//-------------------------------------------------------------------------------------------------
sdSceneLoadMgr::~sdSceneLoadMgr()
{

}
//-------------------------------------------------------------------------------------------------
void sdSceneLoadMgr::AddEntity(sdEntity* pkEntity)
{
	NIASSERT(pkEntity);

	// 目前只处理sdBuilding
	if (pkEntity->GetType() == E_ET_BUILDING)
	{
		// 加入对象列表
		m_kEntityVec.push_back(pkEntity);

		// 检测是否加入加载队列
		IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
		NIASSERT(pkRenderSystem);

		NiCamera* spCamera = pkRenderSystem->GetCamera();
		NIASSERT(spCamera);

		const NiPoint3& kCamPos = spCamera->GetWorldTranslate();
		const NiPoint3& kEntityPos = pkEntity->GetWorldTranslate();
		float fSqlDistance2Camera = (kEntityPos - kCamPos).SqrLength();
		float fViewDistance = pkEntity->GetViewDistance() + m_fMinLoadRadius;
		float fViewSqlDistance = fViewDistance * fViewDistance;

		if (fSqlDistance2Camera < fViewSqlDistance)
			m_kProcessVec.push_back(pkEntity);
	}
}
//-------------------------------------------------------------------------------------------------
void sdSceneLoadMgr::RemoveEntity(sdEntity* pkEntity)
{
	NIASSERT(pkEntity);

	// 目前只处理sdBuilding
	if (pkEntity->GetType() == E_ET_BUILDING)
	{
		EntityVecItr itr = std::find(m_kEntityVec.begin(), m_kEntityVec.end(), pkEntity);
		if (itr != m_kEntityVec.end())
			m_kEntityVec.erase(itr);

		itr = std::find(m_kProcessVec.begin(), m_kProcessVec.end(), pkEntity);
		if (itr != m_kProcessVec.end())
			m_kProcessVec.erase(itr);

		itr = std::find(m_kDelayUnloadingVec.begin(), m_kDelayUnloadingVec.end(), pkEntity);
		if (itr != m_kDelayUnloadingVec.end())
			m_kDelayUnloadingVec.erase(itr);
	}
}
//-------------------------------------------------------------------------------------------------
void sdSceneLoadMgr::Update()
{
	if (m_eLoadMode == E_SCENE_LOAD_SYNC)
		SyncLoad();
	else if (m_eLoadMode == E_SCENE_LOAD_ASYNC)
		AsyncLoad();
}
//-------------------------------------------------------------------------------------------------
void sdSceneLoadMgr::SyncLoad()
{

}
//-------------------------------------------------------------------------------------------------
void sdSceneLoadMgr::AsyncLoad()
{
	//sdPF_Func_Auto;
	
	//sdTimeOvertimeTrack(__FUNCTION__,50.f);

	bool bTimeOver = false;
	float fMaxTime = 20.f;			///< 最大处理时间为20ms
	m_kProfileOutput.Begin();

	// 优先处理上一阵未完成的Unload队列
	uint uiCount = 0;
	EntityVecRItr itr_rbegin = m_kDelayUnloadingVec.rbegin();
	EntityVecRItr itr_rend = m_kDelayUnloadingVec.rend();
	for (; itr_rbegin != itr_rend;)
	{
		sdEntity* pkEntity = (*itr_rbegin++);
		m_kDelayUnloadingVec.pop_back();

		sdBuilding* pkBuilding = (sdBuilding*)pkEntity;
		pkBuilding->UpdateUnload();

		++uiCount;
		if (((uiCount % 100) == 0) && m_kProfileOutput.IsTimeOver(fMaxTime))
		{
			bTimeOver = true;
			break;
		}
	}

	if (bTimeOver)
		return;

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiCamera* spCamera = pkRenderSystem->GetCamera();
	NIASSERT(spCamera);

	// 处理加载卸载队列
	const NiPoint3& kCamPos = spCamera->GetWorldTranslate();
	float fMinSqlLoadRadius = m_fMinLoadRadius * m_fMinLoadRadius;
	if ((m_kLastLoadPos - kCamPos).Length() > fMinSqlLoadRadius)
	{
		m_kLastLoadPos = kCamPos;

		// 处理任务队列,卸载满足条件的对象
		uiCount = 0;
		EntityVecItr itr = m_kProcessVec.begin();
		EntityVecItr itr_end = m_kProcessVec.end();
		{
			sdEntity* pkEntity = *itr;

			const NiPoint3& kCamPos = spCamera->GetWorldTranslate();
			const NiPoint3& kEntityPos = pkEntity->GetWorldTranslate();
			float fSqlDistance2Camera = (kEntityPos - kCamPos).SqrLength();
			float fViewDistance = pkEntity->GetViewDistance() + m_fMinLoadRadius;
			float fViewSqlDistance = fViewDistance * fViewDistance;

			if (fSqlDistance2Camera >= fViewSqlDistance)
			{
				if (bTimeOver)
				{
					m_kDelayUnloadingVec.push_back(pkEntity);
				}
				else
				{
					++uiCount;
					if (((uiCount % 20) == 0) && m_kProfileOutput.IsTimeOver(fMaxTime))
					{
						m_kDelayUnloadingVec.push_back(pkEntity);
						bTimeOver = true;
					}
					else
					{
						sdBuilding* pkBuilding = (sdBuilding*)pkEntity;
						pkBuilding->UpdateUnload();
					}
				}
			}
		}
		m_kProcessVec.clear();
		
		// 从对象队列取出满足加载条件的对象,到加载队列
		itr = m_kEntityVec.begin();
		itr_end = m_kEntityVec.end();
		for (; itr != itr_end; ++itr)
		{
			sdEntity* pkEntity = *itr;

			const NiPoint3& kCamPos = spCamera->GetWorldTranslate();
			const NiPoint3& kEntityPos = pkEntity->GetWorldTranslate();
			float fSqlDistance2Camera = (kEntityPos - kCamPos).SqrLength();
			float fViewDistance = pkEntity->GetViewDistance() + m_fMinLoadRadius;
			float fViewSqlDistance = fViewDistance * fViewDistance;

			if (fSqlDistance2Camera < fViewSqlDistance)
				m_kProcessVec.push_back(pkEntity);
		}
	}

	// 处理任务队列,当前帧没有处理完的放到队列头下一帧继续处理
	uiCount = 0;
	EntityVecItr itr = m_kProcessVec.begin();
	EntityVecItr itr_end = m_kProcessVec.end();
	for (; itr != itr_end; ++itr)
	{
		sdEntity* pkEntity = *itr;
		sdBuilding* pkBuilding = (sdBuilding*)pkEntity;

		const NiPoint3& kCamPos = spCamera->GetWorldTranslate();
		const NiPoint3& kEntityPos = pkEntity->GetWorldTranslate();
		float fSqlDistance2Camera = (kEntityPos - kCamPos).SqrLength();
		float fViewDistance = pkEntity->GetViewDistance() + m_fMinLoadRadius;
		float fViewSqlDistance = fViewDistance * fViewDistance;

		if (fSqlDistance2Camera < fViewSqlDistance)
		{
			pkBuilding->UpdateLoad();
		}
		else
		{
			pkBuilding->UpdateDelayUnload();
		}

		++uiCount;
		if (((uiCount % 100) == 0) && m_kProfileOutput.IsTimeOver(fMaxTime))
		{
			bTimeOver = true;
			break;
		}
	}

	uint uiLastCount = m_kProcessVec.size() - uiCount;
	if (uiLastCount)
	{
		EntityVec kTemp(uiLastCount, 0);
		std::copy(++itr, itr_end, kTemp.begin());

		m_kProcessVec.resize(uiLastCount, 0);
		std::copy(kTemp.begin(), kTemp.end(), m_kProcessVec.begin());
		kTemp.clear();
	}
}
//-------------------------------------------------------------------------------------------------