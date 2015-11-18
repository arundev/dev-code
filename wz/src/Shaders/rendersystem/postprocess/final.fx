//*************************************************************************************************
// ����:	һЩ���ù���Pass
//---------------------------------------------------------
// ����:		
// ����:		2012-10-09
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
bool	g_bRedAlert			: GLOBAL = false;	// ��Ѫ��ĻЧ��
bool	g_bDeadEffect 		: GLOBAL = false;	// ������Ļ���Ч��

// Gammaֵ
float	g_fGammaCorrectionValue	: GLOBAL = 1.0;	

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdColorSampler,		sdColorTex,		false);
SD_LINEAR_CLAMP_SAMPLE(1,	sdRedAlertSampler,	sdRedAlertTex,	false);
 
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
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// �򵥿���
float4 PS_Main_Copy(VS_OUTPUT kInput) : COLOR0
{
	return tex2Dlod(sdColorSampler, float4(kInput.vUVSetScreenTex, 0.0, 0.0));
}
//---------------------------------------------------------------------------------------
// ���������պ�̨����
float4 PS_Main_CopyToBackBuffer(VS_OUTPUT kInput) : COLOR0
{
	// ������������
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// ���Ӻ�Ѫ��ĻЧ��(���ҹ��ɱ仯)
	if (g_bRedAlert)
	{
		float4 vRedColor = tex2D(sdRedAlertSampler, kInput.vUVSetScreenTex);
		float fRadio = sin(5.f * g_fTime) * 0.3 + 0.7;
		vColor.rgb += float3(1.f, 0, 0) * vRedColor.a * fRadio;
	}
	
	// �������������ĻЧ��
	if (g_bDeadEffect)
	{
		vColor = dot(vColor.rgb, g_vLuminScale3);
	}
	
	// Gamma����
	//vColor = pow(saturate(vColor), 1.f / 2.2f);
	vColor = pow(saturate(vColor), 1.f / g_fGammaCorrectionValue);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// �򵥿���
technique PostEffect_Copy
<
	string Description = "PostEffect_Copy";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Copy ();
	}
}
//---------------------------------------------------------------------------------------
// ��������̨���沢�ϳ�
technique PostEffect_CopyToBackBuffer
<
	string Description = "PostEffect_CopyToBackBuffer";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_CopyToBackBuffer ();
	}
}
//---------------------------------------------------------------------------------------