//*************************************************************************************************
// ����:	Bloom
//---------------------------------------------------------
// ����:		
// ����:		2012-10-24
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"


//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ��ɫ
float3	g_vMonoColor		: GLOBAL = {1.f, 0, 0};

// Bloom��ȡ��ֵ
float	g_fBloomThreshold	: GLOBAL = 0.25f;

// Bloom��������
#define SAMPLE_COUNT 15
float2 g_vBloomSampleOffsets[SAMPLE_COUNT]	: GLOBAL;
float  g_vBloomSampleWeights[SAMPLE_COUNT]	: GLOBAL;

// Bloom����
float	g_fBloomIntensity	: GLOBAL;
float	g_fBaseIntensity	: GLOBAL;

float 	g_fBloomSaturation	: GLOBAL;
float	g_fBaseSaturation	: GLOBAL;


//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdColorSampler,	sdColorBuf,	false);	
SD_POINT_CLAMP_SAMPLE(0,	sdBloomSampler,	sdBloomBuf,	false);	

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ�������
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ���ζ�����������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos.xyz, 1.0);
	
	// 
	kOutput.vUVSetScreenTex = kInput.vUVSet0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Bloom_Textured(VS_OUTPUT kInput) : COLOR0
{
	return float4(tex2D(sdColorSampler, kInput.vUVSetScreenTex).rgb, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Bloom_MonoChrome(VS_OUTPUT kInput) : COLOR0
{
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).rgb;
	float fGray = dot(vColor, g_vLuminScale3);
	
	return float4(g_vMonoColor * fGray, 1.f);
}
//---------------------------------------------------------------------------------------
// Adjust original image color to keep only values brighter than the specified threshold
float4 PS_Main_Bloom_Extract(VS_OUTPUT kInput) : COLOR0
{
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).rgb;
	return float4(saturate((vColor - g_fBloomThreshold) / (1.f - g_fBloomThreshold)), 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Bloom_GuassianBlur(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = 0.f;
	for (int i= 0; i < SAMPLE_COUNT; ++i)
		vColor += g_vBloomSampleWeights[i] * tex2D(sdColorSampler, kInput.vUVSetScreenTex + g_vBloomSampleOffsets[i]);
		
	return vColor;
}
//---------------------------------------------------------------------------------------
// Helper for modifying the saturation of a color
float3 AdjustSaturation(float3 vColor, float fSaturation)
{
	// The constans 0.3, 0.59, 0.11 are chosen beacuse the
	// human eye is more sensitive to green light, and less to blue.
	float fGrey = dot(vColor, float3(0.3f, 0.59f, 0.11f));
	
	return lerp(fGrey, vColor, fSaturation);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Bloom_Combine(VS_OUTPUT kInput) : COLOR0
{
	// Look up the bloom and original base image colors.
	// (����UV�϶��ǲ��Ե�)
	float3 vBaseColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).rgb;
	float3 vBloomColor = tex2D(sdBloomSampler, kInput.vUVSetScreenTex).rgb;
	
	// Adjust color saturation and intensity
	vBaseColor = AdjustSaturation(vBaseColor, g_fBaseSaturation) * g_fBaseIntensity;
	vBloomColor = AdjustSaturation(vBloomColor, g_fBloomSaturation) * g_fBloomIntensity;
	
	// Darken down the base image in areas where there is a lot of bloom,
	// to prevent things looking excessively burned-out
	vBaseColor *= (1.f - saturate(vBloomColor));
	
	// Combine the two images.
	return float4(vBaseColor + vBloomColor, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_Bloom_Textured
<
	string Description = "PostEffect_Bloom_Textured";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Bloom_Textured ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_Bloom_MonoChrome
<
	string Description = "PostEffect_Bloom_MonoChrome";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Bloom_MonoChrome ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_Bloom_Extract
<
	string Description = "PostEffect_Bloom_Extract";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Bloom_Extract ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_Bloom_GuassianBlur
<
	string Description = "PostEffect_Bloom_GuassianBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Bloom_GuassianBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_Bloom_Combine
<
	string Description = "PostEffect_Bloom_Combine";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Bloom_Combine ();
	}
}
//---------------------------------------------------------------------------------------