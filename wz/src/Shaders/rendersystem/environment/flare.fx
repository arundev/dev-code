//*************************************************************************************************
// 内容:	Sun Flare
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-23
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// GodRay参数
float3	g_vGodRayDirection	: GLOBAL = { 0.f, 0.f, 1.f};	// 需要normalize的
float	g_fGodRayIntensity	: GLOBAL = 1.f;

//
float	a_fBoardDistance[7]	: ATTRIBUTE;
float	a_fTexCol[7]		: ATTRIBUTE;
float	a_fAlpha[7]			: ATTRIBUTE;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdFlareSampler,		sdFlareTex,		true);
SD_LINEAR_CLAMP_SAMPLE(1,	sdDepthSampler,		sdDepthBuf,		false);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float2	vUVSet0			: TEXCOORD0;	// 
	float2	vSunScreenPos	: TEXCOORD1;
	float	fAlpha			: TEXCOORD2;
	float	fIntensity		: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 计算Sun的投影空间位置
	float4 vWorldPos 	= float4(g_vCameraPostion + g_vGodRayDirection, 1.f);
	float4 vProjPos 	= mul(vWorldPos, g_mViewProj);
	
	// 计算Flare的投影空间起始位置和延伸方向
	float4 vFlareOrigin 	= vProjPos;
	float2 vFlareDirection 	= vProjPos.xy / vProjPos.w;
	
	// 计算Flare的屏幕空间起始位置
	float2 vCenter 			= vProjPos.xy / vProjPos.w * 0.5f + 0.5f;
	kOutput.vSunScreenPos 	= float2(vCenter.x, 1.f - vCenter.y);
	
	//
	kInput.vPos.x 		*= (g_vWindowSize.y / g_vWindowSize.x);
	kOutput.vProjPos.xy	= kInput.vPos.xy + vFlareDirection * a_fBoardDistance[int(kInput.vPos.z)] * 0.8f;
	kOutput.vProjPos.zw	= float2(1.f, 1.f);
	
	//
	float fU	= (sign(kInput.vPos.x) / 2.f + 0.5f + a_fTexCol[int(kInput.vPos.z)]) * 0.25f;
	float fV	= -sign(kInput.vPos.y) / 2.f + 0.5f;
	kOutput.vUVSet0 = float2(fU, fV);
	
	//
	kOutput.fIntensity = clamp(sign(g_fGodRayIntensity - 0.5f) * g_fGodRayIntensity, 0.f, 1.f);
	
	//
	float fFrontSide = sign(vFlareOrigin.w);
	float fAlpha = clamp(2.f - length(vFlareDirection), 0.f, 1.f);
	kOutput.fAlpha = fAlpha * fFrontSide * a_fAlpha[int(kInput.vPos.z)];
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	clip(kInput.fAlpha);
	
	float fDepth = tex2D(sdDepthSampler, kInput.vSunScreenPos).w;
	if (fDepth != 0.f)
		clip(-1);
		
	float4 vBaseColor = tex2D(sdFlareSampler, kInput.vUVSet0) * kInput.fAlpha * kInput.fIntensity;
	
	return vBaseColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Environment_Flare
<
	string Description = "Environment_Flare";
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
		//AlphaBlendEnable = true;
		//SrcBlend = One;
		//DestBlend = One;
	}
}
//---------------------------------------------------------------------------------------