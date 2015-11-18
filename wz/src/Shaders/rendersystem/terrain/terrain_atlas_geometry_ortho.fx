//*************************************************************************************************
// ����:	����Normal/Depth���Ƶ�G-Buffer
//---------------------------------------------------------
// ����:		
// ����:		2012-08-23
// ����޸�:	2013-05-02
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_CUBESAMPLE(0, sdPlanarTableSampler, 	sdPlanarTableTex);				//
SD_POINT_CLAMP_SAMPLE(1, sdDepthSampler, 			sdDepthBuf, 		false);		// ������Ļ��Ȼ���
SD_LINEAR_WRAP_SAMPLE(2, sdBaseNormalSampler, 		sdBaseNormalTex, 	false);		// ���λ���������ͼ
SD_POINT_CLAMP_SAMPLE(3, sdTileSampler,				sdTileTex,			false);		// ����TileMap
SD_LINEAR_WRAP_SAMPLE(4, sdBlendSampler,			sdBlendTex,			false);		// ����BlendMap
SD_POINT_CLAMP_SAMPLE(5, sdAtlasTableSampler,		sdAtlasTableTex,	false);		// ���η�����ͼ��ѯ��
SD_LINEAR_WRAP_SAMPLE(6, sdNormalAtlasSampler,		sdNormalAtlasTex,	false);		// ���η�����ͼͼ��

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ���
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ������������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float3	vUVNearClipWorldPos	: TEXCOORD1;	// ��ǰ���Ӧ���ü����ϵĵ����������
	float3	vUVFarClipWorldPos	: TEXCOORD2;	// ��ǰ���ӦԶ�ü����ϵĵ����������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// ��ǰ���Ӧ��Զ�ü����ϵĵ����������
	kOutput.vUVNearClipWorldPos = mul(float4(kInput.vPos.xy, 0.f, 1.f), g_mDepthToWorld).xyz;
	kOutput.vUVFarClipWorldPos  = mul(float4(kInput.vPos.xy, 1.f, 1.f), g_mDepthToWorld).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ��ȾԶ��,ֻ��Ⱦ����������ͼ
float4 PS_Main_Far_BaseNormal(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�Խ�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// �ü���ָ����ƽ�����ڵ�����
	clip(length(vWorldPos - kInput.vUVNearClipWorldPos) - g_fTerrainFarStart);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex 	= tex2D(sdBaseNormalSampler, vUVSet);
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// �任Normal���۲�ռ�(���terrain_atlas_geometry.fx)
	float3 vViewNormal = mul(float4(vWorldNormal, 0.f), g_mView).xyz;
	// @}
	
	// �������ķ��ߺ����
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// ��Ⱦ����,��Ⱦ�������ߺͷ�����ͼ
float4 PS_Main_Near_BaseNormalAndNormalMap(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�Խ�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float2 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet).xy;
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));	
	// @}
	
	// TileMap
	// @{
	// ����UV����TileMap(�����[0,1]�ָ���[0,255]��ͼ����������)
	float4 vIndices = tex2D(sdTileSampler, vUVSet) * 255.f;
	// @}
	
	// BlendMap
	// @{
	// �����µ�UV(���terrain_atlas_geometry.fx)
	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	
	// ����UV����BlendMap
	float3 vBlendTex = tex2D(sdBlendSampler, vUVSet2).rgb;
	
	// ��һ��Ȩ��
	float fTotalWeight = dot(vBlendTex, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	// @}
	
	// �������߿ռ�
	// @{
	// ��������������(���terrain_atlas_geometry.fx)
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	
	// �����µĵ���UV(���terrain_atlas_geometry.fx)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	
	// ���㵱ǰ������߿ռ�
	float3 vWorldBinormal 	= cross(float3(vPlanarVec.xy, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	// ���߻��(NormalAtlasTable, NormalAtlas)
	// @{
	// ���㵱ǰ���ص��۲��ʸ��
	float3 vWorldViewVector = kInput.vUVNearClipWorldPos - vWorldPos;
	float fWorldDistance = length(vWorldViewVector);
	vWorldViewVector /= fWorldDistance;
	
	// ���㵱ǰ����ӦȡLOD(���ﲻ��,�д���һ����ע)(��,�̶�����)
	float fLodLevel = log2(2.f * fWorldDistance / 768.f / max(sqrt(dot(vWorldViewVector, vWorldNormal)), 0.25f));
	
	// ����ͼ��UV(���terrain_atlas_geometry.fx)
	float4 vUVSetTableU = saturate(vIndices.bgra * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
	float vUVSetTableV	= saturate(fLodLevel * g_fNormalAtlasLevelScale + g_fNormalAtlasLevelOffset);
	
	// ��ͼ���
	float3 vNormal = SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.x, vUVSetTableV), vUVSet3) * vBlendTex.b +
					 SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.y, vUVSetTableV), vUVSet3) * vBlendTex.g +
					 SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.z, vUVSetTableV), vUVSet3) * vBlendTex.r;
	
	// �任DetailNormal(��[0,1]��[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// ����DetailNormal
	vNormal.xy *= g_fTerrainNormalScale;
	
	// ��Զ����������������(���terrain_atlas_geometry.fx)
	float fNormalSmooth = saturate(5.f - 5.f * length(vWorldPos - kInput.vUVNearClipWorldPos) / g_fTerrainFarStart);
	vNormal.xy *= fNormalSmooth;
	
	// ��һ��
	vNormal = normalize(vNormal);
	//vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal�����߿ռ�ת������������(���terrain_atlas_geometry.fx)
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// ת�����۲�����ϵ
	float3 vViewNormal = mul(float4(vNormal, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Far_BaseNormal_Ortho
<
	string Description = "Terrain_AtlasGeometry_Far_BaseNormal_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Far_BaseNormal ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_Ortho
<
	string Description = "Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Near_BaseNormalAndNormalMap ();
	}
}
//---------------------------------------------------------------------------------------