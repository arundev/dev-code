//*************************************************************************************************
// 内容:	StaticMesh的MRTGeometryPass着色器具体实现(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-20
// 最后修改:	2013-05-28
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT_NORMALMAP
{
	float3 	vPos		: POSITION0;
	float3	vNormal		: NORMAL0;
	float3	vBinormal	: TANGENT0;
	float3	vTangent	: BINORMAL0;
	float2	vUVSet		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_VERTEXNORMAL
{
	float4 	vProjPos			: POSITION0;
	float4	vUVSetNormalDepth	: TEXCOORD0;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_NORMALMAP
{
	float4 	vProjPos			: POSITION0;
	float3	vUVSetUVDepth		: TEXCOORD0;
	float3	vUVSetViewNormal	: TEXCOORD1;
	float3	vUVSetViewBinormal	: TEXCOORD2;
	float3	vUVSetViewTangent	: TEXCOORD3;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_DIFFUSEMAP
{
	float4 	vProjPos			: POSITION0;
	float2	vUVSet				: TEXCOORD0;
	float4	vUVSetNormalDepth	: TEXCOORD1;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_GLOWMAP
{
	float4 	vProjPos			: POSITION0;
	float2	vUVSet				: TEXCOORD0;
	float	fUVSetDepth			: TEXCOORD1;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_ZONLY
{
	float4 	vProjPos			: POSITION0;
	float2	vUVSet				: TEXCOORD0;
};
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT_VERTEXNORMAL VS_Main_VertexNormal(float3 vPos : POSITION0, float3 vNormal : NORMAL)
{
	VS_OUTPUT_VERTEXNORMAL kOutput = (VS_OUTPUT_VERTEXNORMAL)0;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSetNormalDepth.xyz 	= vNormal;
	kOutput.vUVSetNormalDepth.w 	= mul(float4(vPos, 1.f), g_mWorldView._m02_m12_m22_m32);
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_NORMALMAP VS_Main_NormalMap(VS_INPUT_NORMALMAP kInput)
{
	VS_OUTPUT_NORMALMAP kOutput = (VS_OUTPUT_NORMALMAP)0;
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSetUVDepth.xy 	= kInput.vUVSet;
	kOutput.vUVSetUVDepth.z 	= mul(float4(kInput.vPos, 1.f), g_mWorldView._m02_m12_m22_m32);
	kOutput.vUVSetViewNormal	= mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz;
	kOutput.vUVSetViewBinormal	= mul(float4(kInput.vBinormal, 0.f), g_mWorldView).xyz;
	kOutput.vUVSetViewTangent	= mul(float4(kInput.vTangent, 0.f), g_mWorldView).xyz;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_DIFFUSEMAP VS_Main_DiffuseMap(float3 vPos : POSITION0, float3 vNormal : NORMAL, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT_DIFFUSEMAP kOutput = (VS_OUTPUT_DIFFUSEMAP)0;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet	= vUVSet;
	kOutput.vUVSetNormalDepth.xyz 	= vNormal;
	kOutput.vUVSetNormalDepth.z		= mul(float4(vPos, 1.f), g_mWorldView._m02_m12_m22_m32);
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_GLOWMAP VS_Main_GlowMap(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT_GLOWMAP kOutput = (VS_OUTPUT_GLOWMAP)0;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet		= vUVSet;
	kOutput.fUVSetDepth	= mul(float4(vPos, 1.f), g_mWorldView._m02_m12_m22_m32);
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_ZONLY VS_Main_ZOnly(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT_ZONLY kOutput = (VS_OUTPUT_ZONLY)0;
	kOutput.vProjPos 	= mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet		= vUVSet;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_VertexNormal(VS_OUTPUT_VERTEXNORMAL kInput) : COLOR
{
	float3 vNormal = kInput.vUVSetNormalDepth.xyz;
	vNormal = mul(float4(vNormal, 0.f), g_mWorldView).xyz;
	vNormal = normalize(vNormal);
	
	float fDepth = kInput.vUVSetNormalDepth.w;
	
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalMap(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float2 vUVSet = kInput.vUVSetUVDepth.xy;
	float3 vTexNormal = tex2D(sdNormalSampler, vUVSet).xyz;
	vTexNormal	 	= (vTexNormal - 0.5f) * 2.f;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale;
	vTexNormal 		= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_DetailNormalMap(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float3 vTexNormal = float3(0.f, 0.f, 1.f);
	
	float2 vUVSetDetailMap = kInput.vUVSetUVDepth.xy * a_vDetailNormalUVTiling;
	float3 vTexDetailNormal = tex2D(sdDetailNormalSampler, vUVSetDetailMap).xyz;
	vTexDetailNormal = (vTexDetailNormal - 0.5f) * 2.f;
	vTexDetailNormal *= a_vDetailNormalScale;
	
	vTexNormal		+= vTexDetailNormal;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale * saturate(1.f - (fDepth / a_fDetailNormalFadeDistance));
	vTexNormal  	= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalAndDetailNormalMap(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float2 vUVSet = kInput.vUVSetUVDepth.xy;
	float3 vTexNormal = tex2D(sdNormalSampler, vUVSet).xyz;
	vTexNormal	= (vTexNormal - 0.5f) * 2.f;
	
	float2 vUVSetDetailMap = vUVSet * a_vDetailNormalUVTiling;
	float3 vTexDetailNormal = tex2D(sdDetailNormalSampler, vUVSetDetailMap).xyz;
	vTexDetailNormal = (vTexDetailNormal - 0.5f) * 2.f;
	vTexDetailNormal *= a_vDetailNormalScale;
	
	vTexNormal		+= vTexDetailNormal;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale * saturate(1.f - (fDepth / a_fDetailNormalFadeDistance));
	vTexNormal  	= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalMap_AlphaTest(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float2 vUVSet = kInput.vUVSetUVDepth.xy;
	float4 vNormalTex = tex2D(sdNormalSampler, vUVSet);
	clip(vNormalTex.a - g_fAlphaTestRef);

	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float3 vTexNormal = vNormalTex.xyz;
	vTexNormal	 	= (vTexNormal - 0.5f) * 2.f;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale;
	vTexNormal 		= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_DetailNormalMap_AlphaTest(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float2 vUVSetDetailMap = kInput.vUVSetUVDepth.xy * a_vDetailNormalUVTiling;
	float4 vDetailNormalTex = tex2D(sdDetailNormalSampler, vUVSetDetailMap);
	clip(vDetailNormalTex.a - g_fAlphaTestRef);

	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float3 vTexNormal = float3(0.f, 0.f, 1.f);
	
	float3 vTexDetailNormal = vDetailNormalTex.xyz;
	vTexDetailNormal = (vTexDetailNormal - 0.5f) * 2.f;
	vTexDetailNormal *= a_vDetailNormalScale;
	
	vTexNormal		+= vTexDetailNormal;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale * saturate(1.f - (fDepth / a_fDetailNormalFadeDistance));
	vTexNormal  	= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalAndDetailNormalMap_AlphaTest(VS_OUTPUT_NORMALMAP kInput) : COLOR
{
	float2 vUVSet = kInput.vUVSetUVDepth.xy;
	float4 vNormalTex = tex2D(sdNormalSampler, vUVSet);
	clip(vNormalTex.a - g_fAlphaTestRef);

	float fDepth = kInput.vUVSetUVDepth.z / g_fCameraFarClip;
	
	float3 vTexNormal = vNormalTex.xyz;
	vTexNormal	= (vTexNormal - 0.5f) * 2.f;
	
	float2 vUVSetDetailMap = vUVSet * a_vDetailNormalUVTiling;
	float3 vTexDetailNormal = tex2D(sdDetailNormalSampler, vUVSetDetailMap).xyz;
	vTexDetailNormal = (vTexDetailNormal - 0.5f) * 2.f;
	vTexDetailNormal *= a_vDetailNormalScale;
	
	vTexNormal		+= vTexDetailNormal;
	vTexNormal.xy 	*= g_fStaticMeshNormalScale * saturate(1.f - (fDepth / a_fDetailNormalFadeDistance));
	vTexNormal  	= normalize(vTexNormal);
	
	float3 vVertexViewNormal = normalize(kInput.vUVSetViewNormal);
	float3 vVertexViewBinormal = normalize(kInput.vUVSetViewBinormal);
	float3 vVertexViewTangent = normalize(kInput.vUVSetViewTangent);
	float3 vViewNormal = mul(vTexNormal, 
		float3x3(vVertexViewTangent, vVertexViewBinormal, vVertexViewNormal));
		
	return float4(PackDepth(fDepth), PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_DiffuseMap_AlphaTest(VS_OUTPUT_DIFFUSEMAP kInput) : COLOR
{
	clip(tex2D(sdBaseSampler, kInput.vUVSet).a - g_fAlphaTestRef);

	float3 vNormal = kInput.vUVSetNormalDepth.xyz;
	vNormal = mul(float4(vNormal, 0.f), g_mWorldView).xyz;
	vNormal = normalize(vNormal);
	
	float fDepth = kInput.vUVSetNormalDepth.w;
	
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_GlowMap(VS_OUTPUT_GLOWMAP kInput) : COLOR
{
	float fDepth = kInput.fUVSetDepth;
	return float4(PackDepth(fDepth), PackNormal(float3(0.f, 0.f, 0.f)));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_GlowMap_AlphaTest(VS_OUTPUT_GLOWMAP kInput) : COLOR
{
	clip(tex2D(sdGlowSampler, kInput.vUVSet).a - g_fAlphaTestRef);
	float fDepth = kInput.fUVSetDepth;
	return float4(PackDepth(fDepth), PackNormal(float3(0.f, 0.f, 0.f)));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ZOnly(VS_OUTPUT_ZONLY kInput) : COLOR
{
	return 0.f;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ZOnly_AlphaTest(VS_OUTPUT_ZONLY kInput) : COLOR
{
	clip(tex2D(sdBaseSampler, kInput.vUVSet).a - g_fAlphaTestRef);
	return 0.f;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique StaticMesh_VertexNormal_Geometry
<
	string Description = "StaticMesh_VertexNormal_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_VertexNormal ();
		PixelShader 	= compile ps_2_a PS_Main_VertexNormal ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_NormalMap_Geometry
<
	string Description = "StaticMesh_NormalMap_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_NormalMap ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_DetailNormalMap_Geometry
<
	string Description = "StaticMesh_DetailNormalMap_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_DetailNormalMap ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_NormalAndDetailNormalMap_Geometry
<
	string Description = "StaticMesh_NormalAndDetailNormalMap_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_NormalAndDetailNormalMap ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_NormalMapAlphaTest_Geometry
<
	string Description = "StaticMesh_NormalMapAlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_NormalMap_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_DetailNormalMap_AlphaTest_Geometry
<
	string Description = "StaticMesh_DetailNormalMap_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_DetailNormalMap_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_NormalAndDetailNormalMap_AlphaTest_Geometry
<
	string Description = "StaticMesh_NormalAndDetailNormalMap_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_NormalMap ();
		PixelShader 	= compile ps_2_a PS_Main_NormalAndDetailNormalMap_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_DiffuseMap_AlphaTest_Geometry
<
	string Description = "StaticMesh_DiffuseMap_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_DiffuseMap ();
		PixelShader 	= compile ps_2_a PS_Main_DiffuseMap_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_GlowMap_Geometry
<
	string Description = "StaticMesh_GlowMap_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_GlowMap ();
		PixelShader 	= compile ps_2_a PS_Main_GlowMap ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_GlowMap_AlphaTest_Geometry
<
	string Description = "StaticMesh_GlowMap_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_GlowMap ();
		PixelShader 	= compile ps_2_a PS_Main_GlowMap_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_ZOnly_Geometry
<
	string Description = "StaticMesh_ZOnly_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_ZOnly ();
		PixelShader 	= compile ps_2_a PS_Main_ZOnly ();
	}
}
//---------------------------------------------------------------------------------------
technique StaticMesh_ZOnly_AlphaTest_Geometry
<
	string Description = "StaticMesh_ZOnly_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_ZOnly ();
		PixelShader 	= compile ps_2_a PS_Main_ZOnly_AlphaTest ();
	}
}
//---------------------------------------------------------------------------------------