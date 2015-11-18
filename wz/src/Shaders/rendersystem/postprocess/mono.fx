//*************************************************************************************************
// 内容:	屏幕颜色单色化
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-09
// 最后修改:	2013-05-02
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 单色颜色
float3		a_vMonoColor	: ATTRIBUTE; 		

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
//SD_LINEAR_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);		// 屏幕颜色缓存

texture sdColorBuf <string NTM = "base";>;
sampler sdColorSampler = sampler_state
{
	Texture		= <sdColorBuf>;
	MINFILTER	= LINEAR;
	MIPFILTER	= NONE;
	MAGFILTER	= LINEAR;
	ADDRESSU	= CLAMP;
	ADDRESSV	= CLAMP;
	SRGB_DISABLE;
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
	//kOutput.vProjPos = float4(kInput.vPos, 1.f);
	kOutput.vProjPos = float4(kInput.vPos.xy, 1.f, 1.f);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样输入纹理
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// 计算灰度值
	float fLumin = dot(vColor.xyz, g_vLuminScale3);
	
	return float4(a_vMonoColor * fLumin, 1.0);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_Mono
<
	string Description = "PostEffect_Mono";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = false;     
		//ZWriteEnable = false;	
		//AlphaTestEnable = false;
		//AlphaBlendEnable = false;		
        //StencilEnable = false;
		//SRGBWriteEnable = false;
	}
}
//---------------------------------------------------------------------------------------