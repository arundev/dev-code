//*************************************************************************************************
// ����:	�����߼�����,����������,��Ҫ��һЩ�ɷ��������Ƶ�ʰȡ����,����Trigger,Wrapper��
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_SCENE_ITEM_H__
#define _SD_GAMECORE_SCENE_ITEM_H__
#include "sdSceneServerObject.h"

class sdSceneItem : public sdSceneServerObject
{
	NiDeclareRTTI;

public:
	sdSceneItem();
	virtual ~sdSceneItem();
	

protected:

	// ��Դ
	//NiNodePtr	m_spNIFNode;
	//sdResourcePtr	m_spResLoadingPart;
};

#endif