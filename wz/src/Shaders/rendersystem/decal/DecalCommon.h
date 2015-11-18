//*************************************************************************************************
// 内容:	贴花着色器的公共部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-08
// 最后修改:	2014-04-23
//*************************************************************************************************
#ifndef _DecalCommon_H__
#define _DecalCommon_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 观察坐标系到投射立方体投射坐标系变换矩阵
float4x4	g_mViewToProjectorUV		: GLOBAL;

// 投射体UV参数
float2		g_vProjectorUVScale			: GLOBAL = { 1.f, 1.f };
float2		g_vProjectorUVOffset		: GLOBAL = { 0.f, 0.f };
float2		g_vProjectorUVSpeed			: GLOBAL = { 1.f, 1.f };

// 投射方向
float3		g_vProjectorDirection		: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf,	false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(1, sdLightSampler,	sdLightBuf,	false);		// 光照缓存


//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 	vProjPos				: POSITION0;
	float4	vUVSetScreenTex			: TEXCOORD0;
	float4	vUVSetFarClipViewPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	// 模型坐标转换到投影坐标
	float4 vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vProjPos = vProjPos;
	
	// 投影空间齐次坐标
	float2 vTProjPos = vProjPos.xy / vProjPos.w;
	
	// 当前点对应远裁剪面上的点的世界坐标
	float4 vUVFarClipProjPos  = float4(vTProjPos.xy, 1.f, 1.f);
	kOutput.vUVSetFarClipViewPos = mul(vUVFarClipProjPos, g_mDepthToView) * vProjPos.w;

	// 投影坐标转换到屏幕纹理坐标(修正半像素偏移)
	float2 vUVSetScreenTex = GetTexCoordForPixel(vTProjPos);
	kOutput.vUVSetScreenTex  = float4(vUVSetScreenTex, 0.f, 1.f) * vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
#endif