//*************************************************************************************************
// 内容:	SSAO
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-24
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 半像素偏移,用于向下采样,不同分辨率值不同
float2 	g_vSSAOHalfPixelOffset	: GLOBAL;

// Occlusion buffer size / RandomNormalTexture size
float4	g_vSSAONoiseScale	: GLOBAL;

// Transforms a position in NDC space [-1,1] to screen space,
// taking into account a half pixel offset for D3D
float4	g_vSSAOScreenPositionScaleBias	: GLOBAL;

// X and Y scaling factors from the world DPG projection matrix multiplied  
// with g_vSSAOScreenPositionScaleBias.xy
float2 	g_vSSAOProjectionScale	: GLOBAL;

// This frame's projection matrix
float4x4	g_vSSAOProjectionMatrix	: GLOBAL;

// Occlusion Radius, <unused>, aloDistanceThreshold, HaloOcclusion
float4 	g_vSSAOOcclusionCalcParameters	: GLOBAL;

// Amount that distance should affect g_vSSAOOcclusionCalcParameters
float	g_fSSAOHaloDistanceScale	: GLOBAL;

// OcclusionPower, OcclusionScale, OcclusionBias, MinOcclusion
float4	g_vSSAOOcclusionRemapParameters	: GLOBAL;

// OcclusionFadeoutMinDistance, 1.f / (OcclusionFadeoutMaxDistance - OcclusionFadeoutMinDistance)
float4	g_vSSAOOcclusionFadeoutParameters	: GLOBAL;

// Transforms a fixed screen space radius ino view space along the x axis 
// when mutiplied with view space
float	m_fSSAOMaxRadiusTransform	: GLOBAL;

// Nicely distributed points with distance from 1/8 to 8/8
#define OCCLUSION_SAMPLE_COUNT 8
static float3 s_akSSAOOcclusionSampleOffsets[OFFSET_SAMPLE_COUNT] = 
{
	// GA optimized set of points in a sphere pow 0.5f distribution -6.939256f
	// (point distributed ovew the volumen) 
	float3(0.068099,  -0.029749,  0.345655),
	float3(-0.333219, -0.031481, -0.371448),
	float3(0.484993,  -0.106742,  0.358312),
	float3(0.140918,   0.672336, -0.167649),
	float3(0.005538,  -0.785597,  0.088357),
	float3(-0.633421,  0.527250,  0.266055),
	float3(-0.744960, -0.029749,  0.330861),
	float3(0.068099,  -0.029749,  0.295312),
};

// SSAO Blur参数
// @{
// x : EdgeDistanceThreshold, 
// y : EdgeDistanceScale
float4 	g_vSSAOBlurParameters	: GLOBAL;

// Sample offset
#define BLUR_SAMPLE_COUNT 6
float4	g_vBlurSampleOffsets[BLUR_SAMPLE_COUNT/2] : GLOBAL;
// @}


//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdDepthSampler,			sdDepthBuf,			false);
SD_LINEAR_CLAMP_SAMPLE(1,	sdAOSampler,			sdAOTex,			false);

