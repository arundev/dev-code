//*************************************************************************************************
// 内容:	BackGround Scroll, 背景卷轴
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-05
// 最后修改:
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
	// 创建卷轴
	void CreateBackGroundScroll();

protected:
	// 卷轴面
	NiMeshPtr	m_spScrollMesh;

	// 卷轴着色器
	NiMaterialPtr	m_spMaterial;

	// 卷轴纹理
	std::string	m_kInnerScrollTexPath;
	std::string	m_kOuterScrollTexPath;
	NiTexturePtr m_spInnerScrollTexture;
	NiTexturePtr m_spOutterScrollTexture;
	NiTexturingPropertyPtr m_spTexturingProp;

	// 卷轴参数
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