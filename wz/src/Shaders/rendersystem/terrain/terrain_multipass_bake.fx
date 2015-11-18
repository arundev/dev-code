//*************************************************************************************************
// 内容:	地形烘焙LightMap(未完成,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4	g_vBakeUVOffsetScale	: GLOBAL = float4(0.f, 0.f, 1.f, 1.f);
float4	g_vTerrainUVOffsetScale	: GLOBAL = float4(0.f, 0.f, 1.f, 1.f);

float4 g_vRecipUVRepeats[3]	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_WRAP_SAMPLE(0, sdBaseNormalSampler, 	sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_LINEAR_WRAP_SAMPLE(1, sdBlendSampler,		sdBlendTex,			false);		// 地形混合权重贴图

SD_LINEAR_WRAP_SAMPLE( 2, sdDiffuseSampler00, sdDiffuseTex00, false)
SD_LINEAR_WRAP_SAMPLE( 3, sdDiffuseSampler01, sdDiffuseTex01, false)
SD_LINEAR_WRAP_SAMPLE( 4, sdDiffuseSampler02, sdDiffuseTex02, false)
SD_LINEAR_WRAP_SAMPLE( 5, sdDiffuseSampler03, sdDiffuseTex03, false)
SD_LINEAR_WRAP_SAMPLE( 6, sdDiffuseSampler04, sdDiffuseTex04, false)
SD_LINEAR_WRAP_SAMPLE( 7, sdDiffuseSampler05, sdDiffuseTex05, false)
SD_LINEAR_WRAP_SAMPLE( 8, sdDiffuseSampler06, sdDiffuseTex06, false)
SD_LINEAR_WRAP_SAMPLE( 9, sdDiffuseSampler07, sdDiffuseTex07, false)
SD_LINEAR_WRAP_SAMPLE(10, sdDiffuseSampler08, sdDiffuseTex08, false)
SD_LINEAR_WRAP_SAMPLE(11, sdDiffuseSampler09, sdDiffuseTex09, false)
SD_LINEAR_WRAP_SAMPLE(12, sdDiffuseSampler10, sdDiffuseTex10, false)
SD_LINEAR_WRAP_SAMPLE(13, sdDiffuseSampler11, sdDiffuseTex11, false)

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;
	float2	vUVSet		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float2 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(vPos.xy * g_vBakeUVOffsetScale.zw + g_vBakeUVOffsetScale.xy * 2.f, 1.f, 1.f);
	kOutput.vUVSet = vUVSet * g_vBakeUVOffsetScale.zw + g_vBakeUVOffsetScale.xy;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
float4 PS_Main_Planar_XY_Bake(VS_OUTPUT kInput) : COLOR0
{
	float2 vWorldPosXY = kInput.vUVSet * g_vTerrainSize;
	
	
	// NormalMap
	// @{
	// 根据UV采样NormalMap(Sampler是Linear,没有偏移半像素)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, kInput.vUVSet);
	
	// 解出倾斜情况
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vBaseNormalTex.z - vBaseNormalTex.w);	
	
	//clip(vPlanarWeight.z - 0.001f);
	// @}

	
	// BlendMap
	// @{
	// 计算UV
	float2 vUVSet1 = kInput.vUVSet * 0.5f;
	
	// 采样BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	
	// 采样BlendMap
	float4 vBaseDiffuseDarkColor = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.5f));

#ifdef _SD_EDITOR
	vBaseDiffuseDarkColor =  max(vBaseDiffuseDarkColor, float4(g_vVertexColorMask, g_fLightMapMask));
#endif
	// @}
	

	// NormalMap
	// @{
	// 计算UV
	float2 vUVSet2 = float2(vWorldPosXY.x, -vWorldPosXY.y);
	
	// 采样NormalMap
	float4 vDiffuseGloss = tex2D(sdDiffuseSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vDiffuseGloss += tex2D(sdDiffuseSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vDiffuseGloss += tex2D(sdDiffuseSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vDiffuseGloss += tex2D(sdDiffuseSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vDiffuseGloss += tex2D(sdDiffuseSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	vDiffuseGloss *= float4(vBaseDiffuseDarkColor.rgb, 1.f);
	// @}
	
	return float4(vDiffuseGloss.rgb, 0.f);
}
	
//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_XY_Bake
<
	string Description = "Terrain_AtlasShading_Planar_XY_Bake";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_XY_Bake ();
	}
}
/*
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_YZ_Bake
<
	string Description = "Terrain_AtlasShading_Planar_YZ_Bake";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_YZ_Bake ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_XY_Bake
<
	string Description = "Terrain_AtlasShading_Planar_XZ_Bake";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar_XZ_Bake ();
	}
}
//---------------------------------------------------------------------------------------
*/