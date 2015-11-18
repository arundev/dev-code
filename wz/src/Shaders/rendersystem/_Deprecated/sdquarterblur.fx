//*************************************************************************************************
// ����:	Quarter Blur(�ѷ���,����)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-22
// ����޸�:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdColorSampler,	sdColorTex,	false);	// ��Ļ��ɫ����

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
	// ����4������������
	float2 vUVSetLT = kInput.vUVSetBlur + float2(-2.f, -2.f) * g_vHalfPixelOffset;
	float2 vUVSetRT = kInput.vUVSetBlur + float2( 2.f, -2.f) * g_vHalfPixelOffset;
	float2 vUVSetLB = kInput.vUVSetBlur + float2(-2.f,  2.f) * g_vHalfPixelOffset;
	float2 vUVSetRB = kInput.vUVSetBlur + float2( 2.f,  2.f) * g_vHalfPixelOffset;
	
	// ����4�����������
	float3 vBlurColor = 0;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetLT).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetRT).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetLB).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetRB).xyz;
	
	return float4(vBlurColor, 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_tecQuarterBlur
<
	string Description = "PostEffect_tecQuarterBlur";
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