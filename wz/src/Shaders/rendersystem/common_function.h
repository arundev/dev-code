//*************************************************************************************************
// ����:	������ɫ���Ĺ�������/�궨��/����
//			�����Ǻ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:
//*************************************************************************************************
#ifndef _COMMON_FUNCTION_H__
#define _COMMON_FUNCTION_H__


//*****************************************************************************
// ���ͺ���
//*****************************************************************************
//-----------------------------------------------------------------------------
float4 mad(float4 m, float4 a, float4 b) 
{
    return m * a + b;
}
//-----------------------------------------------------------------------------
float square(float a)
{
	return a * a;
}
//-----------------------------------------------------------------------------
float sqrlength(float3 vec)
{
	return dot(vec, vec);
}
//-----------------------------------------------------------------------------
float2 LinePlaneIntersection(float fRelativeB, float2 vRelativeZ)
{
	static const float FLT_EPSILON = 0.001f;
	return vRelativeZ / (abs(fRelativeB) <= FLT_EPSILON ? FLT_EPSILON : fRelativeB);
}
//-----------------------------------------------------------------------------
float2 GetTexCoordForPixel(float2 vPixelCoord)
{
	return vPixelCoord * float2(0.5, -0.5) + g_vPixToTexOffset;
}
//-----------------------------------------------------------------------------
float3 SafeNormalize(float3 V)
{
	return V / sqrt(max(dot(V, V), 0.0001f));
}
//*****************************************************************************
// ����Gamebryo Shader Node
//*****************************************************************************
void TextureRGBASample(float2 vUVSet, sampler sdSampler, bool bSaturate, out float4 vColor)
{
	vColor = tex2D(sdSampler, vUVSet);
	if (bSaturate)
	{
		vColor = saturate(vColor);
	}
}
//-----------------------------------------------------------------------------
void CalculateNormalFromColor(float4 vNormalTex, 
	float3 vNormal, float3 vBinormal, float3 vTangent, 
	int iNormalTexType, out float3 vOutNormal)
{
	vNormalTex = vNormalTex * 2.f - 1.f;
	vNormalTex.xy *= g_fTerrainNormalScale;
	
	// Do nothing extra for standard handle compressed types
	if (iNormalTexType == 1)		// DXN
	{
		vNormalTex.xy = vNormalTex.xy;
		vNormalTex.z = sqrt(1.f - vNormalTex.x * vNormalTex.x - vNormalTex.y * vNormalTex.y);
	}
	else if (iNormalTexType == 2)	// DXT5 
	{
		vNormalTex.xy = vNormalTex.ag;
		vNormalTex.z = sqrt(1.f - vNormalTex.x * vNormalTex.x - vNormalTex.y * vNormalTex.y);
	}
	
	float3x3 mForm = float3x3(vTangent, vBinormal, vNormal);
	vOutNormal = mul(vNormalTex.xyz, mForm);
	vOutNormal = normalize(vOutNormal);
}
//*****************************************************************************
// �������߲�ֵ
//*****************************************************************************
//-----------------------------------------------------------------------------
// Hermite��������
//float GetHermiteInterpolation(sampler sdSampler, float2 vRange, float fSampleValue, float fScale, int iTableSize)
//{
//	float fClampValue = saturate((fSampleValue - vRange.x) / (vRange.y - vRange.x)); 
//	float fIndexedValue = fClampValue * iTableSize;
//	int 
//	re
//}
//-----------------------------------------------------------------------------
// Bezier��������
float GetBezierValue(sampler sdSampler, float2 vRange, float fSampleValue, float fScale)
{
	float fClampValue = saturate((fSampleValue - vRange.x) / (vRange.y - vRange.x));
	float2 vUVSet = float2(fClampValue, 0.5);
	float4 vColor = tex2D(sdSampler, vUVSet);
	
	return vColor.a * fScale;
}

