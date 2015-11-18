//*************************************************************************************************
// ����:	�ر���Դͼ��
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_LAYER_MAP_H__
#define _TERRAIN_LAYER_MAP_H__
#include "sdMemoryTexture.h"

// ���ͼ��������
#define MAX_LAYER_NAME_LEN 32
//-------------------------------------------------------------------------------------------------
// ��ʾ�ر�һ����Դ��ͼ(�����ڱ༭��״̬��ʹ��,�൱��Cry��SSurfaceType)
//	\TODO ͳһ������ص�ResourceSystem
//	\TODO �ǵ�BigWorld��UVMapType\Rotate\Translate\Scale����
//	\TODO WZ�а���ͼ�ڼ���ʱ��̬ת��ΪDXT1��ʽ�Լ����ڴ�ʹ��
//-------------------------------------------------------------------------------------------------
class sdLayerMap : public NiRefObject
{
public:
	sdLayerMap(const char* szName, const char* szDiffuseMap, const char* szNormalMap, 
		float fRepeatSize = 1.0f, float fMipmapBias = 0.0f);
	~sdLayerMap();

	const char*	GetName() const { return m_szName;}
	void		SetName(const char* szName) { NIASSERT(szName); strncpy(m_szName, szName, MAX_LAYER_NAME_LEN);}

	const char*	GetDiffuseMapPath() const { return m_szDiffuseMap;}
	void		SetDiffuseMapPath(const char* szName);

	const char*	GetNormalMapPath() const { return m_szNormalMap;}
	void		SetNormalMapPath(const char* szName);

	NiTexture*	GetDiffuseMap() { return m_pkDiffuseMap ? m_pkDiffuseMap->GetGBTexture() : NULL;}
	NiTexture*	GetNormalMap() { return m_pkNormalMap ? m_pkNormalMap->GetGBTexture() : NULL;}

	float	GetRepeatSize() const { return m_fRepeatSize;}
	void	SetRepeatSize(float fRepeatSize) { m_fRepeatSize = fRepeatSize; }

	float	GetMipmapBias() const { return m_fMipmapBias;};
	void	SetMipmapBias(float fMipmapBias) { m_fMipmapBias = fMipmapBias; }

protected:
	// �������ݵ�Texture
	Engine::sdMemoryTexturePtr LoadPixelDataToD3DTexture(const char* szName);

protected:
	// ͼ����
	char m_szName[MAX_LAYER_NAME_LEN];			

	// ͼ������
	char m_szDiffuseMap[MAX_PATH_LEN];	
	char m_szNormalMap[MAX_PATH_LEN];
	Engine::sdMemoryTexturePtr m_pkDiffuseMap;
	Engine::sdMemoryTexturePtr m_pkNormalMap;

	// ͼ����Ϣ
	float	m_fRepeatSize;
	float	m_fMipmapBias;
};
NiSmartPointer(sdLayerMap);
//-------------------------------------------------------------------------------------------------
#endif