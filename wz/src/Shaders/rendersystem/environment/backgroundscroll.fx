//*************************************************************************************************
// ����:	Background Scroll
//				- ���ڻ��Ƶ�����ȦԶ��
//---------------------------------------------------------
// ����:		
// ����:		2012-10-31
// ����޸�:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// [alpha][height][u_wrap][z_scale][v_bias]
float 		a_akBGScrollParams[5]	: ATTRIBUTE;
//#define 	a_fScrollAlpha		a_akBGScrollParams[0]
#define 	a_fScrollHeight		a_akBGScrollParams[1]
#define		a_fScrollUWrap		a_akBGScrollParams[2]
#define		a_dScrollZScale		a_akBGScrollParams[3]
//#define	a_dScrollVBias		a_akBGScrollParams[4]

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdScrollSampler, sdScrollTex, true);

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
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ����ͶӰ����
	kInput.vPos.z *= a_fScrollHeight;
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vProjPos.z *= a_dScrollZScale;
	
	// ����UV
	kOutput.vUVSet0 = float2(kInput.vUVSet0.x * a_fScrollUWrap, kInput.vUVSet0.y);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vScrollColor = tex2D(sdScrollSampler, kInput.vUVSet0);
	return float4(lerp(vScrollColor.rgb, g_vFogColor, g_fFogDensity), vScrollColor.a);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Environment_BackgroundScroll
<
	string Description = "Environment_BackgroundScroll";
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
		//ZFunc = LessEqual;
		//ZWriteenable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------