//*****************************************************************************
// ���ѹ��
//*****************************************************************************
float2 PackDepth(float depth)
{
	float depth_int, depth_frac;
	depth_frac = modf(sqrt(depth) * 256.0f, depth_int);
	return float2(depth_int / 255.0f, depth_frac);
}
//-----------------------------------------------------------------------------
float UnpackDepth(float2 enc)
{
	float depth = dot(enc.xy, float2(255.0f, 1.0f)) * (1.0f / 256.0f);
	depth = depth * depth;
	return depth;
}

//*****************************************************************************
// ����ѹ��
// �ο�
//*****************************************************************************
//-----------------------------------------------------------------------------
// Spheremap Transform, WorldZero����
//-----------------------------------------------------------------------------
float2 PackNormal(float3 normal)
{
	float f = sqrt(dot(float2(-normal.z, 1.0f), float2(8.0f, 8.0f)));
	return normal.xy / f + 0.5f;
}
//-----------------------------------------------------------------------------
float3 UnpackNormal(float2 enc)
{
	float2	fenc 	= enc * 4.0f - 2.0f;
	float	f		= dot(fenc, fenc);
	float 	g 		= sqrt(1.0f - f / 4.0f);
	float3	normal	= float3(fenc * g, f * 0.5f - 1.0f);
	return normal;
}
//-----------------------------------------------------------------------------
// Spheremap Transform, Crytek����
//-----------------------------------------------------------------------------
float2 PackNormal_Crytek(float3 normal)
{
   return normalize(normal.xy) * sqrt(normal.z * 0.5 + 0.5);
}
//-----------------------------------------------------------------------------
float3 UnpackNormal_Crytek(float2 enc)
{
   float3 normal;
   normal.z = dot(enc, enc) * 2 - 1;
   normal.xy = normalize(enc) * sqrt(1 - normal.z * normal.z);
   return normal;
}

//*****************************************************************************
// ��ɫ�ռ�
//*****************************************************************************
//float4 ToLinear(float4 color)
//{
//	return float4(pow(color.rgb, g_fGammaValue), color.a);
//}
//-----------------------------------------------------------------------------
//float3 ToLinear(float3 color)
//{
//	return float3(pow(color.rgb, g_fGammaValue));
//}
//-----------------------------------------------------------------------------
//float4 ToGamma(float4 color)
//{
//	return float4(pow(color.rgb, 1.0f / g_fFinalGammaValue), color.a);
//}
//-----------------------------------------------------------------------------
float3 RGB2XYZ(float3 vRGB)
{
	return float3(
		dot(vRGB, float3(0.5141364f, 0.3238786f,  0.16036376f)),
		dot(vRGB, float3(0.265068f,  0.67023428f, 0.06409157f)),
		dot(vRGB, float3(0.0241188f, 0.1228178f,  0.84442666f)));
}
//-----------------------------------------------------------------------------
float3 XYZ2RGB(float3 vXYZ)
{
	return float3(
		dot(vXYZ, float3(2.5651f, -1.1665f, -0.3986f)),
		dot(vXYZ, float3(-1.0217f, 1.9777f, 0.0439f)),
		dot(vXYZ, float3(0.0753f, -0.2543f, 1.1892f)));
}
//-----------------------------------------------------------------------------
float3 RGB2Yxy(float3 vRGB)
{
	float3 vXYZ = RGB2XYZ(vRGB);
	
	float3 vYxy;
	vYxy.r = vXYZ.g;
	vYxy.gb = vXYZ.rg / dot(float3(1.f, 1.f, 1.f), vXYZ);
	
	return vYxy;
}
//-----------------------------------------------------------------------------
float3 Yxy2RGB(float3 vYxy)
{
	float3 vXYZ;
	vXYZ.r = vYxy.r * vYxy.g / vYxy.b;
	vXYZ.g = vYxy.r;
	vXYZ.b = vYxy.r * (1.f - vYxy.g - vYxy.b) / vYxy.b;
	
	return XYZ2RGB(vXYZ);
}

