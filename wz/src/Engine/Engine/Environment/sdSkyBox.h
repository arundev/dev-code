//*************************************************************************************************
// ����:	��պж���
//---------------------------------------------------------
// ����:		
// ����:		2012-10-31
// ����޸�:
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
	// ������պ�
	void CreateSkyBox();
	NiMeshPtr CreateQuad(NiPoint3* spVertexData, NiPoint2* spUVSetData, ushort* pusIndiceData, 
		NiTexture* spTexture, NiMaterial* spMaterial);

protected:
	// ��պ�����
	NiMeshPtr	m_spPosXMesh;
	NiMeshPtr	m_spNegXMesh;
	NiMeshPtr	m_spPosYMesh;
	NiMeshPtr	m_spNegYMesh;
	NiMeshPtr	m_spPosZMesh;
	NiMeshPtr	m_spNegZMesh;

	// ��պ���ɫ��
	NiMaterialPtr	m_spMaterial;

	// ��պ�����
	std::string	m_kSkyBoxPosXYPath;
	std::string	m_kSkyBoxNegXYPath;
	std::string	m_kSkyBoxPosZPath;

	// ��պв���
	float	m_fSkyBoxWidth;		// ��պгߴ�
	float	m_fSkyBoxLength;
	float	m_fSkyBoxHeight;	
	float	m_fSkyBoxBottom;	// ��պ�λ��
	bool	m_bSktBoxNegZ;		// ��պ��Ƿ��е�
};
NiSmartPointer(sdSkyBox);
#endif