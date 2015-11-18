//*************************************************************************************************
// 内容:	Under Water
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_WRAP_SAMPLE(0, 	sdWaterNormalSampler,		sdWaterNormalTex,	false);	// 水面法线纹理
SD_POINT_CLAMP_SAMPLE(5, 	sdRefractSampler,			sdRefractTex,		false);	// 水面折射纹理
SD_LINEAR_CLAMP_SAMPLE(6, 	sdFresnelSampler,			sdFresnelTex,		false);	// 菲涅尔系数纹理

// 
texture sdReflectCubeTex<string	NTM = "shader";	int NTMIndex = 3;>;																
samplerCUBE sdReflectCubeSampler = sampler_state							
{																
	Texture		= (sdReflectCubeTex);										
	ADDRESSU	= WRAP;										
	ADDRESSV	= WRAP;	
	ADDRESSW	= WRAP;										
	MAGFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	//MIPFILTER	=  ANISOTROPIC;
	MAXANISOTROPY = 4;
	//MipmapLodBias = (LODBias);
	SRGBTexture = true;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSet0				: TEXCOORD0;	// xy height map uv,zw uniforma uv
	float4	vUVSetWorldPos		: TEXCOORD1;	// .w save refrac scale
	float4	vUVSetViewPos		: TEXCOORD2;
	float4	vUVSetProjPos		: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput;
	
	// 世界坐标
	float4 vWorldPosition = mul(float4(vPos, 1.0f), g_mWorld);
	kOutput.vUVSetWorldPos = vWorldPosition;
	
	// 观察坐标
	float4 vViewPosition = mul(float4(vPos, 1.0f), g_mWorldView);
	kOutput.vUVSetViewPos = vViewPosition;
	
	// 投影坐标
	float4 vProjPosition = mul(float4(vWorldPosition.xyz, 1), g_mViewProj);
	kOutput.vProjPos = vProjPosition;
	kOutput.vUVSetProjPos = vProjPosition;

	// 纹理坐标
	float2 vHeightUVSet = vWorldPosition.xy / g_fWaterHeightMapSize;
	kOutput.vUVSet0 = vHeightUVSet;
	
	//
	kOutput.vUVSetWorldPos.w = g_fRefractionDistortionStrength / max(0.002f,vProjPosition.z / vProjPosition.w);
	
	// 当前点对应远裁剪面世界坐标
	//float4 vFarWorldPos = mul(float4(vProjPos.xy / vProjPos.w, 1, 1), g_mDepthToWorld) * vProjPos.w;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_UnderFFTWater(VS_OUTPUT kInput) : COLOR0
{
	// 采样法线贴图
	float4 N = -tex2D(sdWaterNormalSampler, kInput.vUVSet0.xy);
	
	// 反射
	// @{
	// 计算观察方向
	float3 V = g_vViewPos - kInput.vUVSetWorldPos.xyz;
	V = normalize(V);
	
	// 计算反射纹理坐标
	float3 vReflectVec = normalize(reflect(V, N.xyz));
	
	// 采样反射纹理
	float4 vReflectColor = texCUBE(sdReflectCubeSampler, -vReflectVec.xzy);
	// @}
	
	// 折射
	// @{
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// 计算折射纹理坐标
	// Calc refraction UV which bease on device Z
	//float fRefracScale = kInput.vUVSetWorldPos.w;
	float fRefracScale = g_fRefractionDistortionStrength * 0.6f;
	float2 vRefractionUV = vScreenUVSet - fRefracScale * N.xy;
	
	// Get refraction texture color
	float4 vRefractionColor = tex2D(sdRefractSampler, vRefractionUV);
	// @}
	
	// 合成
	vRefractionColor.rgb = lerp(vRefractionColor.rgb, vReflectColor.rgb, 0.6f);
	
	// 合成雾
	float fDistance = distance(g_vViewPos, kInput.vUVSetWorldPos.xyz);
	float fFogFactor = 1.f / exp(fDistance * g_fFogIntensity * 0.125f);
	vRefractionColor.rgb = lerp(g_vWaterColor, vRefractionColor.rgb, fFogFactor);
	
	return vRefractionColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_UnderNormalBlendWater(VS_OUTPUT kInput) : COLOR0
{
	// 法线
	// @{
	// 根据水移动速率计算采样位置偏移(这里没考虑调节水移动方向)
	float fCycle = 1.f / g_fWaterSpeed;
	float fTranslation = fmod(g_fTime, fCycle) * g_fWaterSpeed;
	float2 vTranslation = float2(fTranslation, -fTranslation);

	// 采样法线纹理
	float2 vUVSet0 = kInput.vUVSet0 * 0.2f;
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, vUVSet0 + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 2.f + vTranslation * 4.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 4.f + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 8.f + vTranslation).xyz;
	
	// 恢复法线
	vBaseNormal = vBaseNormal * 0.5f - 1.f;
	
	// 计算用于普通计算的法线
	vBaseNormal.xy *= g_fNormalMapScale;
	
	float3 N = -normalize(vBaseNormal);
	// @}

	// 反射
	// @{
	// 计算观察方向
	float3 V = g_vViewPos - kInput.vUVSetWorldPos.xyz;
	V = normalize(V);
	
	// 计算反射纹理坐标
	float3 vReflectVec = normalize(reflect(V, N));
	
	// 采样反射纹理
	float4 vReflectColor = texCUBE(sdReflectCubeSampler, -vReflectVec.xzy);
	// @}

	// 折射
	// @{
	// 计算屏幕纹理坐标
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// 计算折射纹理坐标
	// Calc refraction UV which bease on device Z
	//float fRefracScale = kInput.vUVSetWorldPos.w;
	float fRefracScale = g_fRefractionDistortionStrength * 0.6f;
	float2 vRefractionUV = vScreenUVSet - fRefracScale * N.xy;
	
	// Get refraction texture color
	float4 vRefractionColor = tex2D(sdRefractSampler, vRefractionUV);
	// @}
	
	// 合成
	vRefractionColor.rgb = lerp(vRefractionColor.rgb, vReflectColor.rgb, 0.6f);
	
	// 合成雾
	float fDistance = distance(g_vViewPos, kInput.vUVSetWorldPos.xyz);
	float fFogFactor = 1.f / exp(fDistance * g_fFogIntensity * 0.125f);
	vRefractionColor.rgb = lerp(g_vWaterColor, vRefractionColor.rgb, fFogFactor);
	
	return vRefractionColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Water_UnderFFTWater
<
	string Description = "Water_UnderFFTWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_UnderFFTWater ();
	}
}
//---------------------------------------------------------------------------------------
technique Water_UnderNormalBlendWater
<
	string Description = "Water_UnderNormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_UnderNormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------