//*************************************************************************************************
// 内容:	Color Grading
//
//	Color Grading
//	- Require a volume texture 16x16x16 unwrapped in 2d texture 256x16
//	- Use 256 * 16 LUT texture to remapping the LDR color.
//	- LDR color should range between 0-1
//	- Can be optimized by using a volume texture
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-13
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);		// 屏幕颜色缓存
SD_POINT_CLAMP_SAMPLE(1, sdWeightSampler,	sdWeightBuf,false);		// 屏幕权重缓存
SD_LINEAR_CLAMP_SAMPLE(2, sdColorGradingForeGroundLUTSampler, sdColorGradingForeGroundLUTTex, false);	// ColorGrading前景查找表纹理及其采样器
SD_LINEAR_CLAMP_SAMPLE(3, sdColorGradingBackgroundLUTSampler, sdColorGradingBackgroundLUTTex, false);	// ColorGrading背景查找表纹理及其采样器

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
// ColorGrading计算
float3 ColorGrading(float3 vInLDRColor)
{
	// A 2d LUT texture 256x16
	// 计算纹理像素偏移与缩放因子
	float2 vOffset = float2(0.5 / 256.0, 0.5 / 16.0);
	float fScale = 15.0 / 16.0;
	
	// Also consider blur value in the blur buffer written by translucency
	float fIntB = floor(vInLDRColor.b / 14.9999) / 16.0;
	float fFracB = (vInLDRColor.b - fIntB) * 16.0;
	
	float fU = fIntB + vInLDRColor.r * fScale / 16.0;
	float fV = vInLDRColor.g * fScale;
	
	float3 vRG0 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG1 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;
	
	return lerp(vRG0, vRG1, fFracB);
}
//---------------------------------------------------------------------------------------
// 基于深度的ColorGrading计算(对背景和前景使用不同的LUT)
float3 ColorGradingDepth(float3 vInLDRColor, float fDepth)
{
	// A 2d LUT texture 256x16
	float2 vOffset = float2(0.5 / 256.0, 0.5 / 16.0);
	float fScale = 15.0 / 16.0;
	
	// Also consider blur value in the blur buffer written by translucency
	float fIntB = floor(vInLDRColor.b / 14.9999) / 16.0;
	float fFracB = (vInLDRColor.b - fIntB) * 16.0;
	
	float fU = fIntB + vInLDRColor.r * fScale / 16.0;
	float fV = vInLDRColor.g * fScale;
	
	float3 vRG0 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG1 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;	
	float3 vRG2 = tex2Dlod(sdColorGradingBackgroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG3 = tex2Dlod(sdColorGradingBackgroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;
	
	return lerp(lerp(vRG0, vRG1, fFracB), lerp(vRG2, vRG3, fFracB), saturate(fDepth));
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样输入纹理
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// 恢复映射系数
	float4 vPostWeightMap = tex2D(sdWeightSampler, kInput.vUVSetScreenTex);
	float fT = dot(vPostWeightMap.yz, float2(255.0, 1.0)) * (1.0 / 256.0);
	
	// 计算映射
	//vColor.rgb = ColorGrading(saturate(vColor.rgb));
	vColor.rgb = ColorGradingDepth(saturate(vColor.rgb), fT);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_ColorGrading
<
	string Description = "PostEffect_ColorGrading";
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