//*************************************************************************************************
// 内容:	Doodads Shading着色器
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
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// 当前点对应远裁剪面上的点的世界坐标
	float3	vUVFarClipViewPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的观察坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(vPos, 1.f);
	kOutput.vUVSetScreenTex = vUVSet + g_vHalfPixelOffset;
	kOutput.vUVFarClipWorldPos = mul(float4(vPos.xy, 1.f, 1.f), g_mDepthToWorld).xyz;
	kOutput.vUVFarClipViewPos  = mul(float4(kOutput.vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vGeomTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float  fDepth = UnpackDepth(vGeomTex.xy);
	float3 vViewNormal = UnpackNormal(vGeomTex.zw);
	
	float3 L = -g_vMainLightDir;
	float3 N = vViewNormal;
	float dotNL = 0.5f + 0.5f * dot(N, L);
	dotNL *= dotNL;
	// if (dotNL < 0.f) dotNL = dotNL * 0.7f + 0.3f;
	float NL = max(0.f, dotNL);
	
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	float2 vUVSetBase = vWorldPos.xy * g_vRecipTerrainSize;
	float2 vTileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - vTileCenterOffset * g_fRecipBlendMapSize;
	float4 vLightTex = tex2D(sdBlendSampler, vUVSetLightMap);
	float3 vDiffuseLight = lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * vLightTex.rgb) * g_fGrassSunLightScalar;
	
	float4 vLocalLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	vDiffuseLight += vLocalLightTex.rgb;
	vDiffuseLight += g_vSkyAmbientColor * max(vLightTex.rgb, 0.5f) * g_fGrassAmbientScalar;
	
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetScreenTex);
	
	return float4(vDiffuseLight * vDiffuseTex.rgb, 0.f);	
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NoLightMap(VS_OUTPUT kInput) : COLOR0
{
	float4 vGeomTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float  fDepth = UnpackDepth(vGeomTex.xy);
	float3 vViewNormal = UnpackNormal(vGeomTex.zw);
	
	float3 L = -g_vMainLightDir;
	float3 N = vViewNormal;
	float dotNL = 0.5f + 0.5f * dot(N, L);
	dotNL *= dotNL;
	// if (dotNL < 0.f) dotNL = dotNL * 0.7f + 0.3f;
	float NL = max(0.f, dotNL);
	
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	float2 vUVSetBase = vWorldPos.xy * g_vRecipTerrainSize;
	float2 vTileCenterOffset = frac(vUVSetBase * g_fTileMapSize) - 0.5f;
	float2 vUVSetLightMap = vUVSetBase - vTileCenterOffset * g_fRecipBlendMapSize;
	float3 vDiffuseLight = lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL) * g_fGrassSunLightScalar;
	
	float4 vLocalLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	vDiffuseLight += vLocalLightTex.rgb;
	vDiffuseLight += g_vSkyAmbientColor * g_fGrassAmbientScalar;
	
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetScreenTex);
	
	return float4(vDiffuseLight * vDiffuseTex.rgb, 0.f);	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Doodads_Shading
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
technique Doodads_Shading_NoLightMap
<
	string Description = "Doodads_Shading_NoLightMap";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_NoLightMap ();
	}
}
//---------------------------------------------------------------------------------------
