//*************************************************************************************************
// 内容:	Doodads Geometry着色器(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-24
// 最后修改:
//*************************************************************************************************
#include "DoodadsCommon.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
bool g_bGrassAnimationEnable	: GLOBAL;

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
	float3 	vNormal			: TEXCOORD1;
	
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
	float4	vProjPos			: POSITION;
	float3	vUVSetUVFadingRatio	: TEXCOORD0;
	float4 	vUVSetNormalDepth	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 像素着色器输入数据流
struct PS_INPUT
{
	float4	vProjPos			: POSITION;
	float3	vUVSetUVFadingRatio	: TEXCOORD0;
	float4 	vUVSetNormalDepth	: TEXCOORD1;
	float	fFace				: VFACE;		// 标记面片朝向
};

//---------------------------------------------------------------------------------------
// 像素着色器输出数据流
//---------------------------------------------------------------------------------------
struct PS_OUTPUT_MRT
{
	float4	vNormalDepth	: COLOR0;
	float4	vDiffuse		: COLOR1;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	// It may be optimized to :
	// floa3 vLocalPos.xyz = kInput.vPos + g_mWorld._m30_m31_m32;
	float3 vLocalPos = mul(float4(kInput.vPos, 1.f), g_mWorld).xyz;
	float3 vWorldPos;
	vWorldPos.x = dot(vLocalPos.xyz, kInput.vInstMatRow0.xyz);
	vWorldPos.y = dot(vLocalPos.xyz, kInput.vInstMatRow1.xyz);
	vWorldPos.z = dot(vLocalPos.xyz, kInput.vInstMatRow2.xyz);
	vWorldPos.xyz += kInput.vInstMatRow3.xyz;
	
	float fViewDistance = distance(vWorldPos, g_vCameraPostion.xyz);
	float fF1 = g_fGrassFadeInDistance - kInput.vInstMatRow1.w;
	float fF2 = g_fGrassFadeOutDistance - kInput.vInstMatRow2.w;
	float fFadingRatio = saturate((fViewDistance - fF1) / (fF2 - fF1));
	kOutput.vUVSetUVFadingRatio.z = fFadingRatio;

	if (g_bGrassAnimationEnable)
		vWorldPos= WindAnimation(kInput.vInstMatRow3.xyz, vWorldPos, max(0.f, 1.f - fFadingRatio * 10.f));
	
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);

	// It may be optimized to :
	//float3 vLocalNormal = kInput.vNormal;
	float3 vLocalNormal = normalize(mul(float4(kInput.vNormal, 0.f), g_mWorld).xyz);
	float3 vWorldNormal;
	vWorldNormal.x = dot(vLocalNormal, kInput.vInstMatRow0.xyz);
	vWorldNormal.y = dot(vLocalNormal, kInput.vInstMatRow1.xyz);
	vWorldNormal.z = dot(vLocalNormal, kInput.vInstMatRow2.xyz);	
	vWorldNormal = normalize(vWorldNormal);
	kOutput.vUVSetNormalDepth.xyz = normalize(mul(float4(vWorldNormal, 0.f), g_mView).xyz);

	kOutput.vUVSetNormalDepth.w = dot(float4(vWorldPos, 1.f), g_mView._m02_m12_m22_m32);
	
	kOutput.vUVSetUVFadingRatio.xy = kInput.vUVSet;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(PS_INPUT kInput) : COLOR0
{
	PS_OUTPUT_MRT kOutput = (PS_OUTPUT_MRT)0;

	float fAlpha = tex2D(sdBaseSampler, kInput.vUVSetUVFadingRatio.xy).a;
	clip(fAlpha - 0.5f);

	float fNoise = dot(tex2D(sdNoiseSampler, kInput.vUVSetUVFadingRatio.xy).rgb, 0.3333f);
	clip(fNoise - kInput.vUVSetUVFadingRatio.z);

	float fDepth = kInput.vUVSetNormalDepth.w / g_fCameraFarClip;
	float3 vNormal = kInput.vUVSetNormalDepth.xyz * sign(-kInput.fFace);

	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
PS_OUTPUT_MRT PS_Main_MRT(PS_INPUT kInput)
{
	PS_OUTPUT_MRT kOutput = (PS_OUTPUT_MRT)0;
	
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetUVFadingRatio.xy);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2D(sdNoiseSampler, kInput.vUVSetUVFadingRatio.xy).rgb, 0.3333f);
	clip(fNoise - kInput.vUVSetUVFadingRatio.z);
	
	float fDepth = kInput.vUVSetNormalDepth.w / g_fCameraFarClip;
	float3 vNormal = kInput.vUVSetNormalDepth.xyz * sign(-kInput.fFace);
	kOutput.vNormalDepth = float4(PackDepth(fDepth), PackNormal(vNormal));

	kOutput.vDiffuse = float4(vDiffuseTex.rgb, 0.f);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Doodads_Geometry_V1
<
	string Description = "Doodads_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();

		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------
technique Doodads_MRTGeometry_V1
<
	string Description = "Doodads_MRTGeometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_MRT ();
		
		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------