//*************************************************************************************************
// 内容:	Depth of Field
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 高斯过滤参数
float2		g_akDOFSamplerOffsets[9]	: GLOBAL;
float		g_akDOFSampleWeights[9]		: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
// 屏幕颜色纹理及采样器
texture2D sdColorTex 
<
	string NTM = "shader";
	int NTMIndex = 0;
>;

sampler2D sdColorSampler = sampler_state
{
    Texture 	= <sdColorTex>;
    AddressU  	= Clamp; 
	AddressV 	= Clamp;
	MinFilter 	= Linear; 
	MagFilter 	= Linear;
    SRGBTexture = false;
};

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
// 向下采样
float4 PS_Main_DownSample(VS_OUTPUT kInput) : COLOR0
{
	return tex2D(sdColorSampler, kInput.vUVSetScreenTex);
}
//---------------------------------------------------------------------------------------
// 单轴高斯过滤
float4 PS_Main_SeparableBlur(VS_OUTPUT kInput) : COLOR0
{
	// 单轴高斯模糊
	float4 vSample = 0.f;
	float4 vColor = 0.f;
	float2 vSamplePosition;
	for (int iSample = 0; iSample < 9; iSample++)
	{
		vSamplePosition = kInput.vUVSetScreenTex + g_akDOFSamplerOffsets[iSample];
		vColor = tex2D(sdColorSampler, vSamplePosition.xy);
		vSample += g_akDOFSampleWeights[iSample] * vColor;
	}
	
	return vSample;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_DOF_DownSample
<
	string Description = "PostEffect_DOF_DownSample";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_DownSample ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_DOF_SeparableBlur
<
	string Description = "PostEffect_DOF_SeparableBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_SeparableBlur ();
	}
}
//---------------------------------------------------------------------------------------
