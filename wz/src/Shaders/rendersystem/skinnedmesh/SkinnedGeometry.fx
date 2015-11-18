//*************************************************************************************************
// 内容:	SkinnedMesh的MRTGeometryPass着色器具体实现(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "skinned_mesh_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT_VERTEXNORMAL
{
	float3 	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	float3	vNormal			: NORMAL0;
	float2	vUVSet			: TEXCOORD0;
};
//---------------------------------------------------------------------------------------
struct VS_INPUT_NORMALMAP
{
	float3 	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	float3	vNormal			: NORMAL0;
	float3	vBinormal		: TANGENT0;
	float3	vTangent		: BINORMAL0;
	float2	vUVSet			: TEXCOORD0;
};
//---------------------------------------------------------------------------------------
struct VS_INPUT_ZONLY
{
	float3 	vPos			: POSITION0;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	float2	vUVSet			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_VERTEXNORMAL
{
	float4 	vProjPos			: POSITION0;
	float3	vUVSetUVDepth		: TEXCOORD0;
	float3 	vUVSetViewNormal	: TEXCOORD1;
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
VS_OUTPUT_VERTEXNORMAL VS_Main_VertexNormal(VS_INPUT_VERTEXNORMAL kInput)
{
	VS_OUTPUT_VERTEXNORMAL kOutput = (VS_OUTPUT_VERTEXNORMAL)0;

	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
	kOutput.vUVSetUVDepth.xy	= kInput.vUVSet;
	kOutput.vUVSetUVDepth.z		= mul(vWorldPos, g_mView._m02_m12_m22_m32);
	
	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
	kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), mSkinnedBoneTransform).xyz;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_NORMALMAP VS_Main_NormalMap(VS_INPUT_NORMALMAP kInput)
{
	VS_OUTPUT_NORMALMAP kOutput = (VS_OUTPUT_NORMALMAP)0;

	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
	kOutput.vUVSetUVDepth.xy	= kInput.vUVSet;
	kOutput.vUVSetUVDepth.z		= mul(vWorldPos, g_mView._m02_m12_m22_m32);
	
	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, mSkinnedBoneTransform,
		kOutput.vUVSetViewNormal, kOutput.vUVSetViewBinormal, kOutput.vUVSetViewTangent);
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_ZONLY VS_Main_ZOnly(VS_INPUT_ZONLY kInput)
{
	VS_OUTPUT_ZONLY kOutput = (VS_OUTPUT_ZONLY)0;
	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos 	= mul(vWorldPos, g_mViewProj);
	kOutput.vUVSet		= kInput.vUVSet;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_VertexNormal(VS_OUTPUT_VERTEXNORMAL kInput) : COLOR
{
	float3 vNormal = normalize(kInput.vUVSetViewNormal);
	
	float fDepth = kInput.vUVSetUVDepth.z;
	
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_VertexNormal_AlphaTest(VS_OUTPUT_VERTEXNORMAL kInput) : COLOR
{
	float2 vUVSet = kInput.vUVSetUVDepth.xy;
	clip(tex2D(sdNormalSampler, vUVSet).a - g_fAlphaTestRef);

	float3 vNormal = normalize(kInput.vUVSetViewNormal);
	
	float fDepth = kInput.vUVSetUVDepth.z;
	
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
technique SkinnedMesh_VertexNormal_Geometry
<
	string Description = "SkinnedMesh_VertexNormal_Geometry";
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
technique SkinnedMesh_VertexNormal_AlphaTest_Geometry
<
	string Description = "SkinnedMesh_VertexNormal_AlphaTest_Geometry";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_VertexNormal ();
		PixelShader 	= compile ps_2_a PS_Main_VertexNormal_AlphaTest ();
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
technique StaticMesh_NormalMap_AlphaTest_Geometry
<
	string Description = "StaticMesh_NormalMap_AlphaTest_Geometry";
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