//*************************************************************************************************
// 内容:	地表Doodad着色器的公共部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-03
// 最后修改:	
//*************************************************************************************************
#ifndef _DOODADS_COMMON_H__
#define _DOODADS_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 用于重新标记渲染通道
#define	DOODADS_LOD		0x00000001
#define	DOODADS_ANIM	0x00000002
#define	DOODADS_FADE	0x00000004
#define	DOODADS_NORMAL	0x00000008

float4	g_vGrassAnimationParams	: GLOBAL;
#define	g_fGrassAmbientScalar	g_vGrassAnimationParams.z
#define	g_fGrassSunLightScalar	g_vGrassAnimationParams.w

float	g_fGrassFadeInDistance	: GLOBAL;
float	g_fGrassFadeOutDistance	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf, 			false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(1, sdLightSampler,	sdLightBuf, 		false);		// 屏幕局部光照缓存
SD_LINEAR_WRAP_SAMPLE(2, sdNoiseSampler,	sdNoiseTex, 		false);		// 噪声图
SD_LINEAR_WRAP_SAMPLE(3, sdBlendSampler,	sdBlendTex,			false);		// 
SD_POINT_CLAMP_SAMPLE(4, sdDiffuseSampler,	sdDiffuseTex,		false);		// 

//---------------------------------------------------------------------------------------
// 风动画
//---------------------------------------------------------------------------------------
float3 WindAnimation(float3 vWorldPos, float3 vVertexPos, float fWeight)
{
	float fZ = (vVertexPos.z - vWorldPos.z) * 0.08f;
	float xx = sin(g_fTime * g_vGrassAnimationParams.x + vWorldPos.y);
	float yy = cos(g_fTime * (g_vGrassAnimationParams.x + 0.5f) + vWorldPos.y);
	float3 vBias = float3(xx, yy, 0.f) * fZ * g_vGrassAnimationParams.y;
	return vVertexPos + vBias * fWeight;
}
//---------------------------------------------------------------------------------------
#endif