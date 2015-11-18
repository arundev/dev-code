//*************************************************************************************************
// ����:	GodRay
//---------------------------------------------------------
// ����:		
// ����:		2012-10-22
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
float3	g_vGodRayDirection	: GLOBAL = { 0.f, 0.f, 1.f};	// ��Ҫnormalize��
float3	g_vGodRayColor		: GLOBAL = { 1.f, 1.f, 0.8f};
float2	g_vGodRaySize		: GLOBAL = {0.4f, 0.4f};
float	g_fGodRayIntensity	: GLOBAL = 1.f;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdColorSampler,		sdColorTex,		false);
SD_LINEAR_CLAMP_SAMPLE(1, 	sdRaySampler,		sdRayTex,		false);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ���
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ������������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_BLUR
{
	float4 	vProjPos		: POSITION;		// ͶӰ����
	float2	vUVSet0			: TEXCOORD0;	// 
	float2	vBlurCenter		: TEXCOORD1;
	float	fFactor			: TEXCOORD2;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_GODRAY
{
	float4 	vProjPos		: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex	: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float2	vUVSetRayTex	: TEXCOORD1;
	float	fFactor			: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// GodRay Radial Blur
VS_OUTPUT_BLUR VS_Main_GodRay_RadialBlur(VS_INPUT kInput) 
{
	VS_OUTPUT_BLUR kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// 
	kOutput.vUVSet0 = kInput.vUVSet0 + g_vHalfPixelOffset * 4.f;
	
	// ����Sun��ͶӰ�ռ�λ��
	float4 vWorldPos = float4(g_vCameraPostion + g_vGodRayDirection, 1.f);
	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	
	float2 vCenter = vProjPos.xy / vProjPos.w * 0.5f + 0.5f;
	kOutput.vBlurCenter = float2(vCenter.x, 1.f - vCenter.y);
	
	// 
	float fDot = max(0.f, dot(g_vGodRayDirection, g_mInvView[2]));
	float fF = 0.9f;
	kOutput.fFactor = max(0, fDot - fF) / (1.f - fF);
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// GodRay Composite
VS_OUTPUT_GODRAY VS_Main_GodRay_Composite(VS_INPUT kInput) 
{
	VS_OUTPUT_GODRAY kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 
	kOutput.vUVSetRayTex = kInput.vUVSet0 + g_vHalfPixelOffset * 4.f;
	
	// 
	float fDot = max(0.f, dot(g_vGodRayDirection, g_mInvView[2]));
	float fF = 0.9f;
	kOutput.fFactor = max(0, fDot - fF) / (1.f - fF);
	
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// GodRay Radial Blur,����ģ��
#define NUM_SAMPLES 8
float4 PS_Main_GodRay_RadialBlur(VS_OUTPUT_BLUR kInput) : COLOR0
{
	clip(kInput.fFactor);
	
	//
	float3 vColor = tex2D(sdColorSampler, kInput.vUVSet0).xyz;
	
	// 
	float2 vCoord = kInput.vUVSet0;
	float2 vCoordDelta = (kInput.vUVSet0 - kInput.vBlurCenter) * 1.f / NUM_SAMPLES * 0.7;
	float fDecay = 1.f;
	float3 vTempColor = 0.f;
	float2 vTempCoord = 0.f;
	for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		vCoord 		-= 	vCoordDelta;
		vTempCoord 	= clamp(vCoord, 0.f, 1.f);
		vTempColor	= tex2D(sdColorSampler, vTempCoord) * fDecay * 0.5f;	// SampleȨ��
		vColor		+= vTempColor;
		fDecay		*= 0.8f;		// Samples����Զ������Ȩ��˥��
	}
	
	return float4(vColor * 0.5f * g_fGodRayIntensity, 1.f);
}
//---------------------------------------------------------------------------------------
// GodRay Composite
float4 PS_Main_GodRay_Composite(VS_OUTPUT_GODRAY kInput) : COLOR0
{
	float3 vBaseColor 	= tex2D(sdColorSampler, kInput.vUVSetScreenTex).xyz;
	float3 vRayColor 	= tex2D(sdRaySampler, kInput.vUVSetRayTex).xyz;
	
	float fFactor = kInput.fFactor;
	float3 vColor = vBaseColor * (1.f - 0.15f * fFactor) + vRayColor * fFactor * 1.2f;
	
	return float4(vColor, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_GodRay
<
	string Description = "PostEffect_GodRay";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_GodRay_Composite ();
		PixelShader 	= compile ps_2_0 PS_Main_GodRay_Composite ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_GodRay_RadialBlur
<
	string Description = "PostEffect_GodRay_RadialBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_GodRay_RadialBlur ();
		PixelShader 	= compile ps_2_0 PS_Main_GodRay_RadialBlur ();
	}
}
//---------------------------------------------------------------------------------------





















