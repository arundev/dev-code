//*************************************************************************************************
// ����:	Depth of Field
//---------------------------------------------------------
// ����:		
// ����:		2012-10-20
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ��˹���˲���
float2		g_akDOFSamplerOffsets[9]	: GLOBAL;
float		g_akDOFSampleWeights[9]		: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
// ��Ļ��ɫ����������
texture2D sdColorTex 
<
	string NTM = "shader";
	int NTMIndex = 0;
>;

sampler2D sdColorSampler = sampler_state
{
    Texture 	= <sdColorTex>;
    AddressU  	= Clamp; 
	AddressV 	= Clamp;
	MinFilter 	= Linear; 
	MagFilter 	= Linear;
    SRGBTexture = false;
};

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
// ���²���
float4 PS_Main_DownSample(VS_OUTPUT kInput) : COLOR0
{
	return tex2D(sdColorSampler, kInput.vUVSetScreenTex);
}
//---------------------------------------------------------------------------------------
// �����˹����
float4 PS_Main_SeparableBlur(VS_OUTPUT kInput) : COLOR0
{
	// �����˹ģ��
	float4 vSample = 0.f;
	float4 vColor = 0.f;
	float2 vSamplePosition;
	for (int iSample = 0; iSample < 9; iSample++)
	{
		vSamplePosition = kInput.vUVSetScreenTex + g_akDOFSamplerOffsets[iSample];
		vColor = tex2D(sdColorSampler, vSamplePosition.xy);
		vSample += g_akDOFSampleWeights[iSample] * vColor;
	}
	
	return vSample;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique PostEffect_DOF_DownSample
<
	string Description = "PostEffect_DOF_DownSample";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_DownSample ();
	}
}
//---------------------------------------------------------------------------------------
technique PostEffect_DOF_SeparableBlur
<
	string Description = "PostEffect_DOF_SeparableBlur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_SeparableBlur ();
	}
}
//---------------------------------------------------------------------------------------
