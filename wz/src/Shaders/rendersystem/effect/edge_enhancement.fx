//*************************************************************************************************
// 内容:	勾边边缘增强
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:	2013-05-30
//*************************************************************************************************
#include "effect_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 勾边参数
// [boundcenter.x][boundcenter.y][boundcenter.z][boundradius]
// [edgecolor.r][edgecolor.g][edgecolor.b][edgewidth]
float	a_akEdgeParam[8]	: GLOBAL;
#define	a_vBoundCenter		float3(a_akEdgeParam[0], a_akEdgeParam[1], a_akEdgeParam[2])
#define	a_fBoundRadius		a_akEdgeParam[3]
#define	a_vEdgeColor		float3(a_akEdgeParam[4], a_akEdgeParam[5], a_akEdgeParam[6])
#define	a_fEdgeWidth		a_akEdgeParam[7]

// 采样权重与采样偏移
float2	a_akSampleOffsets[13]	: GLOBAL;
float	a_afSampleWeights[13]	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdMat0Sampler,	sdMat0Buf,	false);		///< 以前是SD_POINT_CLAMP_SAMPLE

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	///< 输入坐标
};
//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float4	vUVSetProjPos	: TEXCOORD0;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	//float3 vUpDirection 		= float3(0, 0, 1.f);
	//float3 vViewDirection 	= g_vCameraPostion - a_vBoundCenter;
	//float3 vRightDirection 	= cross(vUpDirection, vViewDirection);
	//vRightDirection = normalize(vRightDirection);
	//vUpDirection = normalize(cross(vViewDirection, vRightDirection));
	//vUpDirection = normalize(vUpDirection);

	// 生成一个以对象目标点为中心的Billborad对象
	float3 vOffset = kInput.vPos.x * g_vCameraRight.xyz + kInput.vPos.y * g_vCameraUp.xyz;
	float3 vWorldPos = a_vBoundCenter + a_fBoundRadius * 1.2f * vOffset;
	float4 vProjPos = mul(float4(vWorldPos, 1.f), g_mWorldViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetProjPos = vProjPos;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 计算屏幕纹理坐标
	//float2 vUVSetScreen = (kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w) * float2(0.5f, -0.5f) 
	//	+ float2(0.5f, 0.5f) + g_vHalfPixelOffset;	
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
//	return float4(1.f, 0, 0, 1.f);
//	return float4(vUVSetScreen.x, vUVSetScreen.y, 0, 1.f);
	return float4(tex2D(sdMat0Sampler, vUVSetScreen).rgb, 1.f);
	
	// 采样
	//
	//float fCenter = tex2D(sdMat0Sampler, vUVSetScreen).a;
	//float fLeft 	= tex2D(sdMat0Sampler, float2(vUVSetScreen.x - a_fDegeWidth * 2.f * g_vHalfPixelOffset.x, vUVSetScreen.y)).a;
	//float fRight 	= tex2D(sdMat0Sampler, float2(vUVSetScreen.x + a_fDegeWidth * 2.f * g_vHalfPixelOffset.x, vUVSetScreen.y)).a;
	//float fTop 	= tex2D(sdMat0Sampler, float2(vUVSetScreen.x, vUVSetScreen.y - a_fDegeWidth * 2.f * g_vHalfPixelOffset.y)).a;
	//float fBottom = tex2D(sdMat0Sampler, float2(vUVSetScreen.x, vUVSetScreen.y + a_fDegeWidth * 2.f * g_vHalfPixelOffset.y)).a;
	//float4 vFlag 	= float4(fLeft, fRight, fTop, fBottom);
	//
	//// old v1
	//float4 vLmAndEdge = vFlag * 255.f;
	//float4 vLightMap = 0.f;
	//float4 vEdgeSign = modf(vLmAndEdge / 2.f, vLightMap);
	//float fValue = dot(vEdgeSign, 1.f);
	//if (fValue == 0) 	clip(-1);
	//return float4(a_vEdgeColor, 1.f);
	//
	//// old v2
	//float fValue 	= (dot(vFlag, 1.f) + fCenter) / 5.f;
	//if (fValue == 0)	clip(-1);
	//return float4(a_vEdgeColor, 1.f);
	//	
	float fSample = 0.f;
	for (int i = 0; i < 13; ++i)
	{
		fSample += a_afSampleWeights[i] * tex2D(sdMat0Sampler, vUVSetScreen + a_akSampleOffsets[i] * a_fEdgeWidth).r;
	}
	
	if (fSample == 0)
		clip(-1);
		
//	return float4(a_vEdgeColor, fSample * 1.6f);
	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Effect_EdgeEnhancement
<
	string Description = "Effect_EdgeEnhancement";
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