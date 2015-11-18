//*************************************************************************************************
// 内容:	天空盒对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_SKYBOX__
#define _SD_ENGINE_SCENE_SKYBOX__
#include "sdEntity.h"

class sdSkyBox : public sdEntity
{
public:
	sdSkyBox();
	virtual ~sdSkyBox();

	//
	virtual void Render();

protected:
	// 创建天空盒
	void CreateSkyBox();
	NiMeshPtr CreateQuad(NiPoint3* spVertexData, NiPoint2* spUVSetData, ushort* pusIndiceData, 
		NiTexture* spTexture, NiMaterial* spMaterial);

protected:
	// 天空盒六面
	NiMeshPtr	m_spPosXMesh;
	NiMeshPtr	m_spNegXMesh;
	NiMeshPtr	m_spPosYMesh;
	NiMeshPtr	m_spNegYMesh;
	NiMeshPtr	m_spPosZMesh;
	NiMeshPtr	m_spNegZMesh;

	// 天空盒着色器
	NiMaterialPtr	m_spMaterial;

	// 天空盒纹理
	std::string	m_kSkyBoxPosXYPath;
	std::string	m_kSkyBoxNegXYPath;
	std::string	m_kSkyBoxPosZPath;

	// 天空盒参数
	float	m_fSkyBoxWidth;		// 天空盒尺寸
	float	m_fSkyBoxLength;
	float	m_fSkyBoxHeight;	
	float	m_fSkyBoxBottom;	// 天空盒位置
	bool	m_bSktBoxNegZ;		// 天空盒是否有底
};
NiSmartPointer(sdSkyBox);
#endif