// RGBA8 linear texture containing random normals
SD_POINT_WRAP_SAMPLE(2, 	sdRandomNormalSampler,	sdRandomNormalTex,	false);








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
//
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SAMPLEDEPTH
{
	float4	vProjPos	: POSITION;
	float2	vUVSet0		: TEXCOORD0;	
	float2	vUVSet1		: TEXCOORD1;
	float2	vUVSet2		: TEXCOORD2;	
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_OCCLUSION
{
	float4	vProjPos				: POSITION;
	float2	vUVSet0					: TEXCOORD0;
	float4	vUVSetViewScreenVector	: TEXCOORD1;	
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_BLUR
{
	float4	vProjPos				: POSITION;
	float2	vUVSet0					: TEXCOORD0;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_QUAD
{
	float4	vProjPos				: POSITION;
	float2	vUVSetScreenTex			: TEXCOORD0;
	float3	vUVSetFarWorldPos		: TEXCOORD1;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT_SAMPLEDEPTH VS_Main_SSAO_DownScale(VS_INPUT kInput)
{
	VS_OUTPUT_SAMPLEDEPTH kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// Offsets around the actual textre coordiate to get a representation of neighbor depths
	kOutput.vUVSet0	= kInput.vUVSet0 + g_vSSAOHalfPixelOffset;
	kOutput.vUVSet1	= kOutput.vUVSet0 + float2(g_vSSAOHalfPixelOffset.x, 0);
	kOutput.vUVSet2	= kOutput.vUVSet0 + float2(0, g_vSSAOHalfPixelOffset.x);
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_OCCLUSION VS_Main_SSAO_Occlusion(VS_INPUT kInput)
{
	VS_OUTPUT_OCCLUSION kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	//
	kOutput.vUVSet0	= kInput.vUVSet0 + g_vSSAOHalfPixelOffset;
	
	// deproject to view space
	kOutput.vUVSetViewScreenVector = mul(float4(kInput.vPos, 1.f, 1.f), g_mDepthToView);
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_BLUR VS_Main_SSAO_Blur(VS_INPUT kInput)
{
	VS_OUTPUT_BLUR kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	//
	kOutput.vUVSet0	= kInput.vUVSet0 + g_vSSAOHalfPixelOffset;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_QUAD VS_Main_SSAO_Quad(VS_INPUT kInput)
{
	VS_OUTPUT_QUAD kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	//
	kOutput.vUVSetScreenTex	= kInput.vUVSet0 + g_vSSAOHalfPixelOffset;
	
	//
	kOutput.vUVSetFarWorldPos = mul(float4(kInput.vPos, 1.f, 1.f), g_mDepthToWorld);
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float CalcSceneDepth(float2 vUVSet)
{
	float2 vPackedDepth = tex2D(sdDepthSampler, vUVSet).xy;
	float fDepth = UnpackDepth(vPackedDepth);
	return fDepth;
}
//---------------------------------------------------------------------------------------
// 这里考虑压缩为两个16位
float4 OutputOcclusionAndDepth(float fOcclusion, float fDepth)
{
	return float4(fOcclusion, fDepth, 0, 0);
}
//---------------------------------------------------------------------------------------
float CalcOcclusion(float2 vUVSet)
{
	return tex2D(sdAOSampler, vUVSet).x;
}
//---------------------------------------------------------------------------------------
float CalcDepth(float2 vUVSet)
{
	return tex2D(sdAOSampler, vUVSet).y;
}
//---------------------------------------------------------------------------------------
float2 CalcOcclusionAndDepth(float2 vUVSet)
{
	return tex2D(sdAOSampler, vUVSet).xy;
}
//---------------------------------------------------------------------------------------
// Get 4-neighbour pixel and use the max as down-sampled depth
float4 PS_Main_SSAO_DownScale(VS_OUTPUT_SAMPLEDEPTH kInput) : COLOR0
{
	// 获取4邻域深度
	float fDepth0 = CalcSceneDepth(kInput.vUVSet0);
	float fDepth1 = CalcSceneDepth(kInput.vUVSet1);
	float fDepth2 = CalcSceneDepth(kInput.vUVSet2);
	float fDepth3 = CalcSceneDepth(kInput.vUVSet1 + kInput.vUVSet2 - kInput.vUVSet0);
	
	// 以最小深度作为遮挡,以最大深度作为被遮挡
	float fOcclusion = min(min(fDepth0, fDepth1), min(fDepth2, fDepth3)) * g_fCameraFarClip * 100.f;
	float fDepth = max(max(fDepth0, fDepth1), max(fDepth2, fDepth3)) * g_fCameraFarClip * 100.f;
	
	return OutputOcclusionAndDepth(fOcclusion, fDepth);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_SSAO_Occlusion(VS_OUTPUT_OCCLUSION kInput) : COLOR0
{
	// 
	float fDepth = CalcDepth(kInput.vUVSet0);
	
	// Calculate view space position of the current pixel
	float3 vViewSpacePosition = kInput.vUVSetViewScreenVector.xyz / vUVSetViewScreenVector.w * fDepth / g_fCameraFarClip;
	
	// Get a random normal for this pixel
	float3 vRandomNormal = tex2Dlod(sdRandomNormalSampler, float4(kInput.vUVSet0 * g_vSSAONoiseScale.xy, 0, 0)).xyz;
	vRandomNormal = vRandomNormal * 2.f - 1.f;
	
	float3 vRandomNormalX2 = vRandomNormal * 2.f;
	
	//
	float fRadius = min(m_fSSAOMaxRadiusTransform, fDepth, g_vSSAOOcclusionCalcParameters.x);
	
	//
	float fHaloDistanceThreshold = -  g_vSSAOOcclusionCalcParameters.z - g_fSSAOHaloDistanceScale * fDepth;
	
	// Occlusion factor for this pixel, where 0 is completedly unoccluded and 1 is fully occluded
	float fOccludedPercent = 0.f;
	
	[unroll]
	for (int i = 0; i < OCCLUSION_SAMPLE_COUNT; ++i)
	{
		// Operate on 4 samples at a time to take advantage of vectorized instructions.
		// Reflect each samplke across a random normal, scale by radius,
		// This is necessary to hide the low sample count and view dependence of the results.
		// Calculate view space sample position
		// \Todo - calculate sample position in screenspace to save instructions
		
		float3 vSamplePosition0 = vViewSpacePosition + (s_akSSAOOcclusionSampleOffsets[i] - dot(s_akSSAOOcclusionSampleOffsets[i], vRandomNormal) * vRandomNormalX2) * fRadius.xxx;
		float3 vSamplePosition1 = vViewSpacePosition + (s_akSSAOOcclusionSampleOffsets[i+1] - dot(s_akSSAOOcclusionSampleOffsets[i+1], vRandomNormal) * vRandomNormalX2) * fRadius.xxx;
		float3 vSamplePosition2 = vViewSpacePosition + (s_akSSAOOcclusionSampleOffsets[i+2] - dot(s_akSSAOOcclusionSampleOffsets[i+2], vRandomNormal) * vRandomNormalX2) * fRadius.xxx;
		float3 vSamplePosition3 = vViewSpacePosition + (s_akSSAOOcclusionSampleOffsets[i+3] - dot(s_akSSAOOcclusionSampleOffsets[i+3], vRandomNormal) * vRandomNormalX2) * fRadius.xxx;

		float2 vInvSampleZ01 = 1.f / float2(vSamplePosition0.z, vSamplePosition01.z);
		float2 vInvSampleZ23 = 1.f / float2(vSamplePosition2.z, vSamplePosition03.z);

		// Transform the positions into screen space
		//
		// Optimized path for normal projection matrices which only scale x and y
		float4 vSampleCoords01 = float4(vSamplePosition0.xy, vSamplePosition1.xy) * g_vSSAOProjectionScale.xyxy * vInvSampleZ01.xyxy + g_vSSAOScreenPositionScaleBias.zwzw;
		float4 vSampleCoords23 = float4(vSamplePosition2.xy, vSamplePosition3.xy) * g_vSSAOProjectionScale.xyxy * vInvSampleZ23.xyxy + g_vSSAOScreenPositionScaleBias.zwzw;
	
		float2 vSample0 = CalcOcclusionAndDepth(vSampleCoords01.xy);
		float2 vSample1 = CalcOcclusionAndDepth(vSampleCoords01.zw);
		float2 vSample2 = CalcOcclusionAndDepth(vSampleCoords23.xy);
		float2 vSample3 = CalcOcclusionAndDepth(vSampleCoords23.zw);
		
		// View space z of the nearest opaque occluder at the sample positions
		float4 vOccluderDepths = float4(vSample0.y, vSample1.y, vSample2.y, vSample3.y);
		
		// Difference in view space z between the samples and the nearest opaque occluder
		float4 fDepthDeltas = vOccluderDepths - float4(vSample0.z, vSample1.z, vSample2.z, vSample3.z);
		
		// Soft comparison (instead of DepthDeltas < 0.f), tweaked constant
		float4 vNewDepthDeltas = saturate(g_vSSAOOcclusionCalcParameters.w * fDepthDeltas);
		
		// 0.5f means unknown, better keep this hard coded(instead of g_vSSAOOcclusionCalcParameters.wwww)
		vNewDepthDeltas = fDepthDeltas < fHaloDistanceThreshold.xxxx ? 0.5f : vNewDepthDeltas;
		
		fOccludedPercent += dot(vNewDepthDeltas, 1.f / OCCLUSION_SAMPLE_COUNT);
	}
	
	float fUnOccludedPercent = 1.f - fOccludedPercent;
	
	// Applay constrast and brightness remapping to the unoccluded factor/
	// \Todo - need depth-dependent controls over constrast and brightness since distant
	//		occlusion gets lighter after blurring
	fUnOccludedPercent = saturate(pow(fUnOccludedPercent, g_vSSAOOcclusionRemapParameters.x) * g_vSSAOOcclusionRemapParameters.y + g_vSSAOOcclusionRemapParameters.z);
	
	// Fade out occlusion based on distance , used to hide artifacts on distance objects.
	fUnOccludedPercent = lerp(fUnOccludedPercent, 1.f, saturate((fDepth - g_vSSAOOcclusionFadeoutParameters) * g_vSSAOOcclusionFadeoutParameters.y));
	
	// Clamp the unoccluded percent to the adjustable minimum, output occlusion and depth
	return OutputOcclusionAndDepth(max(fUnOccludedPercent, g_vSSAOOcclusionRemapParameters.w), fDepth);
}
//---------------------------------------------------------------------------------------
// AO Blur Filter
float4 PS_Main_SSAO_Blur(VS_OUTPUT_OCCLUSION kInput) : COLOR0
{
	// Occlusion value in x, Depth in y
	float2 vCenterOcclusionAndDepth = CalcOcclusionAndDepth(kInput.vUVSet0);
	
	// Calculate a kernel scal factor inversely proportional to depth.
	// This is used to decrease the blur amount on distant pixel which preserves detail and unfortunately noise
	float fKernelScale = clamp(g_vSSAOBlurParameters.z / vCenterOcclusionAndDepth.y, 0.5f, 1.f);
	
	// Calculate a depth delta scale based on distance, so that nearby edges are 
	// calculated more accurately and large slopes at a distance are not identified as edges
	float fDeltaScale = clamp(g_vSSAOBlurParameters.y * vCenterOcclusionAndDepth.y, 1.f, 100.f);
	
	// Depth delta thar will be used to identify edges
	half fDepthDelta = g_vSSAOBlurParameters.x + fDeltaScale;
	
	// Start out with the current occlusion value
	half fSum = vCenterOcclusionAndDepth.x;
	
	// Stores an accumulation of valid weights, starts out with 1 as the current occlusion value is fully weighted
	half fTotalWeight = 1.f;
	
	// Calc 2 samples at on loop to better paralize
	[unroll]
	for (int i = 0; i < BLUR_SAMPLE_COUNT; ++i)
	{
		// Applay offsets for each sample, scaling by the depth-dependent fKernelScale
		float4 vSampleOfsets = kInput.vUVSet0.xyxy + g_vBlurSampleOffsets[i] * fKernelScale.xxxx;
		
		// Find depth and occlusion at the current pixel
		float4 vSampleOcclusionAndDepths = float4(CalcOcclusionAndDepth(vSampleOfsets.xy),CalcOcclusionAndDepth(vSampleOfsets.zw));
		
		// If the sample's depth is within fDepthDelta units from the current pixel's depth
		// consider it valid sample
		float2 vDepthCompares = abs(vSampleOcclusionAndDepths.yw - vCenterOcclusionAndDepth.yy) < fDepthDelta.xx;
		
		//
		fTotalWeight += vDepthCompares.x + vDepthCompares.y;
		
		// Accumulate occlusion values from valid samples
		fSum += dot(vSampleOcclusionAndDepths.xz, vDepthCompares);
	}

	// Pass depth through, Normalize and output the filtered occlusion value
	return OutputOcclusionAndDepth(fSum / fTotalWeight, vCenterOcclusionAndDepth.y);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_SSAO_Apply(VS_OUTPUT_QUAD kInput) : COLOR0
{
	float fOcclusion = tex2D(sdAOSampler, kInput.vUVSetScreenTex).x
	return float4(0, 0, 0, 1.f - fOcclusion);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_SSAO_Debug(VS_OUTPUT_QUAD kInput) : COLOR0
{
	float fOcclusion = tex2D(sdAOSampler, kInput.vUVSetScreenTex).x
	return float4(fOcclusion, fOcclusion, fOcclusion, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_SSAO_DownScale
<
	string Description = "PostEffect_SSAO_DownScale";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_SSAO_DownScale ();
		PixelShader 	= compile ps_3_0 PS_Main_SSAO_DownScale ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_SSAO_Occlusion
<
	string Description = "PostEffect_SSAO_Occlusion";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_SSAO_Occlusion ();
		PixelShader 	= compile ps_3_0 PS_Main_SSAO_Occlusion ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_SSAO_Blur
<
	string Description = "PostEffect_SSAO_Blur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_SSAO_Blur ();
		PixelShader 	= compile ps_3_0 PS_Main_SSAO_Blur ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_SSAO_Apply
<
	string Description = "PostEffect_SSAO_Apply";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Quad ();
		PixelShader 	= compile ps_2_0 PS_Main_SSAO_Apply ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_SSAO_Debug
<
	string Description = "PostEffect_SSAO_Debug";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Quad ();
		PixelShader 	= compile ps_2_0 PS_Main_SSAO_Debug ();
	}
}
//---------------------------------------------------------------------------------------



















