//*************************************************************************************************
// ����:	̫��/����
//---------------------------------------------------------
// ����:		
// ����:		2012-10-23
// ����޸�:
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