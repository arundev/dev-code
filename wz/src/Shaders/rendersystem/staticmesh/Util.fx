//*************************************************************************************************
// 内容:	StaticMesh的Debug着色器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:	2013-05-28
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
float4	VS_Main(float3 vPos : POSITION)	: POSITION
{
	return mul(float4(vPos, 1.0f), g_mWorldViewProj);
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 	PS_Main_Wireframe() : COLOR0
{
	return g_vWireframeColor;
}
//---------------------------------------------------------------------------------------
float4 	PS_Main_Solid() : COLOR0
{
	return g_vSolidColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique StaticMesh_Wireframe
<
	string	Description 		= "StaticMesh_Wireframe";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a 	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_Wireframe();
		
		//CullMode			= NONE;
		//FillMode			= WIREFRAME	
		//ZEnable 			= TRUE;
		//ZWriteEnable 		= FALSE;
		//ZFunc				= LESSEQUAL;
		//AlphaTestEnable 	= FALSE;
		//ALphaBlendEnable	= FALSE;
		//StencilEnable		= FALSE;
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_Solid
<
	string	Description 		= "StaticMesh_Solid";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a 	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_Solid();
		
		//CullMode			= NONE;
		//FillMode			= SOLID	
		//ZEnable 			= TRUE;
		//ZWriteEnable 		= TRUE;
		//ZFunc				= LESSEQUAL;
		//AlphaTestEnable 	= FALSE;
		//ALphaBlendEnable	= FALSE;
		//StencilEnable		= FALSE;
	}
}
//---------------------------------------------------------------------------------------