//*************************************************************************************************
// 内容:	Cloud
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-06
// 最后修改:
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
	// 创建云层
	void CreateCloud();

protected:
	// 云
	NiMeshPtr	m_spMesh;

	// 云着色器
	NiMaterialPtr	m_spMaterial;

	// 云纹理
	std::string	m_kCloudTexPath;
	NiTexturePtr m_spCloudTexture;

	// 云参数
	// [cloud_wrap][cloud_speed][z_bias][alpha]
	float m_fCloudSpeed;
	float m_fCloudWrap;
	float m_fCloudDomeZBias;
	float m_fCloudDomeAlpha;
	NiFloatsExtraDataPtr m_spExtraData;
};
NiSmartPointer(sdCloud);
#endif