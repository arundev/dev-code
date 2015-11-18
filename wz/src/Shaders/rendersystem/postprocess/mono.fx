//*************************************************************************************************
// ����:	��Ļ��ɫ��ɫ��
//---------------------------------------------------------
// ����:		
// ����:		2012-10-09
// ����޸�:	2013-05-02
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ��ɫ��ɫ
float3		a_vMonoColor	: ATTRIBUTE; 		

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
//SD_LINEAR_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);		// ��Ļ��ɫ����

texture sdColorBuf <string NTM = "base";>;
sampler sdColorSampler = sampler_state
{
	Texture		= <sdColorBuf>;
	MINFILTER	= LINEAR;
	MIPFILTER	= NONE;
	MAGFILTER	= LINEAR;
	ADDRESSU	= CLAMP;
	ADDRESSV	= CLAMP;
	SRGB_DISABLE;
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
	//kOutput.vProjPos = float4(kInput.vPos, 1.f);
	kOutput.vProjPos = float4(kInput.vPos.xy, 1.f, 1.f);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ������������
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// ����Ҷ�ֵ
	float fLumin = dot(vColor.xyz, g_vLuminScale3);
	
	return float4(a_vMonoColor * fLumin, 1.0);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_Mono
<
	string Description = "PostEffect_Mono";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = false;     
		//ZWriteEnable = false;	
		//AlphaTestEnable = false;
		//AlphaBlendEnable = false;		
        //StencilEnable = false;
		//SRGBWriteEnable = false;
	}
}
//---------------------------------------------------------------------------------------