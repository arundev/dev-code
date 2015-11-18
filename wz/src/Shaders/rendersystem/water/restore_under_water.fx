//*************************************************************************************************
// 内容:	Under Water
//	
//---------------------------------------------------
//	UnderWater.fx 
//	shader for under water
//---------------------------------------------------
//	Author 	: 	Xieweibo(XieWeibo@snda.com)
//	Date	:	2011.5.27
//---------------------------------------------------
//	Copyright (C) 2011 - All Rights Reserved
//---------------------------------------------------
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-04
// 最后修改:
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 水面高度
float	g_fWaterHeight		: GLOBAL;

// 深水深度
float	g_fDeepWaterDepth	: GLOBAL;

// 最大折射透明度 
float	g_fMaxRefractAlpha	: GLOBAL;

// 水下渲染的一些参数
//	.xy	caustics paramter
//	.z	fog intensity
//	.w	refraction distortion strength
float4	g_vUnderWaterParam	: GLOBAL;
#define	g_vCausticsParams	g_vUnderWaterParam.xy
#define	g_fFogIntensity		g_vUnderWaterParam.z
#define	g_fRefractionDistortionStrength	g_vUnderWaterParam.w

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,			sdGeomBuf, 		false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(1, sdRefractSampler,		sdRefractBuf, 	false);		// 场景折射缓存
SD_LINEAR_WRAP_SAMPLE(2, sdCausticsSampler,		sdCausticsTex, 	false);		// 焦散贴图
SD_LINEAR_WRAP_SAMPLE(3, sdWaterNormalSampler,	sdNormalBuf,	false);		// 屏幕水面法线缓存

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点坐标
	float2	vUVSet0			: TEXCOORD0;	// 屏幕矩形顶点纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos				: POSITION;		// 投影坐标
	//float2	vUVSetProjPos		: TEXCOORD0;	// 投影坐标
	float2	vUVSetScreenTex			: TEXCOORD1;	// 纠正半像素偏移的屏幕纹理坐标
	float4	vUVSetFarClipWorldPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的世界坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
// when undex water, of course we can only undex one water,
// we render full screen quad to restore pixel undex & above water surface
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos.xy, 1.f, 1.f);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 投影坐标
	//
	// 原文是:
	//		(kInput.vUVSet0 - 0.5f) * float2(2.f, -2.f);
	//
	//kOutput.vUVSetProjPos = kOutput.vProjPos;
	
	// 当前点对应远裁剪面上的点的世界坐标
	kOutput.vUVSetFarClipWorldPos = mul(kOutput.vProjPos, g_mDepthToWorld);
	
	return kOutput;
}


