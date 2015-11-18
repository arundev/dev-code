//*************************************************************************************************
// ����:	Under Water
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


//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_WRAP_SAMPLE(0, 	sdWaterNormalSampler,		sdWaterNormalTex,	false);	// ˮ�淨������
SD_POINT_CLAMP_SAMPLE(5, 	sdRefractSampler,			sdRefractTex,		false);	// ˮ����������
SD_LINEAR_CLAMP_SAMPLE(6, 	sdFresnelSampler,			sdFresnelTex,		false);	// ������ϵ������

// 
texture sdReflectCubeTex<string	NTM = "shader";	int NTMIndex = 3;>;																
samplerCUBE sdReflectCubeSampler = sampler_state							
{																
	Texture		= (sdReflectCubeTex);										
	ADDRESSU	= WRAP;										
	ADDRESSV	= WRAP;	
	ADDRESSW	= WRAP;										
	MAGFILTER	= LINEAR;
	MINFILTER	= LINEAR;
	//MIPFILTER	=  ANISOTROPIC;
	MAXANISOTROPY = 4;
	//MipmapLodBias = (LODBias);
	SRGBTexture = true;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSet0				: TEXCOORD0;	// xy height map uv,zw uniforma uv
	float4	vUVSetWorldPos		: TEXCOORD1;	// .w save refrac scale
	float4	vUVSetViewPos		: TEXCOORD2;
	float4	vUVSetProjPos		: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput;
	
	// ��������
	float4 vWorldPosition = mul(float4(vPos, 1.0f), g_mWorld);
	kOutput.vUVSetWorldPos = vWorldPosition;
	
	// �۲�����
	float4 vViewPosition = mul(float4(vPos, 1.0f), g_mWorldView);
	kOutput.vUVSetViewPos = vViewPosition;
	
	// ͶӰ����
	float4 vProjPosition = mul(float4(vWorldPosition.xyz, 1), g_mViewProj);
	kOutput.vProjPos = vProjPosition;
	kOutput.vUVSetProjPos = vProjPosition;

	// ��������
	float2 vHeightUVSet = vWorldPosition.xy / g_fWaterHeightMapSize;
	kOutput.vUVSet0 = vHeightUVSet;
	
	//
	kOutput.vUVSetWorldPos.w = g_fRefractionDistortionStrength / max(0.002f,vProjPosition.z / vProjPosition.w);
	
	// ��ǰ���ӦԶ�ü�����������
	//float4 vFarWorldPos = mul(float4(vProjPos.xy / vProjPos.w, 1, 1), g_mDepthToWorld) * vProjPos.w;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_UnderFFTWater(VS_OUTPUT kInput) : COLOR0
{
	// ����������ͼ
	float4 N = -tex2D(sdWaterNormalSampler, kInput.vUVSet0.xy);
	
	// ����
	// @{
	// ����۲췽��
	float3 V = g_vViewPos - kInput.vUVSetWorldPos.xyz;
	V = normalize(V);
	
	// ���㷴����������
	float3 vReflectVec = normalize(reflect(V, N.xyz));
	
	// ������������
	float4 vReflectColor = texCUBE(sdReflectCubeSampler, -vReflectVec.xzy);
	// @}
	
	// ����
	// @{
	// ������Ļ��������
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// ����������������
	// Calc refraction UV which bease on device Z
	//float fRefracScale = kInput.vUVSetWorldPos.w;
	float fRefracScale = g_fRefractionDistortionStrength * 0.6f;
	float2 vRefractionUV = vScreenUVSet - fRefracScale * N.xy;
	
	// Get refraction texture color
	float4 vRefractionColor = tex2D(sdRefractSampler, vRefractionUV);
	// @}
	
	// �ϳ�
	vRefractionColor.rgb = lerp(vRefractionColor.rgb, vReflectColor.rgb, 0.6f);
	
	// �ϳ���
	float fDistance = distance(g_vViewPos, kInput.vUVSetWorldPos.xyz);
	float fFogFactor = 1.f / exp(fDistance * g_fFogIntensity * 0.125f);
	vRefractionColor.rgb = lerp(g_vWaterColor, vRefractionColor.rgb, fFogFactor);
	
	return vRefractionColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_UnderNormalBlendWater(VS_OUTPUT kInput) : COLOR0
{
	// ����
	// @{
	// ����ˮ�ƶ����ʼ������λ��ƫ��(����û���ǵ���ˮ�ƶ�����)
	float fCycle = 1.f / g_fWaterSpeed;
	float fTranslation = fmod(g_fTime, fCycle) * g_fWaterSpeed;
	float2 vTranslation = float2(fTranslation, -fTranslation);

	// ������������
	float2 vUVSet0 = kInput.vUVSet0 * 0.2f;
	float3 vBaseNormal = tex2D(sdWaterNormalSampler, vUVSet0 + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 2.f + vTranslation * 4.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 4.f + vTranslation * 2.f).xyz;
	vBaseNormal += tex2D(sdWaterNormalSampler, vUVSet0 * 8.f + vTranslation).xyz;
	
	// �ָ�����
	vBaseNormal = vBaseNormal * 0.5f - 1.f;
	
	// ����������ͨ����ķ���
	vBaseNormal.xy *= g_fNormalMapScale;
	
	float3 N = -normalize(vBaseNormal);
	// @}

	// ����
	// @{
	// ����۲췽��
	float3 V = g_vViewPos - kInput.vUVSetWorldPos.xyz;
	V = normalize(V);
	
	// ���㷴����������
	float3 vReflectVec = normalize(reflect(V, N));
	
	// ������������
	float4 vReflectColor = texCUBE(sdReflectCubeSampler, -vReflectVec.xzy);
	// @}

	// ����
	// @{
	// ������Ļ��������
	float2 vScreenUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// ����������������
	// Calc refraction UV which bease on device Z
	//float fRefracScale = kInput.vUVSetWorldPos.w;
	float fRefracScale = g_fRefractionDistortionStrength * 0.6f;
	float2 vRefractionUV = vScreenUVSet - fRefracScale * N.xy;
	
	// Get refraction texture color
	float4 vRefractionColor = tex2D(sdRefractSampler, vRefractionUV);
	// @}
	
	// �ϳ�
	vRefractionColor.rgb = lerp(vRefractionColor.rgb, vReflectColor.rgb, 0.6f);
	
	// �ϳ���
	float fDistance = distance(g_vViewPos, kInput.vUVSetWorldPos.xyz);
	float fFogFactor = 1.f / exp(fDistance * g_fFogIntensity * 0.125f);
	vRefractionColor.rgb = lerp(g_vWaterColor, vRefractionColor.rgb, fFogFactor);
	
	return vRefractionColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Water_UnderFFTWater
<
	string Description = "Water_UnderFFTWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_UnderFFTWater ();
	}
}
//---------------------------------------------------------------------------------------
technique Water_UnderNormalBlendWater
<
	string Description = "Water_UnderNormalBlendWater";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_UnderNormalBlendWater ();
	}
}
//---------------------------------------------------------------------------------------