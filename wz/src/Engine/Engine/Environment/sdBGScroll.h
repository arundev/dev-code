//*************************************************************************************************
// ����:	BackGround Scroll, ��������
//---------------------------------------------------------
// ����:		
// ����:		2012-11-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_BGSCROLL__
#define _SD_ENGINE_SCENE_BGSCROLL__
#include "sdEntity.h"

class sdBGScroll : public sdEntity
{
public:
	sdBGScroll();
	virtual ~sdBGScroll();

	//
	virtual void Render();

protected:
	// ��������
	void CreateBackGroundScroll();

protected:
	// ������
	NiMeshPtr	m_spScrollMesh;

	// ������ɫ��
	NiMaterialPtr	m_spMaterial;

	// ��������
	std::string	m_kInnerScrollTexPath;
	std::string	m_kOuterScrollTexPath;
	NiTexturePtr m_spInnerScrollTexture;
	NiTexturePtr m_spOutterScrollTexture;
	NiTexturingPropertyPtr m_spTexturingProp;

	// �������
	float	m_fInnerScrollAlpha;
	float	m_fOuterScrollAlpha;
	float	m_fInnerScrollHeight;
	float	m_fOuterScrollHeight;
	float	m_fInnerScrollUWrap;
	float	m_fOuterScrollUWrap;
	NiFloatsExtraDataPtr m_spScrollMeshExtraData;
};
NiSmartPointer(sdBGScroll);
#endif