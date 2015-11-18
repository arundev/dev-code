//*************************************************************************************************
// ����:	Water Caustics
//
//---------------------------------------------------------
// ����:		
// ����:		2012-11-15
// ����޸�:
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf,		false);	// ������Ȼ���
SD_LINEAR_WRAP_SAMPLE(1, sdCausticsSampler,	sdCausticsTex,	false);	// ǳˮˮ�׽�ɢ����
SD_POINT_CLAMP_SAMPLE(2, sdRefractSampler,	sdRefractTex,	false);	// ˮ����������

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4	vPos		: POSITION0;	// .xyz - position, .w - water alpha
	float4	vDeepColor	: COLOR;
	float4	vParam		: TEXCOORD0;
	float4	vLowColor	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos				: POSITION;		// ͶӰ����
	float4	vUVSetDeepColor			: TEXCOORD0;
	float4	vUVSetLowColor			: TEXCOORD1;	
	float4	vUVSetParam				: TEXCOORD2;	
	float4	vUVSetFarClipWorldPos	: TEXCOORD3;
	float4	vUVSetProjPos			: TEXCOORD4;
	float4	vUVSetViewPos			: TEXCOORD5;
	float4	vUVSetWorldPos			: TEXCOORD6;	
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float4 vWorldPos = kInput.vPos;
	float4 vViewPos = mul(vWorldPos, g_mView);
	float4 vProjPos = mul(vViewPos, g_mProj);
	kOutput.vUVSetWorldPos = vWorldPos;
	kOutput.vUVSetViewPos = vViewPos;
	kOutput.vUVSetProjPos = vProjPos;
	kOutput.vProjPos = vProjPos;
	
	kOutput.vUVSetDeepColor = kInput.vDeepColor;
	kOutput.vUVSetLowColor = kInput.vLowColor;	
	kOutput.vUVSetParam = kInput.vParam;
	
	kOutput.vUVSetFarClipWorldPos = mul(float4(vProjPos.xy/vProjPos.w, 1, 1), g_mDepthToWorld) * vProjPos.w;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR
{
	// ������Ļ��������
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);

	// �����������
	float4 vGeomData = tex2D(sdGeomSampler, vScreenUVSet);
	float fDepth = UnpackDepth(vGeomData.xy);
	if (fDepth < 0.000001f)
		fDepth = 1.f;
		
	// ������������
	float3 vPixelWorldPos = lerp(g_vViewPos, kInput.vUVSetFarClipWorldPos.xyz/kInput.vUVSetFarClipWorldPos.w, fDepth);
	//clip(kInput.vUVSetWorldPos.z - vPixelWorldPos.z);
	
	// ���㽹ɢ
	float fMinRefractAlpha = kInput.vUVSetParam.r;
	float fMaxRefractAlpha = kInput.vUVSetParam.g;
	float2 vCausticsParameter = kInput.vUVSetParam.ba;
	float4 vCausticsColor = tex2D(sdCausticsSampler, vPixelWorldPos.xy / vCausticsParameter.x);
	
	float fDeepWaterAlpha = kInput.vUVSetWorldPos.a;
	float fDeepWaterDepth = kInput.vUVSetDeepColor.a;
	float fDeltaDepth = kInput.vUVSetWorldPos.z - vPixelWorldPos.z;
	float fDeepWaterThreshold = 0.2f;
	float fDeepWaterScale = (fDeltaDepth < fDeepWaterThreshold * fDeepWaterDepth) ? 1.f : 0.2f;
	
	float3 V = g_vViewPos - vPixelWorldPos;
	float fDistance = length(V);
	
	// x - caustic alpha base on world height
	// y - alpha between deepwater color and refraction texture
	// z - caustics alpha based on distance
	// (max caustics distance is based on the water max refraction alpha)
	float3 vSmoothFactor = 1.f - saturate(smoothstep(0, 
		float3(fDeepWaterDepth * fDeepWaterThreshold, fDeepWaterDepth, 20.f * fMaxRefractAlpha), 
		float3(abs(fDeltaDepth - fDeepWaterDepth * fDeepWaterThreshold) * fDeepWaterScale, fDeltaDepth, fDistance)));
		
	//float fWaterSmooth = pow(smoothstep(0, fDeepWaterDepth, fDeltaDepth), 0.15f);
	float fWaterSmooth = smoothstep(0, fDeepWaterDepth, fDeltaDepth);

	// ����
	float4 vTexColor = tex2D(sdRefractSampler, vScreenUVSet);
	
	// ��
	float vPixelViewPos = mul(float4(vPixelWorldPos, 1.f), g_mView);
	float fScattering = 1.f;//CalcHeightFogCurveFactor(float4(vPixelWorldPos.xyz, vPixelViewPos.z));
	float3 vNoFogColor = (vTexColor - g_vFogColor * (fScattering - 1.f)) / fScattering;
	
	float fRefracTexRatio = lerp(fMinRefractAlpha, fMaxRefractAlpha, 1.f - vSmoothFactor.y);
	float3 vWaterColor = lerp(kInput.vUVSetLowColor.rgb, kInput.vUVSetDeepColor.rgb, fRefracTexRatio);
	float3 vFinalColor = lerp(vNoFogColor * (1.f + vCausticsColor.r * vCausticsParameter.y * vSmoothFactor.x * vSmoothFactor.z), vWaterColor, fRefracTexRatio);
	
	// lerp between deep water color with refraction color
	return float4(vFinalColor, max(0.001f, fWaterSmooth));
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Water_RestoreBuffAndCaustics
<
	string Description = "Water_RestoreBuffAndCaustics";
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