//*************************************************************************************************
// ����:	Depth of Field Guass Blur(�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-20
// ����޸�:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ��˹1D���˲�����ƫ����Ȩ��
float2	g_kDOFSamplerOffsets[11]	: GLOBAL;
float	g_kDOFSampleWeights[11]		: GLOBAL;

// ��˹1D���˲�����ƫ������
float	g_fDOFBlurScale				: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);	// ��Ļ��ɫ����

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
	float2	vUVSetBlur		: TEXCOORD0;	// ����������ƫ�Ƶ�1/4��Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// Multiplay the g_vHalfPixelOffset with 4.
	// Since the quater size render target pixel's uv span is four times as greate as
	// the full size render target's pixel
	kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// �����˹ģ��
	float3 vSample = 0.f;
	float3 vColor = 0.f;
	float2 vSamplePosition;
	for (int iSample = 0; iSample < 11; iSample++)
	{
		vSamplePosition = kInput.vUVSetBlur + g_fDOFBlurScale * g_kDOFSamplerOffsets[iSample];
		vColor = tex2D(sdColorSampler, vSamplePosition.xy).xyz;
		vSample += g_kDOFSampleWeights[iSample] * vColor;
	}
	
	return float4(vSample, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_tecDofGuassBlur
<
	string Description = "PostEffect_tecDofGuassBlur";
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


