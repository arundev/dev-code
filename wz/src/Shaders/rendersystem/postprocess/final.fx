//*************************************************************************************************
// 内容:	一些常用公共Pass
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-09
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
bool	g_bRedAlert			: GLOBAL = false;	// 红血屏幕效果
bool	g_bDeadEffect 		: GLOBAL = false;	// 死亡屏幕变灰效果

// Gamma值
float	g_fGammaCorrectionValue	: GLOBAL = 1.0;	

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdColorSampler,		sdColorTex,		false);
SD_LINEAR_CLAMP_SAMPLE(1,	sdRedAlertSampler,	sdRedAlertTex,	false);
 
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
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 简单拷贝
float4 PS_Main_Copy(VS_OUTPUT kInput) : COLOR0
{
	return tex2Dlod(sdColorSampler, float4(kInput.vUVSetScreenTex, 0.0, 0.0));
}
//---------------------------------------------------------------------------------------
// 拷贝到最终后台缓存
float4 PS_Main_CopyToBackBuffer(VS_OUTPUT kInput) : COLOR0
{
	// 采样基础纹理
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// 叠加红血屏幕效果(正弦规律变化)
	if (g_bRedAlert)
	{
		float4 vRedColor = tex2D(sdRedAlertSampler, kInput.vUVSetScreenTex);
		float fRadio = sin(5.f * g_fTime) * 0.3 + 0.7;
		vColor.rgb += float3(1.f, 0, 0) * vRedColor.a * fRadio;
	}
	
	// 叠加死亡变灰屏幕效果
	if (g_bDeadEffect)
	{
		vColor = dot(vColor.rgb, g_vLuminScale3);
	}
	
	// Gamma矫正
	//vColor = pow(saturate(vColor), 1.f / 2.2f);
	vColor = pow(saturate(vColor), 1.f / g_fGammaCorrectionValue);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 简单拷贝
technique PostEffect_Copy
<
	string Description = "PostEffect_Copy";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Copy ();
	}
}
//---------------------------------------------------------------------------------------
// 拷贝到后台缓存并合成
technique PostEffect_CopyToBackBuffer
<
	string Description = "PostEffect_CopyToBackBuffer";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_CopyToBackBuffer ();
	}
}
//---------------------------------------------------------------------------------------