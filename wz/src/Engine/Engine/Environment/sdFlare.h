//*************************************************************************************************
// ����:	���ζ���
//---------------------------------------------------------
// ����:		
// ����:		2012-10-23
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_FLARE__
#define _SD_ENGINE_SCENE_FLARE__
#include "sdEntity.h"

class sdFlare : public sdEntity
{
public:
	sdFlare();
	virtual ~sdFlare();

	//
	void SetDepthTexture(NiTexture* spTexture);
	void Render();

protected:
	NiMeshPtr	m_spMesh;
};
NiSmartPointer(sdFlare);

#endif