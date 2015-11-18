//*************************************************************************************************
// ����:	������ɫ���Ĺ�������(V2,ruantianlong modifiy)
//---------------------------------------------------------
// ����:		
// ����:		2013-09-11
// ����޸�:	2013-09-12
//*************************************************************************************************
#ifndef _TERRAIN_COMMON_V2_H__
#define _TERRAIN_COMMON_V2_H__

//
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// ��ر���
//---------------------------------------------------------------------------------------
// ������ͼ���ߴ�
float2	g_vTerrainAtlasSize			: GLOBAL;	

// ��������򻯲���
float2 	g_vTerrainSkipParam			: GLOBAL;
#define g_fTerrainSkipLevel			g_vTerrainSkipParam.x
#define g_fTerrainSkipLevelPower	g_vTerrainSkipParam.y	///< (0.5)^g_vTerrainSkipParam.x
//---------------------------------------------------------------------------------------
// ����Mipmap�ȼ�
//	@param[in]	vUVSet		��������[0,1]
//	@param[in]	vSize		����ߴ�
//	@param[in]	fScale		
//	@param[in]	fRepeatSize	
//	return					��ѡȡ��mipmap�ȼ�
//---------------------------------------------
// �ο����ϣ�
//	GPU Pro	�� Virtual Texture Mapping
//	GPU Pro	�� Accelerating Virtual Texturing using CUDA
//	GPU Pro 2 �� Large-Scale Terrain Rendering for Outdoor Games
//	GPU Gem �� Fast Filter-Width Estimates with Texture Maps
//	http://http.developer.nvidia.com/GPUGems/gpugems_ch25.html
//	http://www.gamedev.net/topic/444559-easy-way-to-get-mip-level-used-when-sampling-a-texture-ps30/
//	http://www.gamasutra.com/view/feature/3301/runtime_mipmap_filtering.php?print=1
//	http://www.opengl.org/discussion_boards/showthread.php/177520-Mipmap-level-calculation-using-dFdx-dFdy
//	http://www.opengl.org/discussion_boards/showthread.php/171485-Texture-LOD-calculation-(useful-for-atlasing)
//---------------------------------------------
// �㷨һ(��ȷ����):
//	float2 vSampleCoordPixels = vUVSet * vSize;			///< �����Ӧ���������λ��
//	float2 vXDeriv = ddx(vSampleCoordPixels);			///< �������λ��x����ƫ����
//	float2 vYDeriv = ddy(vSampleCoordPixels);			///< �������λ��y����ƫ����
//	float fD = max(dot(vXDeriv, vXDeriv), dot(vYDeriv, vYDeriv));	
//	float fMipmapLevel = max(0.f, (0.5f * log2(fD)));
//---------------------------------------------
// �㷨��:
//	float2 vSampleCoordPixels = vUVSet * vSize;
//	float2 vXDeriv = ddx(vSampleCoordPixels);
//	float2 vYDeriv = ddy(vSampleCoordPixels);
//	float fD = max(max(abs(vXDeriv.x), abs(vXDeriv.y), max(abs(vYDeriv.x), abs(vYDeriv.y));	
//	float fMipmapLevel = max(0.f, log2(1.f / fD));
//---------------------------------------------------------------------------------------
float ComputeMipmapLevel(float2 vUVSet, float2 vSize, float fScale, float fRepeatSize)
{
	vSize *= g_vTerrainSkipParam.y;
	
	// ��������ռ��Ӧ������
	float2 vSampleCoordPixels = vUVSet * vSize * fScale;
	
	//
	float2 vXDeriv = ddx(vSampleCoordPixels) * fRepeatSize;
	float2 vYDeriv = ddy(vSampleCoordPixels) * fRepeatSize;
	float fD = max(dot(vXDeriv, vXDeriv), dot(vYDeriv, vYDeriv));
	
	// ����
	// 	0.5f * log2(fD) �ȼ��� log2(sqrt(fD))
	//	2.f��һ������Ч���Ĺ̶�ƫ��
	float fMipmapLevel = max(0.f, (0.5f * log2(fD) - 2.f));
	
	return fMipmapLevel;
}
//---------------------------------------------------------------------------------------
float ComputeMipmapLevel_NODDXY(float2 vXDeriv, float2 vYDeriv, float fRepeatSize)
{
	vXDeriv *= fRepeatSize;
	vYDeriv *= fRepeatSize;
	
	float fD = max(dot(vXDeriv, vXDeriv), dot(vYDeriv, vYDeriv));
	float fMipmapLevel = max(0.f, (0.5f * log2(fD) - 2.f));

	return fMipmapLevel;
}
//---------------------------------------------------------------------------------------
// ��ԭʼͼҳ����Ĳ�������ת����ͼ���Ĳ�������
//	@param[in] 	vMipmapScale	ԭʼͼҳ���������ֵ(��������Ϊmipmap���ԭʼ���������ֵ��ʹ�õ�)
//	@param[in]	vUVSet			ԭʼͼҳ����Ĳ�������
//	@param[in]	vRowCol			ԭʼͼҳ�������ͼ����λ��
//	return						��Ӧ��ͼ���Ĳ�������
//---------------------------------------------------------------------------------------
float2 ComputeUV(float vMipmapScale, float2 vUVSet, float2 vRowCol)
{
	// ����������������������, ���������ͼҳ��Ե
	//	1.��ȡ��[0,1)����
	//	1.���Ŵ���[0,1)�任��[0, 1.f - 1.f / width)
	//	2.������ƫ�ƣ�[0, 1.f - 1.f / width)�任��[0.5f / width, 1.f - 0.5f / width)
	float2 vUVSetFrac = frac(vUVSet);
	vUVSetFrac = vUVSetFrac * (1.f - 1.f / (512.f * vMipmapScale * g_vTerrainSkipParam.y)) + 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y);
	//vUVSetFrac = clamp(vUVSetFrac, 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y), 1.f - 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y));
	
	// �����Ӧ��ͼ����������
	float2 vRowColScale = 512.f / g_vTerrainAtlasSize;	///< ͼ����ÿ��ͼ��ԭʼ�ߴ���512
	float2 vNewUVSet = (vUVSetFrac + vRowCol.yx) * vRowColScale;
	
	return vNewUVSet;
}
//---------------------------------------------------------------------------------------
#endif