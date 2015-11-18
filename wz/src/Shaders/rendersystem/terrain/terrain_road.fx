//*************************************************************************************************
// 内容:	terrain road, 贴地表的道路
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-23
// 最后修改:	
//*************************************************************************************************
#include "terrain_common.h"
#include "../_Deprecated/common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4	a_vRoadParam	: ATTRIBUTE;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdRoadBaseTex
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdRoadBaseSampler = sampler_state
{
	Texture = (sdRoadBaseTex);
	AddressU = WRAP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

texture sdRoadNormalTex
<
	string NTM = "normal";
	int NTMIndex = 0;
>;

sampler sdRoadNormalSampler = sampler_state
{
	Texture = (sdRoadNormalTex);
	AddressU = WRAP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

texture sdRoadGlossTex
<
	string NTM = "gloss";
	int NTMIndex = 0;
>;

sampler sdRoadGlossSampler = sampler_state
{
	Texture = (sdRoadGlossTex);
	AddressU = WRAP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

texture sdRoadLightTex
<
	string NTM = "dark";
	int NTMIndex = 0;
>;

sampler sdRoadLightSampler = sampler_state
{
	Texture = (sdRoadLightTex);
	AddressU = WRAP;
	AddressV = WRAP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

SD_POINT_CLAMP_SAMPLE(2, sdLightSampler, sdLightBuf,	false);		// 屏幕局部光照缓存

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos	: POSITION0;
	float2	vUVSet	: TEXCOORD0;
	float3	vNormal	: NORMAL0;	
};
//---------------------------------------------------------------------------------------
struct VS_INPUT_NORMAL
{
	float3	vPos		: POSITION0;
	float2	vUVSet		: TEXCOORD0;
	float3	vNormal		: NORMAL0;
	float3 	vBinormal	: BINORMAL0;
	float3	vTangent	: TANGENT0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION0;	
	float2	vUVSet				: TEXCOORD0;
	float4	vUVSetProjPos		: TEXCOORD1;
	float4	vUVSetViewPosAndFog	: TEXCOORD2;
	float3	vUVSetWorldPos		: TEXCOORD3;
	float3	vUVSetViewNrm		: TEXCOORD4;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_NORMAL
{
	float4	vProjPos			: POSITION0;	
	float2	vUVSet				: TEXCOORD0;
	float4	vUVSetProjPos		: TEXCOORD1;
	float4	vUVSetViewPosAndFog	: TEXCOORD2;
	float3	vUVSetWorldPos		: TEXCOORD3;
	float3	vUVSetViewNormal	: TEXCOORD4;
	float3	vUVSetViewBinormal	: TEXCOORD5;
	float3	vUVSetViewTangent	: TEXCOORD6;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Road(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	float4 vWorldPos = mul(float4(kInput.vPos, 1.f), g_mWorld);
	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetProjPos = vProjPos;
	kOutput.vUVSetWorldPos = vWorldPos.xyz;
	
	kOutput.vUVSetViewNrm = normalize(mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz);
	
	float4 vViewPos = mul(vWorldPos, g_mView);
	kOutput.vUVSetViewPosAndFog.xyz = vViewPos.xyz;
	kOutput.vUVSetViewPosAndFog.w = CalcFogFactors(vViewPos.xyz);
	
	kOutput.vUVSet.x = kInput.vUVSet.x * a_vRoadParam.x;
	kOutput.vUVSet.y = kInput.vUVSet.y ;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_NORMAL VS_Main_Road_Normal(VS_INPUT_NORMAL kInput)
{
	VS_OUTPUT_NORMAL kOutput = (VS_OUTPUT_NORMAL)0;
	
	float4 vWorldPos = mul(float4(kInput.vPos, 1.f), g_mWorld);
	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetProjPos = vProjPos;
	kOutput.vUVSetWorldPos = vWorldPos.xyz;
	
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, g_mWorldView,
		kOutput.vUVSetViewNormal, kOutput.vUVSetViewBinormal, kOutput.vUVSetViewTangent);
	
	float4 vViewPos = mul(vWorldPos, g_mView);
	kOutput.vUVSetViewPosAndFog.xyz = vViewPos.xyz;
	kOutput.vUVSetViewPosAndFog.w = CalcFogFactors(vViewPos.xyz);
	
	kOutput.vUVSet.x = kInput.vUVSet.x * a_vRoadParam.x;
	kOutput.vUVSet.y = kInput.vUVSet.y ;
	
	return kOutput;
}


//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
float4 PS_Main_Road(VS_OUTPUT kInput) : COLOR0
{
	// 
	float2 vUVSetBase = kInput.vUVSetWorldPos.xy * g_vRecipTerrainSize;
	float2 tileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - tileCenterOffset * g_fRecipBlendMapSize;
	float4 vLightTex = tex2D(sdRoadLightSampler, vUVSetLightMap);

	// 
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vLocalLightTex = tex2D(sdLightSampler, vUVSetScreen);
	
	float3 vViewPos = kInput.vUVSetViewPosAndFog.xyz;
	float3 vViewVec = normalize(-vViewPos);
	float3 vDiffuseLight = 0.f;
	float3 vSpecularLight = 0.f;
	AccumLighting(vViewVec, kInput.vUVSetViewNrm, g_fTerrainShiness, vLightTex.a, vLocalLightTex, vDiffuseLight, vSpecularLight);
	
	//
	float4 vBaseColor = tex2D(sdRoadBaseSampler, kInput.vUVSet) * float4(vLightTex.rgb, 1.f);
	
	float4 vFinalColor = 0.f;
	vFinalColor.rgb = vDiffuseLight * vBaseColor.rgb * g_vTerrainDiffuseMaterial + vSpecularLight * g_vTerrainSpecularMaterial;
	
	//
	float fFogFactor = kInput.vUVSetViewPosAndFog.w;
	vFinalColor.rgb = lerp(vFinalColor.rgb, g_vFogColor, fFogFactor);
	
	//
	float fK = a_vRoadParam.w / a_vRoadParam.y;	// 0.1f
	vFinalColor.a = vBaseColor.w * saturate((0.5f - abs((kInput.vUVSet.x / a_vRoadParam.x - 0.5f))) / fK);	// kInput.vColor.a
	
	return vFinalColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Road_NoLight(VS_OUTPUT kInput) : COLOR0
{
	// 
	//float2 vUVSetBase = kInput.vUVSetWorldPos.xy * g_vRecipTerrainSize;
	//float2 tileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	//float2 vUVSetLightMap = vUVSetBase - tileCenterOffset * g_fRecipBlendMapSize;
	//float4 vLightTex = tex2D(sdRoadLightSampler, vUVSetLightMap);
	float4 vLightTex = float4(1.f, 1.f, 1.f, 1.f);

	// 
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vLocalLightTex = tex2D(sdLightSampler, vUVSetScreen);
	
	//
	float3 vViewPos = kInput.vUVSetViewPosAndFog.xyz;
	float3 vViewVec = normalize(-vViewPos);
	float3 vDiffuseLight = 0.f;
	float3 vSpecularLight = 0.f;
	AccumLighting(vViewVec, kInput.vUVSetViewNrm, g_fTerrainShiness, vLightTex.a, vLocalLightTex, vDiffuseLight, vSpecularLight);
	
	//
	float4 vBaseColor = tex2D(sdRoadBaseSampler, kInput.vUVSet) * float4(vLightTex.rgb, 1.f);
	
	float4 vFinalColor = 0.f;
	vFinalColor.rgb = vDiffuseLight * vBaseColor.rgb * g_vTerrainDiffuseMaterial;// + vSpecularLight * g_vTerrainSpecularMaterial;
	
	//
	float fFogFactor = kInput.vUVSetViewPosAndFog.w;
	vFinalColor.rgb = lerp(vFinalColor.rgb, g_vFogColor, fFogFactor);
	
	//
	float fK = a_vRoadParam.w / a_vRoadParam.y;	// 0.1f
	vFinalColor.a = vBaseColor.w * saturate((0.5f - abs((kInput.vUVSet.x / a_vRoadParam.x - 0.5f))) / fK);	// kInput.vColor.a
	
	return vFinalColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Road_Normal(VS_OUTPUT_NORMAL kInput) : COLOR0
{
	// 
	float2 vUVSetBase = kInput.vUVSetWorldPos.xy * g_vRecipTerrainSize;
	float2 tileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - tileCenterOffset * g_fRecipBlendMapSize;
	float4 vLightTex = tex2D(sdRoadLightSampler, vUVSetLightMap);
	
	//
	float4 vNormalTex = 0.f;
	TextureRGBASample(kInput.vUVSet, sdRoadNormalSampler, bool(false), vNormalTex);
	
	float3 vViewNormal = 0.f;
	CalculateNormalFromColor(vNormalTex, 
		kInput.vUVSetViewNormal, kInput.vUVSetViewBinormal, kInput.vUVSetViewTangent, 
		int(0), vViewNormal);

	// 
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vLocalLightTex = tex2D(sdLightSampler, vUVSetScreen);
	
	//
	float3 vViewPos = kInput.vUVSetViewPosAndFog.xyz;
	float3 vViewVec = normalize(-vViewPos);
	float3 vDiffuseLight = 0.f;
	float3 vSpecularLight = 0.f;
	AccumLighting(vViewVec, vViewNormal, g_fTerrainShiness, vLightTex.a, vLocalLightTex, vDiffuseLight, vSpecularLight);
	
	//
	float4 vBaseColor = tex2D(sdRoadBaseSampler, kInput.vUVSet);// * float4(vLightTex.rgb, 1.f);
	
	float4 vFinalColor = 0.f;
	vFinalColor.rgb = vDiffuseLight * vBaseColor.rgb * g_vTerrainDiffuseMaterial + vSpecularLight * g_vTerrainSpecularMaterial;
	
	//
	float fFogFactor = kInput.vUVSetViewPosAndFog.w;
	vFinalColor.rgb = lerp(vFinalColor.rgb, g_vFogColor, fFogFactor);
	
	//
	float fK = a_vRoadParam.w / a_vRoadParam.y;	// 0.1f
	vFinalColor.a = vBaseColor.w * saturate((0.5f - abs((kInput.vUVSet.x / a_vRoadParam.x - 0.5f))) / fK);	// kInput.vColor.a
	
	return vFinalColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Road_Normal_Gloss(VS_OUTPUT_NORMAL kInput) : COLOR0
{
	// 
	float2 vUVSetBase = kInput.vUVSetWorldPos.xy * g_vRecipTerrainSize;
	float2 tileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - tileCenterOffset * g_fRecipBlendMapSize;
	float4 vLightTex = tex2D(sdRoadLightSampler, vUVSetLightMap);
	
	//
	float4 vNormalTex = 0.f;
	TextureRGBASample(kInput.vUVSet, sdRoadNormalSampler, bool(false), vNormalTex);
	
	float3 vViewNormal = 0.f;
	CalculateNormalFromColor(vNormalTex, 
		kInput.vUVSetViewNormal, kInput.vUVSetViewBinormal, kInput.vUVSetViewTangent, 
		int(0), vViewNormal);
		
	//
	float4 vGlossTex = 0.f;
	TextureRGBASample(kInput.vUVSet, sdRoadGlossSampler, bool(false), vGlossTex);	

	// 
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vLocalLightTex = tex2D(sdLightSampler, vUVSetScreen);
	
	//
	float3 vViewPos = kInput.vUVSetViewPosAndFog.xyz;
	float3 vViewVec = normalize(-vViewPos);
	float3 vDiffuseLight = 0.f;
	float3 vSpecularLight = 0.f;
	AccumLighting(vViewVec, vViewNormal, g_fTerrainShiness, vLightTex.a, vLocalLightTex, vDiffuseLight, vSpecularLight);
	
	//
	float4 vBaseColor = tex2D(sdRoadBaseSampler, kInput.vUVSet);// * float4(vLightTex.rgb, 1.f);
	
	float4 vFinalColor = 0.f;
	vFinalColor.rgb = vDiffuseLight * vBaseColor.rgb * g_vTerrainDiffuseMaterial + 
			vSpecularLight * g_vTerrainSpecularMaterial * vGlossTex.rgb;
	
	//
	float fFogFactor = kInput.vUVSetViewPosAndFog.w;
	vFinalColor.rgb = lerp(vFinalColor.rgb, g_vFogColor, fFogFactor);
	
	//
	float fK = a_vRoadParam.w / a_vRoadParam.y;	// 0.1f
	vFinalColor.a = vBaseColor.w * saturate((0.5f - abs((kInput.vUVSet.x / a_vRoadParam.x - 0.5f))) / fK);	// kInput.vColor.a
	
	return vFinalColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_Road
<
	string Description = "Terrain_Road";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_Road ();
		PixelShader 	= compile ps_2_a PS_Main_Road ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_Road_NoLight
<
	string Description = "Terrain_Road_NoLight";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_Road ();
		PixelShader 	= compile ps_2_a PS_Main_Road_NoLight ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_Road_Normal
<
	string Description = "Terrain_Road_Normal";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_Road_Normal ();
		PixelShader 	= compile ps_2_a PS_Main_Road_Normal ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_Road_Normal_Gloss
<
	string Description = "Terrain_Road_Normal_Gloss";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_Road_Normal ();
		PixelShader 	= compile ps_2_a PS_Main_Road_Normal_Gloss ();
	}
}
//---------------------------------------------------------------------------------------