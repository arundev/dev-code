//*************************************************************************************************
// ����:	Depth of Field(�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-15
// ����޸�:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// Coc�������
float	g_fFocalDepth		: GLOBAL = 1000.0;	// �������
float	g_fNoBlurDepth		: GLOBAL = 5.0;		// ������Χ
float	g_fFullBlurDepth	: GLOBAL = 20.0;	// ��ɢ����
float	g_fDofIntensity		: GLOBAL = 1.0;		// ��ɢ�̶�

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdDepthSampler,			sdDepthBuf,			false);	// ��Ļ����뷨�߻���
SD_LINEAR_CLAMP_SAMPLE(1, sdColorSampler,			sdColorBuf,			false);	// ��Ļ��ɫ����
SD_LINEAR_CLAMP_SAMPLE(2, sdQuarterColorSampler,	sdQuarterColorBuf,	false);	// 1/4��Ļ��ɫ����

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
	float4		vProjPos			: POSITION;		// ͶӰ����
	float2		vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	//float2	vUVSetBlur			: TEXCOORD1;	// ����������ƫ�Ƶ�1/4��Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ģ������ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// ��Ļ��������(����������ƫ��)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 1/4��Ļ��������(����������ƫ��)
	//kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// �����������,ת��Ϊ��ʵ���
	float4 vGeoTex = tex2D(sdDepthSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	float fRealDepth = fDepth * g_fCameraFarClip;
	
	// ����Coc
	float fDistance	= max(fRealDepth - g_fFocalDepth, 0.0);
	float fDofFactor = saturate(max(0, fDistance - g_fNoBlurDepth) / (g_fFullBlurDepth - g_fNoBlurDepth));
	
	// ����ԭʼ����
	float3 vBaseColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).xyz;
	
	// ����ģ������
	// @{
	float2 vUVSetLT = kInput.vUVSetScreenTex + float2(-1.f, -1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetRT = kInput.vUVSetScreenTex + float2( 1.f, -1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetLB = kInput.vUVSetScreenTex + float2(-1.f,  1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetRB = kInput.vUVSetScreenTex + float2( 1.f,  1.f) * g_vHalfPixelOffset * 4.f;
	
	float3 vBlurColor = 0;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetLT).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetRT).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetLB).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetRB).xyz;
	// @}
	
	// ��ɫ�ϳ�
	return float4(lerp(vBaseColor, vBlurColor, fDofFactor), 1.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_tecDof
<
	string Description = "PostEffect_tecDof";
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