//*****************************************************************************
// �����任
//*****************************************************************************
//
//-----------------------------------------------------------------------------
void TransformSkinnedPosition(float3 vPosition, 
							int4 iBlendIndices, 
							float3 vBlendWeights, 
							BONEMATRIX_TYPE mBones[MAX_BONES],
							out float4 vWorldPos,
							out float4x4 mSkinnedBoneTransform)
{
	// ���������Ƥ����ϵ(skin)ת������������ϵ(world)
	// ��ϳ�������λ�úͷ��ߴ���Ƥ����ϵת����������ϵ
	float fWeight3 = 1.0f - vBlendWeights[0] - vBlendWeights[1] - vBlendWeights[2];
	BONEMATRIX_TYPE mShortSkinnedBoneTransform;
	mShortSkinnedBoneTransform 	= mBones[iBlendIndices[0] * vBlendWeights[0]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[1] * vBlendWeights[1]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[2] * vBlendWeights[2]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[3] * fWeight3];
	mSkinnedBoneTransform = float4x4(
		mShortSkinnedBoneTransform[0], 0.0f,
		mShortSkinnedBoneTransform[1], 0.0f,
		mShortSkinnedBoneTransform[2], 0.0f,
		mShortSkinnedBoneTransform[3], 1.0f);
		
	// ������ת����������ϵ
	vWorldPos.xyz = mul(float4(vPosition, 1.0f), mSkinnedBoneTransform).xyz;
	vWorldPos.w = 1.0f;
}
//-----------------------------------------------------------------------------
void TransformSkinnedPositionPrecision(float3 vPosition, 
							int4 iBlendIndices, 
							float3 vBlendWeights, 
							BONEMATRIX_TYPE mBones[MAX_BONES],
							out float4 vWorldPos,
							out float4x4 mSkinnedBoneTransform)
{
	// ���������Ƥ����ϵ(skin)ת������������ϵ(world)
	// ��ϳ�������λ�úͷ��ߴ���Ƥ����ϵת����������ϵ
	float fWeight3 = 1.0f - vBlendWeights[0] - vBlendWeights[1] - vBlendWeights[2];
	BONEMATRIX_TYPE mShortSkinnedBoneTransform;
	mShortSkinnedBoneTransform 	= mBones[iBlendIndices[0] * vBlendWeights[0]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[1] * vBlendWeights[1]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[2] * vBlendWeights[2]];
	mShortSkinnedBoneTransform += mBones[iBlendIndices[3] * fWeight3];
	mSkinnedBoneTransform = float4x4(
		mShortSkinnedBoneTransform[0], 0.0f,
		mShortSkinnedBoneTransform[1], 0.0f,
		mShortSkinnedBoneTransform[2], 0.0f,
		mShortSkinnedBoneTransform[3] - g_vCameraPostion.xyz, 1.0f);	///< ����Ϊʲô��ȥ���λ��
		
	// ������ת����������ϵ
	vWorldPos.xyz = mul(float4(vPosition, 1.0f), mSkinnedBoneTransform).xyz;
	vWorldPos.w = 1.0f;
}
//-----------------------------------------------------------------------------
void TransformNBT(float3 vNormal, 
				float3 vBinormal, 
				float3 vTargent, 
				float4x4 mSkinnedWorldView,
				out float3 vViewNormal, 
				out float3 vViewBinormal,
				out float3 vViewTargent)
{
	// Transform the normal into world space for lighting
	// ����ľ�й淶��,����PixelShader�й淶��,��Ϊ����淶���󾭹���դ��ֵҲ�����ٱ��ֹ淶��
	vViewNormal = mul(vNormal, (float3x3)mSkinnedWorldView);
	vViewBinormal = mul(vBinormal, (float3x3)mSkinnedWorldView);
	vViewTargent = mul(vTargent, (float3x3)mSkinnedWorldView);	
}
//-----------------------------------------------------------------------------

