//*************************************************************************************************
// ����:	Cloud
//---------------------------------------------------------
// ����:		
// ����:		2012-11-06
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_CLOUD__
#define _SD_ENGINE_SCENE_CLOUD__
#include "sdEntity.h"

class sdCloud : public sdEntity
{
public:
	sdCloud();
	virtual ~sdCloud();

	//
	virtual void Render();

protected:
	// �����Ʋ�
	void CreateCloud();

protected:
	// ��
	NiMeshPtr	m_spMesh;

	// ����ɫ��
	NiMaterialPtr	m_spMaterial;

	// ������
	std::string	m_kCloudTexPath;
	NiTexturePtr m_spCloudTexture;

	// �Ʋ���
	// [cloud_wrap][cloud_speed][z_bias][alpha]
	float m_fCloudSpeed;
	float m_fCloudWrap;
	float m_fCloudDomeZBias;
	float m_fCloudDomeAlpha;
	NiFloatsExtraDataPtr m_spExtraData;
};
NiSmartPointer(sdCloud);
#endif