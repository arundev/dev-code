//*************************************************************************************************
// 内容:	StaticMesh的着色器的特有的公共变量与函数
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-10
// 最后修改:
//*************************************************************************************************
#ifndef _STATICMESH_COMMON_H__
#define _STATICMESH_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// 变量
//---------------------------------------------------------------------------------------
// LightMap
float4	a_vLightMapChannel			: ATTRIBUTE	= float4(1.f, 0.f, 0.f,	0.f);
float4 	a_vLightMapTransform		: ATTRIBUTE	= float4(0.f, 0.f, 1.f, 1.f);

// DetailNormalMap的缩放与采样
float3	a_vDetailNormalScale		: ATTRIBUTE	= float3(1.f, 1.f, 1.f);
float2	a_vDetailNormalUVTiling		: ATTRIBUTE = float2(5.f, 5.f);
float	a_fDetailNormalFadeDistance	: ATTRIBUTE = 30.f;

//
float4x4	a_mWorldProjectionMat	: ATTRIBUTE;

// 树叶动画
float4	a_vTransmit					: ATTRIBUTE = float4(1.f, 1.f, 1.f, 1.f);

// 辉光强度
float	a_fGlowPower				: ATTRIBUTE = 1.f;

//---------------------------------------------------------------------------------------
// 延迟着色的纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdLightBufferSampler, 		sdTexLightBuffer, 	false);	
SD_POINT_CLAMP_SAMPLE(1, 	sdGeomBufferSampler, 		sdTexGeomBuffer, 	false);
SD_POINT_CLAMP_SAMPLE(2, 	sdMatBufferSampler, 		sdTexMatBuffer, 	false);	// Old MRT
SD_LINEAR_CLAMP_SAMPLE(3, 	sdCurveFogBufferSampler, 	sdTexFogCurveMap, 	false);	

SD_POINT_CLAMP_SAMPLE(2, 	sdMatBuffer0Sampler, 		sdTexMatBuffer0, 	false);	// MRT3
SD_POINT_CLAMP_SAMPLE(3, 	sdMatBuffer1Sampler, 		sdTexMatBuffer1, 	false);	// MRT3
SD_POINT_CLAMP_SAMPLE(4, 	sdMatBuffer2Sampler, 		sdTexMatBuffer2, 	false);	// MRT3

//---------------------------------------------------------------------------------------
// 物件着色器渲染通道
//---------------------------------------------------------------------------------------
#define VERTEX_DIFFUSEMAP_CHANNEL			0x00000001	///< 漫反射通道
#define VERTEX_NORMALMAP_CHANNEL			0x00000002	///< 法线贴图通道
#define VERTEX_GLOSSMAP_CHANNEL				0x00000004	///< 高光贴图通道
#define VERTEX_LIGHTMAP_CHANNEL				0x00000008	///< 光照贴图通道
#define VERTEX_GLOWMAP_CHANNEL				0x00000010	///< 辉光贴图通道
#define VERTEX_DETAIL_NORMAL_CHANNEL		0x00000020	///< 细节贴图通道
#define VERTEX_ALPHATEST_FLAG				0x00000040	///< 透明测试
#define VERTEX_COMPACTVERTEX_FLAG			0x00000080	///< 顶点压缩
#define VERTEX_LEAFANIM_FLAG				0x00000100	///< 树叶顶点动画

#define	PIXEL_SPECULAR_LIGHTTING_CHANNEL	0x00000001	///< 高光
#define	PIXEL_RIM_LIGHTTING_CHANNEL			0x00000002	///< 轮廓光
#define	PIXEL_GLOW_LIGHTTING_CHANNEL		0x00000004	///< 辉光

//---------------------------------------------------------------------------------------
// 树叶动画
//---------------------------------------------------------------------------------------
float4 SmoothCurve(float4 x)
{
	return x * x * (3.f - 2.f * x);
}
//---------------------------------------------------------------------------------------
float4 TriangleWave(float4 x)
{
	return abs(frac(x + 0.5f) * 2.f - 1.f);
}
//---------------------------------------------------------------------------------------
float4 SmoothTriangleWave(float4 x)
{
	return SmoothCurve(TriangleWave(x));
}
//---------------------------------------------------------------------------------------
float3 CalcDetailWave(float3 vPos, float3 vNormal, float fSpeed)
{
	float fBranchPhase = 1.f;// dot(vPos, 1);
	float fVertexPhase = dot(vPos.xyz, fBranchPhase);
	float2 vWavesIn = g_fTime * fSpeed + float2(fVertexPhase, fVertexPhase);
	float4 vWaves = frac(vWavesIn.xxyy * float4(1.975f, 0.793f, 0.375f, 0.193f)) * 2.f - 1.f;
	vWaves = SmoothTriangleWave(vWaves);
	float2 vWavesSum = vWaves.xz + vWaves.yw;
	return vWavesSum.xxy * float3(vNormal.x * 0.2f * a_vTransmit.w / 20.f, vNormal.y * 0.2f * a_vTransmit.w / 20.f, 0.2f * a_vTransmit.w / 20.f);
}
//---------------------------------------------------------------------------------------
#endif