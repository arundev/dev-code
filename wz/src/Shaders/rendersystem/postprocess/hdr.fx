//*************************************************************************************************
// 内容:	HDR
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-23
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 源Texture半像素偏移,用于向下采样,不同分辨率值不同
float2 	g_vHDRHalfPixelOffset	: GLOBAL;

// 目标Target尺寸
float2 	g_vHDRScreenMapSize		: GLOBAL;

// 1x4像素偏移
#define OFFSET_SAMPLE_COUNT 4
static const float s_fOffsets[OFFSET_SAMPLE_COUNT] = {-1.5f, -0.5f, 0.5f, 1.5f};

// 二维3x3高斯算子像素偏移与权重
#define BLUR_SAMPLE_COUNT 9
static const float2 s_vBlurOffsets[BLUR_SAMPLE_COUNT] =
{
	{-1, -1}, {0, -1}, {1, -1},
	{-1, 0},  {0, 0},  {1, 0},
	{-1, 1},  {0, 1},  {1, 1},
};

static const float s_fBlurWeights[BLUR_SAMPLE_COUNT] =
{
	1.f/16.f,	2.f/16.f,	1.f/16.f,
	2.f/16.f,	2.f/16.f,	2.f/16.f,
	1.f/16.f,	2.f/16.f,	1.f/16.f,
};

// 自适应亮度系数
// 1.f - exp(-deltaTime * adaptationRate);
float	g_fAdaptedParam			: GLOBAL;	

// Bloom阈值过滤参数
// x: maxLuminance, y: toneMapKey, z: bloomThreshold, w: minAverageLuminance
float4	g_vBloomThresholdParams	: GLOBAL;	

// Bloom一维高斯过滤参数
float2	g_akHDRSamplerOffsets[9]	: GLOBAL;
float	g_akHDRSampleWeights[9]		: GLOBAL;

// ToneMapping映射参数
// x: maxLuminance * maxLuminance, y: toneMapKey, z: bloomIntensity, w: minAverageLuminance
float4	g_vToneMapParams		: GLOBAL; 	
//float g_fToneMapBrilliance	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdColorSampler,					sdColorBuf,					false);
SD_LINEAR_CLAMP_SAMPLE(1,	sdBloomSampler,					sdBloomBuf,					false);
SD_POINT_CLAMP_SAMPLE(2,	sdCurLuminanceSampler,			sdCurLuminanceBuf,			false);
SD_POINT_CLAMP_SAMPLE(3,	sdCurAdaptedLuminanceSampler,	sdCurAdaptedLuminanceBuf,	false);
SD_POINT_CLAMP_SAMPLE(4,	sdLastAdaptedLuminanceSampler,	sdLastAdaptedLuminanceBuf,	false);

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
	float2	vUVSetScreenTex		: TEXCOORD0;	// FullSize纠正半像素偏移的屏幕纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// FullSize纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHDRHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 向下采样