//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_RestoreUnderWater(VS_OUTPUT kInput) : COLOR0
{
	// 采样屏幕法线缓存
	float4 vNormal = tex2D(sdWaterNormalSampler, kInput.vUVSetScreenTex);
	
	// 计算扰动后的屏幕坐标
	float fRefractScale = g_fRefractionDistortionStrength * 2.f;// * 0.1f
	float2 vUVSet0 = kInput.vUVSetScreenTex.xy - fRefractScale * vNormal.xy;
	
	// 采样场景深度, 计算像素世界坐标,计算像素到相机欧拉距离
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSet0);
	float fDepth = UnpackDepth(vGeoTex.xy);
	float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarClipWorldPos.xyz / kInput.vUVSetFarClipWorldPos.w, fDepth);
	float fDistanceToCamera = distance(g_vCameraPostion, vPixelWorldPos);
	
	// 修正近平面处水面高度
	//bool bIsSky = false;
	if (fDepth <= 0.000001f)
	{
		vPixelWorldPos.z = g_fWaterHeight + 1.f;
		//bIsSky = true;
	}
	
	//
	float fDeltaDepth = g_fWaterHeight - vPixelWorldPos.z ;
	float fUnderWaterPixel = fDeltaDepth > -0.01f;	// check this pixel is under or above water plane
	float3 vSmoothFactor = 1.f - smoothstep(0, 
		float3(g_fDeepWaterDepth / 2.f, g_fDeepWaterDepth, 20.f * g_fMaxRefractAlpha),
		float3(abs(fDeltaDepth - g_fDeepWaterDepth / 2.f), fDeltaDepth, fDistanceToCamera));
	//float fWaterSmooth = 1.f - smoothstep(0, 2.f, fDeltaDepth);
	
	// 采样焦散纹理
	float4 vCausticsColor = tex2D(sdCausticsSampler, vPixelWorldPos.xy / g_vCausticsParams.x);

	// 采样折射纹理
	float4 vRefracColor = tex2D(sdRefractSampler, vUVSet0);
	vRefracColor.rgb *= (1.f + vCausticsColor.r * g_vCausticsParams.y * vSmoothFactor.x * fUnderWaterPixel * vSmoothFactor.z);
	
	// lerp between deep water color with refraction color
	//float fDist = distance(g_vCameraPostion, vPixelWorldPos, fDepth);
	//float fFogFactor = fUnderWaterPixel ? 1.f / exp(fDist * g_fFogIntensity * 0.125f) : 1.f;
	//float vFinalColor = lerp(float4(vWaterColor, 1.f), vRefracColor, fFogFactor);
	float vFinalColor = vRefracColor;
	
	return vFinalColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_RestoreUnderNormalBlendWater(VS_OUTPUT kInput) : COLOR0
{
	// 法线
	// @{
	// 根据水移动速率计算采样位置偏移
	float fTranslation = fmod(g_fTime, 200.f) * 0.005f;
	float2 vTranslation = float2(fTranslation, -fTranslation);
	
	// 采样法线纹理
	float2 vUVSet0 = kInput.vUVSetScreenTex; // * 0.2f;
	float3 vNormal = tex2D(sdWaterNormalSampler, vUVSet0 + vTranslation * 2.f).xyz;
	vNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 2.f + vTranslation * 4.f).xyz;
	vNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 4.f + vTranslation * 2.f).xyz;
	vNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 8.f + vTranslation).xyz;
	
	// 恢复法线
	vNormal = vNormal * 0.5f - 1.f;
	// @}
	

	// 计算扰动后的屏幕坐标
	float fRefractScale = g_fRefractionDistortionStrength * 2.f;// * 0.1f
	float2 vUVSet1 = kInput.vUVSetScreenTex.xy - fRefractScale * vNormal.xy * 0.05f;
	
	// 采样场景深度, 计算像素世界坐标,计算像素到相机欧拉距离
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSet1);
	float fDepth = UnpackDepth(vGeoTex.xy);
	float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarClipWorldPos.xyz / kInput.vUVSetFarClipWorldPos.w, fDepth);
	float fDistanceToCamera = distance(g_vCameraPostion, vPixelWorldPos);
	
	// 修正近平面处水面高度
	//bool bIsSky = false;
	if (fDepth <= 0.000001f)
	{
		vPixelWorldPos.z = g_fWaterHeight + 1.f;
		//bIsSky = true;
	}
	
	//
	float fDeltaDepth = g_fWaterHeight - vPixelWorldPos.z ;
	float fUnderWaterPixel = fDeltaDepth > -0.01f;	// check this pixel is under or above water plane
	float3 vSmoothFactor = 1.f - smoothstep(0, 
		float3(g_fDeepWaterDepth / 2.f, g_fDeepWaterDepth, 20.f * g_fMaxRefractAlpha),
		float3(abs(fDeltaDepth - g_fDeepWaterDepth / 2.f), fDeltaDepth, fDistanceToCamera));
	//float fWaterSmooth = 1.f - smoothstep(0, 2.f, fDeltaDepth);
	
	// 采样焦散纹理
	float4 vCausticsColor = tex2D(sdCausticsSampler, vPixelWorldPos.xy / g_vCausticsParams.x);

	// 采样折射纹理
	float4 vRefracColor = tex2D(sdRefractSampler, vUVSet1);
	vRefracColor.rgb *= (1.f + vCausticsColor.r * g_vCausticsParams.y * vSmoothFactor.x * fUnderWaterPixel * vSmoothFactor.z);
	
	// lerp between deep water color with refraction color
	float fDist = distance(g_vCameraPostion, vPixelWorldPos);
	float fFogFactor = fUnderWaterPixel ? 1.f / exp(fDist * g_fFogIntensity * 0.125f) : 1.f;
	float vFinalColor = lerp(float4(g_vWaterColor, 1.f), vRefracColor, fFogFactor);
	
	return vFinalColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_RestoreUnderNoReflectNormalBlendWater(VS_OUTPUT kInput) : COLOR0
{
	// 采样场景深度, 计算像素世界坐标,计算像素到相机欧拉距离
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	float3 vPixelWorldPos = lerp(g_vCameraPostion, kInput.vUVSetFarClipWorldPos.xyz / kInput.vUVSetFarClipWorldPos.w, fDepth);
	float fDistanceToCamera = distance(g_vCameraPostion, vPixelWorldPos);
	
	// 修正近平面处水面高度
	//bool bIsSky = false;
	if (fDepth <= 0.000001f)
	{
		vPixelWorldPos.z = g_fWaterHeight + 1.f;
		//bIsSky = true;
	}
	
	//
	float fDeltaDepth = g_fWaterHeight - vPixelWorldPos.z ;
	float fUnderWaterPixel = fDeltaDepth > -0.01f;	// check this pixel is under or above water plane
	float3 vSmoothFactor = 1.f - smoothstep(0, 
		float3(g_fDeepWaterDepth / 2.f, g_fDeepWaterDepth, 20.f * g_fMaxRefractAlpha),
		float3(abs(fDeltaDepth - g_fDeepWaterDepth / 2.f), fDeltaDepth, fDistanceToCamera));
	//float fWaterSmooth = 1.f - smoothstep(0, 2.f, fDeltaDepth);
	
	// 采样焦散纹理
	float4 vCausticsColor = tex2D(sdCausticsSampler, vPixelWorldPos.xy / g_vCausticsParams.x);

	// 采样折射纹理
	float4 vRefracColor = tex2D(sdRefractSampler, kInput.vUVSetScreenTex);
	vRefracColor.rgb *= (1.f + vCausticsColor.r * g_vCausticsParams.y * vSmoothFactor.x * fUnderWaterPixel * vSmoothFactor.z);
	
	// lerp between deep water color with refraction color
	float fDist = distance(g_vCameraPostion, vPixelWorldPos);
	float fFogFactor = fUnderWaterPixel ? 1.f / exp(fDist * g_fFogIntensity * 0.125f) : 1.f;
	float vFinalColor = lerp(float4(g_vWaterColor, 1.f), vRefracColor, fFogFactor);
	
	return vFinalColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Water_RestoreUnderWater
<
	string Description = "Water_RestoreUnderWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_RestoreUnderWater ();
	}
}
//---------------------------------------------------------------------------------------
technique Water_RestoreUnderNormalBlendWater
<
	string Description = "Water_RestoreUnderNormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_RestoreUnderNormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------
technique Water_RestoreUnderNoReflectNormalBlendWater
<
	string Description = "Water_RestoreUnderNoReflectNormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_RestoreUnderNoReflectNormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------