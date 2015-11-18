//*************************************************************************************************
// ����:	Post Fog(�ѷ���,����)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-22
// ����޸�:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdDepthSampler,	sdDepthBuf,	false);	// ��Ļ����뷨�߻���
SD_POINT_CLAMP_SAMPLE(1, sdColorSampler,	sdColorBuf,	false);	// ��Ļ��ɫ����

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ�������
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ���ζ�����������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex	: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float2	vUVSetProjPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// ��Ļ��������(����������ƫ��)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	//
	kOutput.vUVSetProjPos = kInput.vPos.xy;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float UnpackCoarseDepth(float2 enc) 
{ 
	return enc.x * enc.x; 
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// �����������,ת��Ϊ��ʵ���
	// (wzԭ�ľ���UnpackCoarseDepth())
	float4 vGeoTex = tex2D(sdDepthSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackCoarseDepth(vGeoTex.xy);
	float fRealDepth = fDepth * g_fCameraFarClip;
	
	// 
	float3 vViewPos = float3(kInput.vUVSetProjPos, 1.f) * fRealDepth;
	
	// ������ϵ��
	float fFogFactor = CalcFogFactors(vViewPos);
	
	// float3 vBaseColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).xyz;
	return float4(g_vFogColor, fFogFactor);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_tecPostFog
<
	string Description = "PostEffect_tecPostFog";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------