//*************************************************************************************************
// ����:	SkyDome Cloud
//				- ������Ʋ�
//---------------------------------------------------------
// ����:		
// ����:		2012-10-31
// ����޸�:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// [cloud_wrap][cloud_speed][z-bias][alpha]
float4	a_vSkyDomeCloudParams	: ATTRIBUTE;
#define a_fSkyDomeCloudWrap		a_vSkyDomeCloudParams.x
#define	a_fSkyDomeCloudSpeed	a_vSkyDomeCloudParams.y
#define	a_fSkyDomeCloudAlpha	a_vSkyDomeCloudParams.w

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdCloudSampler, sdCloudTex, true);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos				: POSITION0;
	float3	vUVSetUVAndAlpha	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float3	vUVSetUVAndAlpha	: TEXCOORD0;	// UV������͸����
	float3	vUVSetViewPos		: TEXCOORD1;	// �۲�ռ�����
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ����ͶӰ����
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vUVSetViewPos = kOutput.vProjPos;
	kOutput.vProjPos.z *= 0.999999f;
	
	// ����UV
	kOutput.vUVSetUVAndAlpha.xy = float2(kInput.vUVSetUVAndAlpha.xy * a_fSkyDomeCloudWrap * 0.5f);
	kOutput.vUVSetUVAndAlpha.xy += float2(g_fTime * 0.1f * a_fSkyDomeCloudSpeed, 0.f);
	
	// ͸����
	kOutput.vUVSetUVAndAlpha.z = kInput.vUVSetUVAndAlpha.z;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ����������
	float2 vUVSet0 = kInput.vUVSetUVAndAlpha.xy;
	float4 vCloudColor = tex2D(sdCloudSampler, vUVSet0);
	
	// ����ϳ�
	float fDensity = CalcSkyFogDensity(normalize(kInput.vUVSetViewPos));
	float3 vFinalColor = lerp(vCloudColor.rgb, g_vSkyFogColor, fDensity);
	
	float fAlpha = kInput.vUVSetUVAndAlpha.z;
	
	return float4(vFinalColor, vCloudColor.a * saturate(fAlpha / max(a_fSkyDomeCloudAlpha, 0.001f)));
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Environment_SkyDomeCloud
<
	string Description = "Environment_SkyDomeCloud";
	bool UsesNiRenderState = true;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//FillMode = Solid;
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