//*************************************************************************************************
// ����:	�ر���ͼ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-08
// ����޸�:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_NORMAL_MAP_H__
#define _TERRAIN_NORMAL_MAP_H__
#include "sdDynamicTexture.h"

class sdHeightMap;

//-------------------------------------------------------------------------------------------------
// �ر���ͼ,��HeightMap�ܶ���ͬ,�������ϱ�Ե���ķ�����û�д洢��
//	1.���Ա���LOD�仯����Ĺ��ձ仯
//-------------------------------------------------------------------------------------------------
class sdNormalMap : public NiRefObject
{
public:
	sdNormalMap(uint uiSize, sdHeightMap* pkHeightMap);
	sdNormalMap(NiTexture* spNormalTexture);
	~sdNormalMap();

	// ����,�ֱ����������/����ָ������(���ڱ༭״̬)
	void	Update();
	void	Update(int iX, int iY, int iW, int iH);

	// ���Է��� 
	NiTexture*	GetGBTexture() const;

protected:
	// �߶�ͼ
	sdHeightMap* m_pkHeightMap;	

	// NormalMap������
	Engine::sdDynamicTexturePtr m_pkDynamicNormalTexture;	///< ���ڱ༭״̬,���ṩ��̬����
	NiTexturePtr m_spNormalTexture;							///< ������Ϸ״̬,������Ⱦ

	// �¶���ֵ
	NiPoint3	m_kPlanarThreshold;
};
NiSmartPointer(sdNormalMap);
//-------------------------------------------------------------------------------------------------
#endif