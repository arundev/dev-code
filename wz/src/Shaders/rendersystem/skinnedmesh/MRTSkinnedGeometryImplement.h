//*************************************************************************************************
// ����:	SkinnedMesh��MRTGeometryPass��ɫ������ʵ��(V2��,�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-21
// ����޸�:
//*************************************************************************************************
#include "skinned_mesh_common.h"

//---------------------------------------------------------------------------------------
// ȡ��һЩ�ڲ����õ���Ԥ�����
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef PS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

// ���ڶ���ͨ���������
#undef DIFFUSEMAP
#undef NORMALMAP
#undef GLOSSMAP
#undef LIGHTMAP
#undef ALPHATEST
#undef RIMLIGHT

// ���ڶ�����ɫ����������ṹ
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV

//---------------------------------------------------------------------------------------
// ����CHANNELS_FLAG��ֵ����ȡ��ǰ���õ�ͨ�����
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, DIFFUSEMAP_CHANNEL)
	#define DIFFUSEMAP 1
#else
	#define DIFFUSEMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, GLOSSMAP_CHANNEL)
	#define GLOSSMAP 1
#else
	#define GLOSSMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, LIGHTMAP_CHANNEL)
	#define LIGHTMAP 1
#else
	#define LIGHTMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, ALPHA_TEST_FLAG)
	#define ALPHATEST 1
#else
	#define ALPHATEST 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, RIM_LIGHTTING_CHANNEL)
	#define RIMLIGHT 1
#else
	#define RIMLIGHT 0
#endif

//---------------------------------------------------------------------------------------
// ���ݵ�ǰ���õ�ͨ����������������������
//---------------------------------------------------------------------------------------
#if (!NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	1
#else
	#define VS_OUTPUT_VERTEX_NORMAL 0
#endif

#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || DETAILNORMALMAP)
	#define VS_OUTPUT_BASE_UV 1
#else
	#define VS_OUTPUT_BASE_UV 0
#endif

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3 	vPos			: POSITION;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
#else
	float3	vNormal			: NORMAL;
	float3 	vBinormal		: BINORMAL;
	float3	vTangent		: TANGENT;
#endif

#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD0;
#endif
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fDepth			: TEXCOORD0;
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD1;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	float3 	vViewNormal		: TEXCOORD2;
#else
	float3	vViewNormal		: TEXCOORD2;
	float3 	vViewBinormal	: TEXCOORD3;
	float3	vViewTangent	: TEXCOORD4;
#endif
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
	kOutput.fDepth = dot(vWorldPos, g_mView._m02_m12_m22_m32);
	
#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vViewNormal = mul(float4(kInput.vNormal, 0.f), mSkinnedBoneTransform).xyz;
#else
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, mSkinnedBoneTransform,
		kOutput.vViewNormal, kOutput.vViewBinormal, kOutput.vViewTangent);
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
void PS_SHADER(VS_OUTPUT kInput, 				\
			   out float4 geoData	: COLOR0,	\
			   out float4 matData	: COLOR1
			   )
{
	// ����Diffuse_Tex * Diffuse_Mat
	// ��������ͼ��Alphaͨ����AlphaTest
	// @{
#if DIFFUSEMAP
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetBase);
	float3 vCompactedDiffuse = vDiffuseTex.rgb * g_vDiffuseMaterialColor;
	
	#ifdef ALPHATEST
		clip(vDiffuseTex.a - g_fAlphaTestRef);
	#endif
#else
	clip(-1);	// ������û�б���������ʾ
	float3 vCompactedDiffuse = g_vDiffuseMaterialColor;
#endif	
	// @}
	

	// ����Normal��Depth
	// @{
	// �۲�ռ���ȹ�һ����[0,1]
	float fDepth = kInput.fDepth / g_fCameraFarClip;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3 vViewNormal 	= kInput.vViewNormal;
#else
	// ����NormalMap
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;

	// ��һ��,��ת�����۲�����ϵ
	vNormalTex = normalize(vNormalTex);
	
	float3 vNorNormal 	= normalize(kInput.vViewNormal);
	float3 vNorBinormal = normalize(kInput.vViewBinormal);
	float3 vNorTangent 	= normalize(kInput.vViewTangent);
	float3 vViewNormal = mul(vNormalTex, float3x3(vNorTangent, vNorBinormal, vNorNormal));
#endif
	
	geoData	= float4(PackDepth(fDepth), PackNormal(vViewNormal));
	// @}
	
	
	// ����߹�ϵ��
	// @{
	// �������ֱ���������ṩ����,�Ӷ��Ż������dot����
	float fSpecMatLum = dot(g_vSpecularMaterial.rgb, g_vLuminScale2);
#if GLOSSMAP
	float fGlossSpec = tex2D(sdGlossSampler, kInput.vUVSetBase).r * fSpecMatLum;
#else
	//float fGlossSpec = fSpecMatLum;
	float fGlossSpec = 0.f;
#endif
	
	
	// ���������ͼ
	//	1.LightMap��Ϣ������Alphaͨ���ĸ�7bit
	//  2.RimLight��Ϣ������Alphaͨ�������1bit
	// @{
#if LIGHTMAP
	float fLightTex = a_fMainLightOcclusion;
#else
	float fLightTex = a_fMainLightOcclusion;	// ����Ҳ�ܵ�LightDepth�ڵ�
#endif

	// ������
	int iGlossSpec = fGlossSpec * 31.f + 0.5f;	///< gloss 5bit, lightmap 3bit
	int iGlossSpecShiftLeft3 = iGlossSpec * 8;
	int iLightMap = fLightTex * 7.f + 0.5f;
	matData.a = (iLightMap + iGlossSpecShiftLeft3) / 255.f;
	
	///< shiness 3bit
	///< ����ɫshinessд��Ϊ20, ����shiness����rimlight���ȿ�����
#if RIMLIGHT
	int iShiness = int(clamp(a_fRimBrightness, 0, 7));		///< 0-7�м�һ��ֵ
#else
	int iShiness = int(clamp(g_fShiness, 0, 7));			///< 0-7�м�һ��ֵ	
#endif
	
	int3 inter = 0;
	inter.x = iShiness / 4;
	int re = iShiness - (inter.x * 4);
	inter.y = re / 2;
	inter.z = re - (inter.y * 2);
	
	int3 vDiffuseLeastBit0 = int3(vCompactedDiffuse * 255 / 2) * 2;
	matData.rgb = float3(vDiffuseLeastBit0 + inter.xyz) / 255.f;	
	// @}
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRTSkinnedGeometry, CHANNELS_FLAG)
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_SHADER ();
		PixelShader 	= compile ps_3_0 PS_SHADER ();
	}
}
//---------------------------------------------------------------------------------------