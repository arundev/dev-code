//*************************************************************************************************
// 内容:	Doodads Shading着色器(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-24
// 最后修改:
//*************************************************************************************************
#include "DoodadsCommon.h"
#include "../terrain/terrain_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2 	vUVSet			: TEXCOORD0;
	
	// 实例化数据
	float4	vInstMatRow0	: TEXCOORD2;	// Rotation X
	float4	vInstMatRow1	: TEXCOORD3;	// Rotation Y
	float4	vInstMatRow2	: TEXCOORD4;	// Rotation Z
	float4	vInstMatRow3	: TEXCOORD5;	// Translation Position
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float4	vUVSet			: TEXCOORD0;	// .xy diffuse tex coordinate, .zw terrain light map coordinate
	float4	vUVSetProjPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 像素着色器输入数据流
//---------------------------------------------------------------------------------------
#define PS_INPUT HEADER_KEY(PS_Input, CHANNELS_FLAG)
struct PS_INPUT
{
	float4	vProjPos		: POSITION;
	float4	vUVSet			: TEXCOORD0;	// .xy diffuse tex coordinate, .zw terrain light map coordinate
	float4	vUVSetProjPos	: TEXCOORD1;
	float	fFace			: VFACE;		// 标记面片朝向
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	float3 vLocalPos = mul(float4(kInput.vPos, 1.f), g_mWorld).xyz;
	float3 vWorldPos;
	vWorldPos.x = dot(vLocalPos.xyz, kInput.vInstMatRow0.xyz);
	vWorldPos.y = dot(vLocalPos.xyz, kInput.vInstMatRow1.xyz);
	vWorldPos.z = dot(vLocalPos.xyz, kInput.vInstMatRow2.xyz);
	vWorldPos.xyz += kInput.vInstMatRow3.xyz;
	
	float fViewDistance = distance(vWorldPos, g_vCameraPostion.xyz);
	float fFadingRatio = saturate((fViewDistance - g_fGrassFadeInDistance) / (g_fGrassFadeOutDistance - g_fGrassFadeInDistance));
	
	vWorldPos = WindAnimation(kInput.vInstMatRow3.xyz, vWorldPos, max(0.f, 1.f - fFadingRatio * 10.f));
	
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	kOutput.vUVSetProjPos = kOutput.vProjPos;

	//float3 vLocalNormal = mul(normalize(float4(1.f, 1.f, 1.f, 0.f)), g_mWorld).xyz;
	//float3 vWorldNormal;
	//vWorldNormal.x = dot(vLocalNormal, kInput.vInstMatRow0.xyz);
	//vWorldNormal.y = dot(vLocalNormal, kInput.vInstMatRow1.xyz);
	//vWorldNormal.z = dot(vLocalNormal, kInput.vInstMatRow2.xyz);	
	//vWorldNormal = normalize(vWorldNormal);

	kOutput.vUVSet.xy = kInput.vUVSet;
	kOutput.vUVSet.zw = vWorldPos.xy * g_vRecipTerrainSize;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(PS_INPUT kInput) : COLOR0
{
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vGeomTex = tex2D(sdGeomSampler, vUVSetScreen);
	float3 vViewNormal = UnpackNormal(vGeomTex.zw);
	
	float3 L = -g_vMainLightDir;
	float3 N = vViewNormal;
	float dotNL = 0.5f + 0.5f * dot(N, L);
	dotNL *= dotNL;
	// if (dotNL < 0.f) dotNL = dotNL * 0.7f + 0.3f;
	float NL = max(0.f, dotNL);
	
	float2 vUVSetBase = kInput.vUVSet.zw;
	float2 vTileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - vTileCenterOffset * g_fRecipBlendMapSize;
	float4 vLightTex = tex2D(sdBlendSampler, vUVSetLightMap);
	float3 vDiffuseLight = lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * vLightTex.rgb) * g_fGrassSunLightScalar;
	
	float4 vLocalLightTex = tex2D(sdLightSampler,vUVSetScreen);
	
	vDiffuseLight += vLocalLightTex.rgb;
	vDiffuseLight += g_vSkyAmbientColor * max(vLightTex.rgb, 0.5f) * g_fGrassAmbientScalar;
	
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSet.xy);
	
	return float4(vDiffuseLight * vDiffuseTex.rgb, 0.f);	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Doodads_Shading_V1
<
	string Description = "Doodads_Shading";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------
