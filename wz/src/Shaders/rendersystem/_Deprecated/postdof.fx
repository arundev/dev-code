//*************************************************************************************************
// ����:	Depth of Field(�ѷ���,����)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-09
// ����޸�:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// Coc�������
float3 	g_vDofStartEnd	: GLOBAL;
#define	g_fDofStart		g_vDofStartEnd.x
#define	g_fDofEnd		g_vDofStartEnd.y
#define	g_fDofStartZ	g_vDofStartEnd.z

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, 	sdGeomSampler,			sdGeomBuf,			false);	// ��Ļ����뷨�߻���
SD_LINEAR_CLAMP_SAMPLE(1,	sdQuarterColorSampler,	sdQuarterColorBuf,	false);	// 1/4��Ļ��ɫ����

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
	float2	vUVSetBlur		: TEXCOORD1;	// ����������ƫ�Ƶ�1/4��Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos.xy, g_fDofStartZ, 1.0);
	
	// ��Ļ��������(����������ƫ��)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// Multiply the g_vHalfPixelOffset with 4.
	// Since the quater size rendertarget pixel's uv span is four times as 
	// greate as the full size rendertarget's pixel
	kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float CalcDofCoc(float fDepth)
{
	return saturate(fDepth * g_fDofStart + g_fDofEnd);
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ���
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ����ģ��
	float fCoc = CalcDofCoc(fDepth);
	
	// ����ģ������
	float3 vBlurColor = tex2D(sdQuarterColorSampler, kInput.vUVSetBlur).rgb;
	
	return float4(vBlurColor, fCoc);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_PostDof
<
	string Description = "PostEffect_PostDof";
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