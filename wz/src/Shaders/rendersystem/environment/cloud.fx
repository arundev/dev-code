//*************************************************************************************************
// 内容:	SkyDome Cloud
//				- 天空穹的云层
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// [cloud_wrap][cloud_speed][z-bias][alpha]
float4	a_vSkyDomeCloudParams	: ATTRIBUTE;
#define a_fSkyDomeCloudWrap		a_vSkyDomeCloudParams.x
#define	a_fSkyDomeCloudSpeed	a_vSkyDomeCloudParams.y
#define	a_fSkyDomeCloudAlpha	a_vSkyDomeCloudParams.w

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdCloudSampler, sdCloudTex, true);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos				: POSITION0;
	float3	vUVSetUVAndAlpha	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float3	vUVSetUVAndAlpha	: TEXCOORD0;	// UV坐标与透明度
	float3	vUVSetViewPos		: TEXCOORD1;	// 观察空间坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 计算投影坐标
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vUVSetViewPos = kOutput.vProjPos;
	kOutput.vProjPos.z *= 0.999999f;
	
	// 计算UV
	kOutput.vUVSetUVAndAlpha.xy = float2(kInput.vUVSetUVAndAlpha.xy * a_fSkyDomeCloudWrap * 0.5f);
	kOutput.vUVSetUVAndAlpha.xy += float2(g_fTime * 0.1f * a_fSkyDomeCloudSpeed, 0.f);
	
	// 透明度
	kOutput.vUVSetUVAndAlpha.z = kInput.vUVSetUVAndAlpha.z;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样云纹理
	float2 vUVSet0 = kInput.vUVSetUVAndAlpha.xy;
	float4 vCloudColor = tex2D(sdCloudSampler, vUVSet0);
	
	// 计算合成
	float fDensity = CalcSkyFogDensity(normalize(kInput.vUVSetViewPos));
	float3 vFinalColor = lerp(vCloudColor.rgb, g_vSkyFogColor, fDensity);
	
	float fAlpha = kInput.vUVSetUVAndAlpha.z;
	
	return float4(vFinalColor, vCloudColor.a * saturate(fAlpha / max(a_fSkyDomeCloudAlpha, 0.001f)));
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Environment_SkyDomeCloud
<
	string Description = "Environment_SkyDomeCloud";
	bool UsesNiRenderState = true;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//FillMode = Solid;
		//ZEnable = true;
		//ZFunc = LessEqual;
		//ZWriteenable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------