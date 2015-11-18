//*************************************************************************************************
// ����:	StaticMesh Emissive Effect��ɫ��(�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-21
// ����޸�:	2013-09-10
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;
	float2 	vUVSet		: TEXCOORD0;
	float4	vUVSetColor	: TEXCOORD1;

};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_VertexColor(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0, float4 vColor : COLOR0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet = vUVSet;
	kOutput.vUVSetColor = vColor;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Material(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0, float4 vColor : COLOR0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet = vUVSet;
	kOutput.vUVSetColor = g_vEmissiveMaterial;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Solid(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = tex2D(sdBaseSampler, kInput.vUVSet);
	return float4(vColor.rgb * kInput.vUVSetColor.rgb, vColor.a * kInput.vUVSetColor.a);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_AlphaTest(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = tex2D(sdBaseSampler, kInput.vUVSet);
	clip(vColor.a - g_fAlphaTestRef);
	return float4(vColor.rgb * kInput.vUVSetColor.rgb, vColor.a * kInput.vUVSetColor.a);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique StaticMesh_VertexColor_Solid
<
	string	Description 		= "StaticMesh_VertexColor_Solid";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main_VertexColor();
		PixelShader		= compile	ps_2_a 	PS_Main_Solid();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_VertexColor_AlphaTest
<
	string	Description 		= "StaticMesh_VertexColor_AlphaTest";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main_VertexColor();
		PixelShader		= compile	ps_2_a 	PS_Main_AlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_Material_Solid
<
	string	Description 		= "StaticMesh_Material_Solid";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main_Material();
		PixelShader		= compile	ps_2_a 	PS_Main_Solid();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_Material_AlphaTest
<
	string	Description 		= "StaticMesh_Material_AlphaTest";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main_Material();
		PixelShader		= compile	ps_2_a 	PS_Main_AlphaTest();
	}
}
//---------------------------------------------------------------------------------------