float4 PS_Main_HDR_DownScale(VS_OUTPUT kInput) : COLOR0
{
	return tex2D(sdColorSampler, kInput.vUVSetScreenTex);
}
//---------------------------------------------------------------------------------------
// 计算每像素亮度
float4 PS_Main_HDR_SceneLuminance(VS_OUTPUT kInput) : COLOR0
{
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	float fLuminance = dot(vColor, g_vLuminScale3);
	float fLogLuminance = log(fLuminance + 0.00001f);
	
	return float4(fLogLuminance, 0, 0, 0);
}
//---------------------------------------------------------------------------------------
// 
float4 PS_Main_HDR_DownScaleLuminance(VS_OUTPUT kInput) : COLOR0
{
	float4 vLuminance = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			vLuminance += tex2D(sdColorSampler, kInput.vUVSetScreenTex + float2(s_fOffsets[i], s_fOffsets[j]) * g_vHDRScreenMapSize);
			
	vLuminance /= 16.0f;
	return vLuminance;
}
//---------------------------------------------------------------------------------------
// 
float4 PS_MainHDR_FinalDownScaleLuminance(VS_OUTPUT kInput) : COLOR0
{
	float4 vLuminance = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			vLuminance += tex2D(sdColorSampler, kInput.vUVSetScreenTex + float2(s_fOffsets[i], s_fOffsets[j]) * g_vHDRScreenMapSize);
			
	vLuminance /= 16.0f;
	return float4(exp(vLuminance.r), 0, 0, 0);
}
//---------------------------------------------------------------------------------------
// 采样当前亮度
float4 PS_Main_HDR_InitLuminance(VS_OUTPUT kInput) : COLOR0
{
	float fLuminance = tex2D(sdCurLuminanceSampler, float2(0.5f, 0.5f)).r;
	return float4(fLuminance, 0, 0, 0);
}
//---------------------------------------------------------------------------------------
// 
float4 PS_Main_HDR_AdaptedLuminance(VS_OUTPUT kInput) : COLOR0
{
	float fLastAdaptedLuminance = tex2D(sdLastAdaptedLuminanceSampler, float2(0.5f, 0.5f)).r;
	float fCurLuminance = tex2D(sdCurLuminanceSampler, float2(0.5f, 0.5f)).r;
	float fAdaptedLuminance = fLastAdaptedLuminance + (fCurLuminance - fLastAdaptedLuminance) * g_fAdaptedParam;
	return float4(fAdaptedLuminance, 0, 0, 0);
}
//---------------------------------------------------------------------------------------
// Bloom高亮区域提取
float4 PS_Main_HDR_BloomThreshold(VS_OUTPUT kInput) : COLOR0
{
	// 采样亮度纹理,获取当前场景平均亮度
	float fAverageLuminance = tex2D(sdCurAdaptedLuminanceSampler, float2(0.5f, 0.5f)).r;
	fAverageLuminance = max(fAverageLuminance, g_vBloomThresholdParams.w);

	// 采样屏幕纹理,计算当前像素亮度
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	float fLuminance = dot(vColor, g_vLuminScale3);
	
	//
	float fScaled = (fLuminance * g_vBloomThresholdParams.y) / fAverageLuminance;
	float fCompressed = (fScaled * (1.f + (fScaled / g_vBloomThresholdParams.x))) / (1 + fScaled);
	
	float3 vSample = fCompressed * vColor;
	vSample -= g_vBloomThresholdParams.z;
	vSample = max(vSample, 0);
	
	return float4(vSample, 1.f);
}
//---------------------------------------------------------------------------------------
// 3x3高斯过滤
float4 PS_Main_HDR_BloomBlur(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = 0.f;
	for (int i= 0; i < BLUR_SAMPLE_COUNT; ++i)
		vColor += s_fBlurWeights[i] * tex2D(sdColorSampler, kInput.vUVSetScreenTex + s_vBlurOffsets[i] * g_vHDRScreenMapSize);
		
	return vColor;
}
//---------------------------------------------------------------------------------------
// 1x9单轴高斯模糊
float4 PS_Main_HDR_SeparableBlur(VS_OUTPUT kInput) : COLOR0
{
	float4 vSample = 0.f;
	float4 vColor = 0.f;
	float2 vSamplePosition;
	for (int iSample = 0; iSample < 9; iSample++)
	{
		vSamplePosition = kInput.vUVSetScreenTex + g_akHDRSamplerOffsets[iSample];
		vColor = tex2D(sdColorSampler, vSamplePosition.xy);
		vSample += g_akHDRSampleWeights[iSample] * vColor;
	}
	
	return vSample;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_HDR_ToneMap(VS_OUTPUT kInput) : COLOR0
{
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	vColor = pow(max(vColor, 0.f), 2.2f);
	
	float fAverageLuminance = tex2D(sdCurAdaptedLuminanceSampler, float2(0.5f, 0.5f)).r;
	fAverageLuminance = max(fAverageLuminance, g_vBloomThresholdParams.w);
	
	float3 vYxy = RGB2Yxy(vColor);
	float fScaled = (vYxy.r * g_vToneMapParams.y) / (fAverageLuminance + 0.00001f);
	vYxy.r = (fScaled * (1.f + (fScaled / g_vToneMapParams.x))) / (1.f + fScaled);
	vColor = Yxy2RGB(vYxy);
	
	// filmic tonemapping
	//
	//float3 vToneMapColor = vColor + tex2D(sdBloomSampler, kInput.vUVSetScreenTex).rgb * g_vToneMapParams.z;
	//
	half3 t1 = 6.2f * vColor * vColor;
	float cc = 0.8f;
	half3 t2 = cc * vColor;
	float3 vToneMapColor = (t1 + t2) / (t1 + 4.1f * t2 + 0.05f) + 
				(0.634f * cc - 0.247) * vColor +
				tex2D(sdBloomSampler, kInput.vUVSetScreenTex).rgb * g_vToneMapParams.z;
	return float4(vToneMapColor, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_DownScale
<
	string Description = "PostEffect_HDR_DownScale";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_DownScale ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_SceneLuminance
<
	string Description = "PostEffect_HDR_SceneLuminance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_SceneLuminance ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_DownScaleLuminance
<
	string Description = "PostEffect_HDR_DownScaleLuminance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_DownScaleLuminance ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_FinalDownScaleLuminance
<
	string Description = "PostEffect_HDR_FinalDownScaleLuminance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_MainHDR_FinalDownScaleLuminance ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_InitLuminance
<
	string Description = "PostEffect_HDR_InitLuminance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_InitLuminance ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_AdaptedLuminance
<
	string Description = "PostEffect_HDR_AdaptedLuminance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_AdaptedLuminance ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_BloomThreshold
<
	string Description = "PostEffect_HDR_BloomThreshold";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_BloomThreshold ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_BloomBlur
<
	string Description = "PostEffect_HDR_BloomBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_BloomBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_SeparableBlur
<
	string Description = "PostEffect_HDR_SeparableBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_SeparableBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_HDR_ToneMap
<
	string Description = "PostEffect_HDR_ToneMap";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_HDR_ToneMap ();
	}
}
//---------------------------------------------------------------------------------------