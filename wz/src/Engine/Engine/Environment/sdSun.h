//*************************************************************************************************
// 内容:	太阳/月亮
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_SUN__
#define _SD_ENGINE_SCENE_SUN__
#include "sdEntity.h"

class sdSun : public sdEntity
{
public:
	sdSun();
	virtual ~sdSun();

	//
	void SetDepthTexture(NiTexture* spTexture);
	void Render();

protected:
	NiMeshPtr	m_spMesh;
};
NiSmartPointer(sdSun);

#endif