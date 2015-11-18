//*************************************************************************************************
// 内容:	贴花着色器具体实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-09
// 最后修改:	2014-04-20
//*************************************************************************************************
#include "DecalCommon.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
//#undef VS_INPUT
//#undef VS_OUTPUT

//#undef VS_SHADER
#undef PS_SHADER

#undef TECHNIQUE

#undef DIFFUSEMAP
#undef NORMALMAP
#undef LIGHTMAP
#undef GLOWMAP

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, DIFFUSEMAP_CHANNEL)
	#define DIFFUSEMAP 1
#else
	#define DIFFUSEMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, LIGHTMAP_CHANNEL)
	#define LIGHTMAP 1
#else
	#define LIGHTMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, GLOWMAP_CHANNEL)
	#define GLOWMAP 1
#else
	#define GLOWMAP 0
#endif

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
#if (DIFFUSEMAP || GLOWMAP)
	#define PS_SHADER	HEADER_KEY(PS_Main, CHANNELS_FLAG)
	#define TECHNIQUE	HEADER_KEY(Decal_Projector, CHANNELS_FLAG)
#endif

#if defined (PS_SHADER) && defined(TECHNIQUE)

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// 屏幕纹理坐标
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// 深度
	float4 vGeoTex 	= tex2D(sdGeomSampler, vUVSetScreenTex);
	float  fDepth	= UnpackDepth(vGeoTex.xy);

	// 反算观察坐标系坐标
	float3 vViewPos = kInput.vUVSetFarClipViewPos.xyz / kInput.vUVSetFarClipViewPos.w * fDepth;
	// @}
	
	// Projecting
	// @{
	// 从eye坐标系转回到projector投射坐标系
	float4 vDecalUVSet = mul(float4(vViewPos, 1.f), g_mViewToProjectorUV);
	vDecalUVSet /= vDecalUVSet.w;
	
	// 裁剪掉投影体之外的像素(只保留了[0,1][0,1][0,1]投影立方体)
	clip(vDecalUVSet.xyz);
	clip(1.f - vDecalUVSet.xyz);
	// @}
	
	// 采样FilterMap
#if LIGHTMAP
	float4 vLightColor = tex2D(sdDarkSampler, vDecalUVSet.xy);
	clip(vLightColor.a - g_fAlphaTestRef);
#endif

	// 计算Decal纹理坐标
//	float2 vCycle = 1.f / g_vProjectorUVSpeed;
//	float2 vTranslation = fmod(float2(g_fTime, g_fTime), vCycle) * g_vProjectorUVSpeed;
//	float2 vUVSetBase = (vDecalUVSet.xy + vTranslation) * g_vProjectorUVScale + g_vProjectorUVOffset;
	float2 vUVSetBase = vDecalUVSet.xy * g_vProjectorUVScale + g_vProjectorUVOffset;
	
	// 采样GlowMap/DiffuseMap
#if GLOWMAP
	float4 vDiffuseColor = tex2D(sdGlowSampler, vUVSetBase) * g_vDiffuseMaterial;
#elif DIFFUSEMAP
	float4 vDiffuseColor = tex2D(sdBaseSampler, vUVSetBase) * g_vDiffuseMaterial;
#endif

#if LIGHTMAP
	vDiffuseColor *= vLightColor;
#endif

	clip(vDiffuseColor.a - g_fAlphaTestRef);

#if GLOWMAP
	return vDiffuseColor;
#endif

	// 计算光照
#if DIFFUSEMAP
	#if NORMALMAP
		float3 vLocalNormal = (tex2D(sdNormalSampler, vUVSetBase).xyz - 0.5f) * float3(2.f, 2.f, -2.f);
		float3 vViewNormal = normalize(mul(vLocalNormal, g_mWorldView));
	#else
		float3 vViewNormal = UnpackNormal(vGeoTex.zw);
	#endif

	float3 vViewVec = -normalize(vViewPos);
	
	float3 vDiffuseLight;
	float3 vSpecularLight;
	float4 vLocalLight = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	AccumLighting(vViewVec, vViewNormal, g_vSpecularMaterial.a, 1.f, 1.f, vLocalLight, vDiffuseLight, vSpecularLight);
	
	float4 vColor;
	vColor.rgb = vDiffuseColor.rgb * vDiffuseLight + vSpecularLight * g_vSpecularMaterial.rgb;
	vColor.a = vDiffuseColor.a;
	
	return vColor;
#endif
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_SHADER ();
	}
}
//---------------------------------------------------------------------------------------
#endif