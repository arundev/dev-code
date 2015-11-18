//*************************************************************************************************
// 内容:	地形着色器的公共部分(V2,ruantianlong modifiy)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-09-11
// 最后修改:	2013-09-12
//*************************************************************************************************
#ifndef _TERRAIN_COMMON_V2_H__
#define _TERRAIN_COMMON_V2_H__

//
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 相关变量
//---------------------------------------------------------------------------------------
// 地形贴图集尺寸
float2	g_vTerrainAtlasSize			: GLOBAL;	

// 地形纹理简化参数
float2 	g_vTerrainSkipParam			: GLOBAL;
#define g_fTerrainSkipLevel			g_vTerrainSkipParam.x
#define g_fTerrainSkipLevelPower	g_vTerrainSkipParam.y	///< (0.5)^g_vTerrainSkipParam.x
//---------------------------------------------------------------------------------------
// 计算Mipmap等级
//	@param[in]	vUVSet		纹理坐标[0,1]
//	@param[in]	vSize		纹理尺寸
//	@param[in]	fScale		
//	@param[in]	fRepeatSize	
//	return					所选取的mipmap等级
//---------------------------------------------
// 参考资料：
//	GPU Pro	的 Virtual Texture Mapping
//	GPU Pro	的 Accelerating Virtual Texturing using CUDA
//	GPU Pro 2 的 Large-Scale Terrain Rendering for Outdoor Games
//	GPU Gem 的 Fast Filter-Width Estimates with Texture Maps
//	http://http.developer.nvidia.com/GPUGems/gpugems_ch25.html
//	http://www.gamedev.net/topic/444559-easy-way-to-get-mip-level-used-when-sampling-a-texture-ps30/
//	http://www.gamasutra.com/view/feature/3301/runtime_mipmap_filtering.php?print=1
//	http://www.opengl.org/discussion_boards/showthread.php/177520-Mipmap-level-calculation-using-dFdx-dFdy
//	http://www.opengl.org/discussion_boards/showthread.php/171485-Texture-LOD-calculation-(useful-for-atlasing)
//---------------------------------------------
// 算法一(正确做法):
//	float2 vSampleCoordPixels = vUVSet * vSize;			///< 计算对应的纹理采样位置
//	float2 vXDeriv = ddx(vSampleCoordPixels);			///< 计算采样位置x方向偏导数
//	float2 vYDeriv = ddy(vSampleCoordPixels);			///< 计算采样位置y方向偏导数
//	float fD = max(dot(vXDeriv, vXDeriv), dot(vYDeriv, vYDeriv));	
//	float fMipmapLevel = max(0.f, (0.5f * log2(fD)));
//---------------------------------------------
// 算法二:
//	float2 vSampleCoordPixels = vUVSet * vSize;
//	float2 vXDeriv = ddx(vSampleCoordPixels);
//	float2 vYDeriv = ddy(vSampleCoordPixels);
//	float fD = max(max(abs(vXDeriv.x), abs(vXDeriv.y), max(abs(vYDeriv.x), abs(vYDeriv.y));	
//	float fMipmapLevel = max(0.f, log2(1.f / fD));
//---------------------------------------------------------------------------------------
float ComputeMipmapLevel(float2 vUVSet, float2 vSize, float fScale, float fRepeatSize)
{
	vSize *= g_vTerrainSkipParam.y;
	
	// 计算采样空间对应的像素
	float2 vSampleCoordPixels = vUVSet * vSize * fScale;
	
	//
	float2 vXDeriv = ddx(vSampleCoordPixels) * fRepeatSize;
	float2 vYDeriv = ddy(vSampleCoordPixels) * fRepeatSize;
	float fD = max(dot(vXDeriv, vXDeriv), dot(vYDeriv, vYDeriv));
	
	// 计算
	// 	0.5f * log2(fD) 等价于 log2(sqrt(fD))
	//	2.f是一个调整效果的固定偏置
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
// 将原始图页纹理的采样坐标转换到图集的采样坐标
//	@param[in] 	vMipmapScale	原始图页纹理的缩放值(这里是作为mipmap相对原始纹理的缩放值来使用的)
//	@param[in]	vUVSet			原始图页纹理的采样坐标
//	@param[in]	vRowCol			原始图页纹理相对图集的位置
//	return						对应的图集的采样坐标
//---------------------------------------------------------------------------------------
float2 ComputeUV(float vMipmapScale, float2 vUVSet, float2 vRowCol)
{
	// 对输入纹理坐标做出调整, 避免采样到图页边缘
	//	1.截取出[0,1)部分
	//	1.缩放处理，[0,1)变换到[0, 1.f - 1.f / width)
	//	2.半像素偏移，[0, 1.f - 1.f / width)变换到[0.5f / width, 1.f - 0.5f / width)
	float2 vUVSetFrac = frac(vUVSet);
	vUVSetFrac = vUVSetFrac * (1.f - 1.f / (512.f * vMipmapScale * g_vTerrainSkipParam.y)) + 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y);
	//vUVSetFrac = clamp(vUVSetFrac, 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y), 1.f - 0.5f / (512.f * vMipmapScale * g_vTerrainSkipParam.y));
	
	// 计算对应的图集采样坐标
	float2 vRowColScale = 512.f / g_vTerrainAtlasSize;	///< 图集中每张图的原始尺寸是512
	float2 vNewUVSet = (vUVSetFrac + vRowCol.yx) * vRowColScale;
	
	return vNewUVSet;
}
//---------------------------------------------------------------------------------------
#endif