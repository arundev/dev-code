//*************************************************************************************************
// 内容:	看上去像是UI(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-23
// 最后修改:	
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
//float4x4	g_mWorldViewProj		: WORDLVIEWPROJECTION;
//float		g_fTime					: TIME;

float		a_akArrayPrecent[12]	: ATTRIBUTE;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION0;
	float2	vUVSet		: TEXCOORD0;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = mul(float4(vPos.xyz, 1.f), g_mWorldViewProj);
	kOutput.vUVSet = kOutput.vUVSet;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_ShortCutScreen0(VS_OUTPUT kInput) : COLOR0
{	
	float4 vColor = float4(1.f, 0.5f, 0.5f, 0.f);
	
//	float ff = g_fTime / 1.f;
//	for (int i = 0; i < 1; ++i)
//		a_akArrayPrecent[i] = fmod(ff, 1.f);
		
	float fHalfRadius = 0.25f;
	float2 vUVSet0 = kInput.vUVSet;
	
	float fExtendX = 10.f * kInput.vUVSet.x;
//	float fExtenxY = 10.f * kInput.vUVSet.y;
//	float fFracX = frac(fExtendX);
//	float fFracY = frac(fExtendY);
	float4 vGray = float4(0.5f, 0.5f, 0.5f, 0.f);
	int iSlot = floor(fExtendX);
	
	float fExtendRadio = 0.1f;
	vUVSet0.y = vUVSet0.y * fExtendRadio;
	
	float2 axis;
	float2 vCenter = float2(iSlot * 0.1f + 0.05f, 0.5f * fExtendRadio);
	float2 r2 = vUVSet0 - vCenter;
//	if (((r2.x * r2.x) / 0.01f + r2.y * r2.y) < 0.25f)
	if (abs(r2.x) < (0.05f - 0.01f) && abs(r2.y) < (0.05f - 0.004f))
//  if (length(r2) < 0.05f)
	{
		vColor = vGray;
		
		axis = normalize(float2(0, -0.1f));
		float2 r = normalize(vUVSet0 - vCenter);
		float3 cc = cross(float3(axis, 0), float3(r, 0));
		float b = dot(r, axis);
		float a = asin(cc.z); // acos(v);
		
		if (b > 0)
		{
			if (a > 0)
			{
				a = a;
			}
			else
			{
				a = 6.28f + a;
			}
		}
		else
		{
			if (a > 0)
			{
				a = 3.14f - a;
			}
			else
			{
				a = 3.14f - a;
			}
		}

		if (a_akArrayPrecent[iSlot] != 0.f)
		{
			vColor = float4(0.12f, 0.17f, 0.12f, 0.9f);
		}
		
		if (a < a_akArrayPrecent[iSlot] * 3.1415926f * 2.f)
		{
			vColor = float4(1.f, 0.5f, 0.5f, 0.f);
		}
	}
	
	return vColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ShortCutScreen1(VS_OUTPUT kInput) : COLOR0
{	
	float4 vColor = float4(1.f, 0.5f, 0.5f, 0.f); // tex2D(s0, kInput.vUVSet);
	
//	for (int i = 0; i < 1; ++i)
//		a_akArrayPrecent[i] = i * 0.2f + 0.f;;
		
	float fHalfRadius = 0.5f;
	float2 vUVSet0 = kInput.vUVSet;
	
//	float fExtendX = 10.f * kInput.vUVSet.x;
	float fExtendY = 10.f * kInput.vUVSet.y;
//	float fFracX = frac(fExtendX);
//	float fFracY = frac(fExtendY);
	float4 vGray = float4(0.5f, 0.5f, 0.5f, 0.f);
	int iSlot = floor(fExtendY);
	
	float fExtendRadio = 0.1f;
	vUVSet0.x = vUVSet0.x * fExtendRadio;
	
	float2 axis;
	float2 vCenter = float2(0.5f * 0.1f, iSlot * 0.1f + 0.05f);
	float2 r2 = vUVSet0 - vCenter;
//	if (((r2.x * r2.x) + (r2.y * r2.y) / 0.01f) < 0.25f)
	if (abs(r2.x) < (0.05f - 0.004f) && abs(r2.y) < (0.05f - 0.01f))
//  if (length(r2) < 0.05f)
	{
		vColor = vGray;
		
		axis = normalize(float2(fHalfRadius, 0.f));
		float2 r = normalize(vUVSet0 - vCenter);
		float3 cc = cross(float3(axis, 0), float3(r, 0));
		float b = dot(r, axis);
		float a = asin(cc.z); // acos(v);
		
		if (b > 0)
		{
			if (a > 0)
			{
				a = a;
			}
			else
			{
				a = 6.28f + a;
			}
		}
		else
		{
			if (a > 0)
			{
				a = 3.14f - a;
			}
			else
			{
				a = 3.14f - a;
			}
		}

		if (a_akArrayPrecent[iSlot] != 0.f)
		{
			vColor = float4(0.2f, 0.2f, 0.2f, 1.f);
		}
		
		if (a < a_akArrayPrecent[iSlot] * 3.1415926f * 2.f)
		{
			vColor = float4(1.f, 0.5f, 0.5f, 0.f);
		}
	}
	
	return vColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ShortCutBag(VS_OUTPUT kInput) : COLOR0
{	
	float4 vColor = float4(1.f, 0.5f, 0.5f, 0.f); // tex2D(s0, kInput.vUVSet);
	
//	float ff = g_fTime / 1.f;
//	for (int i = 0; i < 1; ++i)
//		a_akArrayPrecent[0] = fmod(ff, 1.f);
		
	float fHalfRadius = 0.5f;
	float2 vUVSet0 = kInput.vUVSet;
	
	float fExtendX = 6.f * kInput.vUVSet.x;
	float fExtendY = 2.f * kInput.vUVSet.y;
	float4 vGray = float4(0.5f, 0.5f, 0.5f, 0.f);
	int iSlotX = floor(fExtendX);
	int iSlotY = floor(fExtendY);
	
	float fExtendRadio = 0.2f;
//	vUVSet0.x = vUVSet0.x * fExtendRadio;
	vUVSet0.y = vUVSet0.y * fExtendRadio;
	
	float2 vCenter = float2(iSlotX * 0.1667f + 0.0833f, (iSlotY * 0.5f + 0.25f) * fExtendRadio);
	float2 r2 = vUVSet0 - vCenter;
//	if (((r2.x * r2.x) / 0.0069f + (r2.y * r2.y) / 0.06251f) < 1.f)
	if (length(r2) < 0.0833f)
	{
		vColor = vGray;
		
		float2 axis = normalize(float2(0.0833f, 0.f));
		float2 r = normalize(vUVSet0 - vCenter);
		float3 cc = cross(float3(axis, 0), float3(r, 0));
		float b = dot(r, axis);
		float a = asin(cc.z); // acos(v);
		
		if (b > 0)
		{
			if (a > 0)
			{
				a = a;
			}
			else
			{
				a = 6.28f + a;
			}
		}
		else
		{
			if (a > 0)
			{
				a = 3.14f - a;
			}
			else
			{
				a = 3.14f - a;
			}
		}

		int iSlot = iSlotX * 6 + iSlotY;		
		if (a < a_akArrayPrecent[iSlot] * 3.1415926f * 2.f)
		{
			vColor = float4(0.f, 0.f, 0.f, 0.f);
		}
	}
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique ShotCut_Screen0
<
	string Description = "ShotCut_Screen0";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_ShortCutScreen0 ();
	}
}
//---------------------------------------------------------------------------------------
technique ShotCut_Screen1
<
	string Description = "ShotCut_Screen1";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_ShortCutScreen1 ();
	}
}
//---------------------------------------------------------------------------------------
technique ShotCut_Bag
<
	string Description = "ShotCut_Bag";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_ShortCutBag ();
	}
}
//---------------------------------------------------------------------------------------