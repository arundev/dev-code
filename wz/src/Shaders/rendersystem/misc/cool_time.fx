//*************************************************************************************************
// ����:	uiͼ����ȴ��ɫ��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-07
// ����޸�:	
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
//float4x4	g_mWorld		: WORLD;
//float4x4	g_mViewProj		: VIEWPROJECTION;
//float2	g_vWindowSize	: GLOBAL;

float2	a_vCoolLeftTop		: ATTRIBUTE = float2(0, 0);
float	a_akCoolValue[108]	: ATTRIBUTE;

float 	a_fSlotSize			: ATTRIBUTE = 32;
float	a_fUseMaxSlotIndex	: ATTRIBUTE = 108;	// Ӱ�����Slot����
float	a_fColNum			: ATTRIBUTE;		// ����
float	a_fColBlank			: ATTRIBUTE;		// �м��
float	a_fRowNum			: ATTRIBUTE;		// ����
float	a_fRowBlank			: ATTRIBUTE;		// �м��

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;	
	float2	vUVSet0		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	float4 vProjPos = mul(float4(vPos.xyz, 1.f), g_mWorldViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSet0 = vProjPos.xy;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{	
	float4 vTransColor = float4(0, 0, 0, 0);
	float4 vMaskColor = float4(0.08f, 0.08f, 0.08f, 0.8f);
	float4 vOutColor = vTransColor;
	
	// ���㵱ǰ������Ļ����
	float2 vPos = kInput.vUVSet0 * g_vWindowSize;
	
	// ����Slot�հ�����
	int bc = (int)(vPos.x - a_vCoolLeftTop.x) % (int)(a_fSlotSize + a_fColBlank);
	int br = (int)(vPos.y - a_vCoolLeftTop.y) % (int)(a_fSlotSize + a_fRowBlank);
		
	int aa = 5;
	if (bc < aa - 1 || bc > a_fSlotSize - aa)
		return vOutColor;
		
	if (br < aa || br > a_fSlotSize - aa)
		return vOutColor;
		
	// ���㵱ǰ��������Slot
	int col = (int)(vPos.x - a_vCoolLeftTop.x) / (int)(a_fSlotSize + a_fColBlank);
	int row = (int)(vPos.y - a_vCoolLeftTop.y) / (int)(a_fSlotSize + a_fRowBlank);
	int slotIndex = row * a_fColNum + col;
	
	// 
	float fCoolValue = a_akCoolValue[slotIndex];
	if (fCoolValue == 1.f)
		return vOutColor;
		
	// ��������Slot����ȴ����λ��
	float2 vCoolCenter;
	vCoolCenter.x = a_vCoolLeftTop.x + col * (a_fSlotSize + a_fColBlank) + a_fSlotSize / 2.f;
	vCoolCenter.y = a_vCoolLeftTop.y + row * (a_fSlotSize + a_fRowBlank) + a_fSlotSize / 2.f;	
	vCoolCenter /= g_vWindowSize;
	
	// ������ȴ����
	float fPercent = fCoolValue * PI * 2;
	float2 o = float2(0, -1);
	int b = 0;
	
	if (kInput.vUVSet0.x >= vCoolCenter.x && kInput.vUVSet0.y <= vCoolCenter.y)
	{
		b = 0;
		o = float2(0, -1);
	}
		if (kInput.vUVSet0.x >= vCoolCenter.x && kInput.vUVSet0.y >= vCoolCenter.y)
	{
		b = 1;
		o = float2(1, 0);
	}
		if (kInput.vUVSet0.x <= vCoolCenter.x && kInput.vUVSet0.y >= vCoolCenter.y)
	{
		b = 2;
		o = float2(0, 1);
	}
		if (kInput.vUVSet0.x <= vCoolCenter.x && kInput.vUVSet0.y <= vCoolCenter.y)
	{
		b = 3;
		o = float2(-1, 0);
	}
	
	int sb = floor(fCoolValue * 4);
	if (b > sb)
	{
		vOutColor = vMaskColor;
	}
	else if (b < sb)
	{
		vOutColor = vTransColor;
	}
	else
	{
		float2 r = normalize(kInput.vUVSet0 - vCoolCenter);
		float v = dot(r, o);
		float a = acos(v) + sb * PI / 2.f;
		if (a < fPercent)
			vOutColor = vTransColor;
		else
			vOutColor = vMaskColor;
	}
	
	return vOutColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique UI_CoolTime
<
	string Description = "UI_CoolTime";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = false;
		//ZWriteEnable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = true;
		//BlendOp = Add;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------