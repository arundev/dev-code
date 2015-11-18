//*************************************************************************************************
// 内容:	Dof/GodRay/Bloom/HDR合成Pass
//		
//		这里有点类似UE3的UmbraShader,参考早期UDK的着色器
//		UDK-2010-07/Engine/Shaders/UberPostProcessBlendPixelShader.usf
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 效果开关情况
bool	g_bDof				: GLOBAL = false;
bool	g_bGodRay			: GLOBAL = false;
bool	g_bBloom			: GLOBAL = false;
bool	g_bUseToneMapping	: GLOBAL = false;

// GodRay
float3	g_vGodRayDirection	: GLOBAL = { 0.f, 0.f, 1.f};

// Bloom
float 	g_fBloomLightIntensity	: GLOBAL = 1.f;

// 
float	g_fSceneFading			: GLOBAL = 1.f;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdColorSampler,		sdColorTex,		false);	// 屏幕颜色纹理
SD_POINT_CLAMP_SAMPLE(1,	sdGeomSampler,		sdGeomTex,		false);	// 屏幕Dof-GodRay权重纹理
SD_LINEAR_CLAMP_SAMPLE(2,	sdDofSampler,		sdDofTex,		false);	
SD_LINEAR_CLAMP_SAMPLE(3,	sdGodRaySampler,	sdGodRayTex,	false);	
SD_LINEAR_CLAMP_SAMPLE(4,	sdBloomSampler,		sdbloomTex,		false);	

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点
	float2	vUVSet0			: TEXCOORD0;	// 屏幕矩形纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float4	vUVSetScreenTex		: TEXCOORD0;	// FullSize和QuadSize纠正半像素偏移的屏幕纹理坐标
	float	fFactor				: TEXCOORD1;	// GodRay参数
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// FullSize和QuadSize纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex.xy = kInput.vUVSet0 + g_vHalfPixelOffset;
	kOutput.vUVSetScreenTex.zw = kInput.vUVSet0 + g_vHalfPixelOffset * 4.f;
	
	// 计算GodRay的Factor
	float fDot = max(0.f, dot(g_vGodRayDirection, g_mInvView[2]));
	float fF = 0.5f;
	kOutput.fFactor = max(0, fDot - fF) / (1.f - fF); 
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样基础纹理
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex.xy);
	
	// 采样Dof-GodRay权重纹理
	float4 vPostWeight = tex2D(sdGeomSampler, kInput.vUVSetScreenTex.xy);
	
	// 合成Dof
	if (g_bDof)
	{
		float fCoc = vPostWeight.x;
		float3 vSmallBlurColor = tex2D(sdDofSampler, kInput.vUVSetScreenTex.xy + g_vHalfPixelOffset * 3.f).xyz;
		
		vColor.rgb = lerp(vColor.rgb, vSmallBlurColor, fCoc);
	}	
	
	// 合成GodRay
	if (g_bGodRay)
	{
		float3 vGodRayColor = tex2D(sdGodRaySampler, kInput.vUVSetScreenTex.zw);
		float fGodRayIntensity = kInput.fFactor;
		
		vColor.rgb = vColor.rgb * (1.f - fGodRayIntensity * 0.15f) + 
					 vGodRayColor * fGodRayIntensity * 1.2f;
	}
	
	// 合成Bloom
	if (g_bBloom)
	{
		float3 vBloomColor = tex2D(sdBloomSampler, kInput.vUVSetScreenTex.zw).xyz * g_fBloomLightIntensity;
		vColor.rgb = vColor.rgb + vBloomColor.rgb;
	}
	
	// 合成ToneMap
	if (g_bUseToneMapping)
	{
		vColor.rgb = vColor.rgb / (vColor.rgb + 0.187f) * 1.035f;
	}
	else
	{
		vColor.rgb = saturate(vColor.rgb);
	}
	
	return vColor * g_fSceneFading;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_Composite
<
	string Description = "PostEffect_Composite";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------