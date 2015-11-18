//*************************************************************************************************
// 内容:	Background Scroll
//				- 用于绘制地形外圈远景
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// [alpha][height][u_wrap][z_scale][v_bias]
float 		a_akBGScrollParams[5]	: ATTRIBUTE;
//#define 	a_fScrollAlpha		a_akBGScrollParams[0]
#define 	a_fScrollHeight		a_akBGScrollParams[1]
#define		a_fScrollUWrap		a_akBGScrollParams[2]
#define		a_dScrollZScale		a_akBGScrollParams[3]
//#define	a_dScrollVBias		a_akBGScrollParams[4]

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdScrollSampler, sdScrollTex, true);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 计算投影坐标
	kInput.vPos.z *= a_fScrollHeight;
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vProjPos.z *= a_dScrollZScale;
	
	// 计算UV
	kOutput.vUVSet0 = float2(kInput.vUVSet0.x * a_fScrollUWrap, kInput.vUVSet0.y);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vScrollColor = tex2D(sdScrollSampler, kInput.vUVSet0);
	return float4(lerp(vScrollColor.rgb, g_vFogColor, g_fFogDensity), vScrollColor.a);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Environment_BackgroundScroll
<
	string Description = "Environment_BackgroundScroll";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
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