//*************************************************************************************************
// ����:	Tree Leaf Shading��ɫ��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-14
// ����޸�:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdLightSampler, 	sdLightBuf, 	false);	// ���ջ���
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, 	sdGeomBuf, 		false);	// ���λ���
SD_POINT_CLAMP_SAMPLE(2, sdMatSampler, 		sdMatBuf, 		false);	// ���ʻ���

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 	vPos		: POSITION0;
	float2	vUVSet0		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 	vProjPos		: POSITION0;
	float2	vUVSetScreenPos	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	kOutput.vUVSetScreenPos = kInput.vUVSet0 + g_vHalfPixelOffset;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR
{
	// ����
	float4 vGeomData = tex2D(sdGeomSampler, kInput.vUVSetScreenPos);
	float3 vViewNormal = UnpackNormal(vGeomData.zw);
	
	// ���ع���
	float4 vLocalLight = tex2D(sdLightSampler, kInput.vUVSetScreenPos);
	
	// ȫ�ֹ���
	float3 vDiffuseLight;
	AccumLightingOnlyDiffuse(vViewNormal, vLocalLight, vDiffuseLight);

	// ����
	float3 vDiffuseTex = tex2D(sdMatSampler, kInput.vUVSetScreenPos).rgb;
	
	return float4(vDiffuseLight * g_vTreeLeafMaterial * vDiffuseTex, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Tree_MRTLeafShading
<
	string	Description 		= "Tree_MRTLeafShading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main();
	}
}
//---------------------------------------------------------------------------------------