//*****************************************************************************
// ���ռ���
//*****************************************************************************
void AccumLighting(float3 V, float3 N, float fShiness, float fDarkMap, float fLightMap, float4 vLocalLight, out float3 vDiffuseLight, out float3 vSpecularLight)
{
	// 
	float4 vShadowFactor = float4(fDarkMap, fDarkMap, fDarkMap, fLightMap);
	vShadowFactor = saturate(1.f + g_vLightFactor * (vShadowFactor - 1.f));
	
	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float3 R = reflect(g_vMainLightDir, N);
	
	float NL = saturate(dot(N, L));
	float VR = saturate(dot(V, R));
	
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * vShadowFactor.w * vShadowFactor.y);
	vSpecularLight	= vDiffuseLight * pow(VR, fShiness);
	// @}
	
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	R = reflect(g_vAssistLightDir, N);
	
	NL = saturate(dot(N, L));
	VR = saturate(dot(V, R));
	
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL * vShadowFactor.z);
	vSpecularLight	+= vDiffuseLight * pow(VR, fShiness);
	// @}

	
	// �ϳɾֲ�����(��ֵĺϳ��㷨)
	// @{
	vLocalLight *= g_fLocalLightRange;
	
	vDiffuseLight	+= vLocalLight.rgb;
	vSpecularLight	+= vLocalLight.rgb / (dot(vLocalLight.rgb, g_vLuminScale) + 0.001f) * vLocalLight.a;
	// @}
	
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
	//vDiffuseLight += g_vSkyAmbientColor;
}
//-----------------------------------------------------------------------------
void AccumLighting(float3 V, float3 N, float fShiness, float fLightMap, float4 vLocalLight, out float3 vDiffuseLight, out float3 vSpecularLight)
{
	// 
	fLightMap = saturate(1.0f + g_fMainLightLightMapFactor * (fLightMap - 1.0f));
	
	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float3 R = reflect(g_vMainLightDir, N);
	
	float NL = saturate(dot(N, L));
	float VR = saturate(dot(V, R));
	
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * fLightMap);
	vSpecularLight	= vDiffuseLight * pow(VR, fShiness);
	// @}
	
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	R = reflect(g_vAssistLightDir, N);
	
	NL = saturate(dot(N, L));
	VR = saturate(dot(V, R));
	
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL);
	vSpecularLight	+= vDiffuseLight * pow(VR, fShiness);
	// @}

	
	// �ϳɾֲ�����(��ֵĺϳ��㷨)
	// @{
	vLocalLight *= g_fLocalLightRange;
	
	vDiffuseLight	+= vLocalLight.rgb;
	vSpecularLight	+= vLocalLight.rgb / (dot(vLocalLight.rgb, g_vLuminScale) + 0.001f) * vLocalLight.a;
	// @}
	
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
}
//-----------------------------------------------------------------------------
void AccumLighting(float3 V, float3 N, float fShiness, out float3 vDiffuseLight, out float3 vSpecularLight)
{
	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float3 R = reflect(g_vMainLightDir, N);
	
	float NL = saturate(dot(N, L));
	float VR = saturate(dot(V, R));
	
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL);
	vSpecularLight	= vDiffuseLight * pow(VR, fShiness);
	// @}
	
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	R = reflect(g_vAssistLightDir, N);
	
	NL = saturate(dot(N, L));
	VR = saturate(dot(V, R));
	
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL);
	vSpecularLight	+= vDiffuseLight * pow(VR, fShiness);
	// @}
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
}
//-----------------------------------------------------------------------------
void AccumLightingOnlyDiffuse(float3 N, float fDarkMap, float fLightMap, float4 vLocalLight, out float3 vDiffuseLight)
{
	// 
	float4 vShadowFactor = float4(fDarkMap, fDarkMap, fDarkMap, fLightMap);
	vShadowFactor = saturate(1.f + g_vLightFactor * (vShadowFactor - 1.f));

	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float NL = saturate(dot(N, L));
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * vShadowFactor.w * vShadowFactor.y);
	// @}
		
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	NL = saturate(dot(N, L));
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL * vShadowFactor.z);
	// @}
	
	// �ϳɾֲ�����(��ֵĺϳ��㷨)
	vLocalLight *= g_fLocalLightRange;
	vDiffuseLight += vLocalLight.rgb;
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
}
//-----------------------------------------------------------------------------
void AccumLightingOnlyDiffuse(float3 N, float fLightMap, float4 vLocalLight, out float3 vDiffuseLight)
{
	// 
	float fShadowFactor = saturate(1.f + g_fMainLightLightMapFactor * (fLightMap - 1.f));
	
	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float NL = saturate(dot(N, L));
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL * fShadowFactor);
	// @}
		
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	NL = saturate(dot(N, L));
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL);
	// @}
	
	// �ϳɾֲ�����(��ֵĺϳ��㷨)
	vLocalLight *= g_fLocalLightRange;
	vDiffuseLight += vLocalLight.rgb;
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
}
//-----------------------------------------------------------------------------
void AccumLightingOnlyDiffuse(float3 N, float4 vLocalLight, out float3 vDiffuseLight)
{
	// ���㲢�ϳ�ȫ���������(��ֵļ����㷨)
	// @{
	float3 L = -g_vMainLightDir;
	float NL = saturate(dot(N, L));
	vDiffuseLight 	= lerp(g_vMainLightAmbientColor, g_vMainLightColor, NL);
	// @}
		
	// ���㲢�ϳ�ȫ�ָ��������(��ֵļ����㷨)
	// @{
	L = -g_vAssistLightDir;
	NL = saturate(dot(N, L));
	vDiffuseLight 	+= lerp(g_vAssistLightAmbientColor, g_vAssistLightColor, NL);
	// @}
	
	// �ϳɾֲ�����(��ֵĺϳ��㷨)
	vLocalLight *= g_fLocalLightRange;
	vDiffuseLight += vLocalLight.rgb;
	
	// �ϳ�ȫ�ֻ�������(�ѷ���ת��������ռ�,Ȼ���ֵ��������ջ�����)
	vDiffuseLight	+= lerp(g_vTerraimAmbientColor, g_vSkyAmbientColor, saturate(dot(N, g_mInvViewT._m20_m21_m22)));
}
//---------------------------------------------------------------------------------------
// ����ȫ����
//---------------------------------------------------------------------------------------
float4 CalcHeightFogCurveFactor(sampler sdSampler, float4 vWorldPos, bool bSky)
{
	float fSkyHeight = g_fFogCameraFar / 5000.0 * vWorldPos.z;
	float fDist = bSky ? sqrt(g_fFogCameraFar * g_fFogCameraFar - fSkyHeight * fSkyHeight) * g_fFogSkyDensity : vWorldPos.w;
	float fDistInFront = fDist - g_fFogStartDistance;
	if (fDistInFront < 0)
		return float4(0, 0, 0, 1.0);
		
	// 
	float2 vRelativeZ = float2(g_fFogStartHeight - g_vViewPos.z, g_fFogEndHeight - g_vViewPos.z);
	float2 vHeightPercent = saturate(LinePlaneIntersection(vWorldPos.z - g_vViewPos.z, vRelativeZ));
	
	// fog layer distance based on % over max/min fog heights
	float fLayerDistance = fDistInFront * abs(vHeightPercent.y - vHeightPercent.x);
	float fNegFogLineIntegral = g_fFogDistanceScale * fLayerDistance;
	
	// clamp anything beyond the extinction distance to 0 scattering
	// scattering falloff is exponential based on normalized layer distance
	float fScattering = 0.0;
	if (fLayerDistance < g_fFogExtinctionDistance)
		fScattering = 1.0 - GetBezierValue(sdSampler, float2(g_fFogStartDistance, g_fFogEndDistance), fLayerDistance, 1.0);

	// radio of the inscattering color to be used
	float fInScattering = fScattering - 1.0;
	float3 vFogColor = fInScattering * g_vFogColor;
	
	return float4(vFogColor, fScattering);
}
//---------------------------------------------------------------------------------------
#endif