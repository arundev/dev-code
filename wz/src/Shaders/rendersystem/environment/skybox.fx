//*************************************************************************************************
// ����:	Sky Box
//				- ���ڻ���6����պ���, �Ժ��Ǹĳ�һ��������CryEngine/BigWorld
//---------------------------------------------------------
// ����:		
// ����:		2012-10-31
// ����޸�:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdSkySampler, sdSkyTex, true);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// ͶӰ����
	float2	vUVSet0			: TEXCOORD0;
	float3	vUVSetViewPos	: TEXCOORD1;	// �۲�ռ�����
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vUVSet0 = kInput.vUVSet0;
	kOutput.vUVSetViewPos = kInput.vPos;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float fDensity = CalcSkyFogDensity(normalize(kInput.vUVSetViewPos));
	float3 vFinalColor = lerp(tex2D(sdSkySampler, kInput.vUVSet0).xyz, g_vSkyFogColor, fDensity);
	
	return float4(vFinalColor, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Environment_SkyBox
<
	string Description = "Environment_SkyBox";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = true;
		//ZWriteenable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = false;
	}
}
//---------------------------------------------------------------------------------------