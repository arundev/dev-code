//*************************************************************************************************
// ����:	�����߼�����
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_ACTOR_H__
#define _SD_ENGINE_SCENE_ACTOR_H__
#include "sdEntity.h"

//-------------------------------------------------------------------------------------------------
class sdActor : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdActor)

public:
	// ��Ӱ����(DecalShadow)
	enum EShadowType
	{
		E_ST_REALTIME_SHADOW,	///< ʵʱ��Ӱ
		E_ST_ROUND_SHADOW,		///< Բ����Ӱ
		E_ST_NO_SHADOW,			///< ����Ӱ
	};

public:
	sdActor();
	virtual ~sdActor();

	// �麯���̳�(�̳���sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// ������
	// @{
	

	// @}

protected:	
	// ������
	bool m_bEnableRimlight;

	// ��ӰЧ��
	bool m_bEnableShadow;
	EShadowType m_eShadowType;

	// ��������
	bool m_bOffScreenUpdate;			///< �Ƿ���������
	float m_fOffScreenUpdateDistance;	///< �������·�Χ

	//m_eSkeletonQuility��	///< Ӱ��ÿ����������������
};
NiSmartPointer(sdActor);
//-------------------------------------------------------------------------------------------------
#endif