//*************************************************************************************************
// ����:	������ɫ(V2,ruantianlong modifiy)
//---------------------------------------------------------
// ����:		
// ����:		2013-09-11
// ����޸�:	2013-09-12
//*************************************************************************************************
#include "terrain_common_v2.h"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_CUBESAMPLE(0, sdPlanarTableSampler, 	sdPlanarTableTex);				//
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, 			sdGeomBuf, 			false);		// ��Ļ����뷨�߻���
SD_POINT_CLAMP_SAMPLE(2, sdLightSampler, 			sdLightBuf, 		false);		// ��Ļ�ֲ����ջ���
SD_LINEAR_WRAP_SAMPLE(3, sdBaseNormalSampler, 		sdBaseNormalTex, 	false);		// ���λ���������ͼ
SD_POINT_CLAMP_SAMPLE(4, sdTileSampler,				sdTileTex,			false);		// ����TileMap
SD_LINEAR_WRAP_SAMPLE(5, sdBlendSampler,			sdBlendTex,			false);		// ����BlendMap
SD_POINT_CLAMP_SAMPLE(6, sdAtlasTableSampler,		sdAtlasTableTex,	false);		// ������������ͼ��ѯ��
SD_LINEAR_WRAP_SAMPLE(7, sdDiffuseAtlasSampler,		sdDiffuseAtlasTex,	false);		// ������������ͼͼ��

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
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// ��ǰ���ӦԶ�ü����ϵĵ����������
	float3	vUVFarClipViewPos	: TEXCOORD2;	// ��ǰ���ӦԶ�ü����ϵĵ�Ĺ۲�����
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
	
	// ��ǰ���ӦԶ�ü����ϵĵ����������
	float4 vUVFarClipProjPos  = float4(kInput.vPos.xy, 1.f, 1.f);
	float4 vUVFarClipWorldPos = mul(vUVFarClipProjPos, g_mDepthToWorld);
	kOutput.vUVFarClipWorldPos = vUVFarClipWorldPos.xyz;
	
	// ��ǰ���ӦԶ�ü����ϵĵ�Ĺ۲�����
	//	1.��Ҫg_mView��ƽ�Ʊ任,����Ҫ�ָ�float4
	// 	2.�滻��w�����Ǳ����������ۻ�
	kOutput.vUVFarClipViewPos  = mul(float4(vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
float4 PS_Main_Simple(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���,����һ���ǶȵĲ�������ɫ
	//float3 vPlanarWeight;
	//vPlanarWeight.xy 	= vBaseNormalTex.zw;
	//vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);

	//clip(1.5f - dot(sign(vPlanarWeight), 1.f));
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// ����UV����TileMap(�����[0,1]�ָ���[0,255]��ͼ����������)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
	// @} 
	
	
	// BlendMap
	// @{
	// �����µ�UV(���������BlendTex�����ĵ�����Tile���ķ���ƫ��)
	//	1.���㵱ǰFragment���TexTile���ĵ��ƫ��(��λ:TexTile)
	//	2.����ǰFragment���TexTile���ĵ��ƫ��ת��Ϊ��BlendMapPixel����ƫ��(��λ:BlendMapPixel)
	//	3.��UVSet��ȥ��BlendMapPixel����ƫ��(ע��UVSet����û�д��������ƫ��)
	//
	// WZԭʽ:
	//	float2 tileCenterOffset = frac(vUVSet * (2048.0 / 4.0)) - 0.5;
	//	vUVSet -= tileCenterOffset * (1.0 / 2048.0);
	//
	// ����ʵ��:
	//	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5;
	//	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	//
	// �޶���(����������BlendMap��������ƫ��)
	//	float2 vUVSet2 = vUVSet + g_vRecipBlendMapSize * 0.5f;
	//
	float2 tileCenterOffset = frac(vUVSet * g_fTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_fRecipBlendMapSize;
	
	// ����UV����BlendMap
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
	// ��һ��Ȩ��
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// ��ͼ���
	// @{
	// �����µĵ���UV(�򵥵�ʹ�õ���λ��,yȡ��ֵ����Ϊ���ε�Y������ͼ��Y���෴)
	float2 vUVSet3 = float2(vWorldPos.x, -vWorldPos.y);
	//float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255 - 1; 
	//float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));

//	// ���㵱ǰ���ص��۲���ʸ��
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	// ���㵱ǰ����ӦȡLOD(�ȼ�����ǵ�ǰPixel��Ӧ������ռ�Pixel�ĳߴ�,Ȼ���2ȡ�����õ�LOD)
//	//	g_vFarPixelSize				�����ڴ�ֱԶƽ���ϵĶ�Ӧ�ߴ�(���Ƴߴ�,��ʵԶƽ����һ���������һ����)
//	//	g_vFarPixelSize * fDepth	�����ڵ�ǰ�����µĴ�ֱƽ���ϵĶ�Ӧ�ߴ�
//	//	dot(vWorldViewVector, vWorldNormal)	��ǰ�������ͶӰ��λ�ĽǶ�,����ͶӰ����ļн�����ֵ
//	// Ŀǰû�н���LOD�㼶֮��Ĺ��ˣ�����LOD֮����ɺܲ���Ȼ
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// ����ͼ��UV
//	//	.xyz �ֱ���3��Layer��id����õ�������U���� 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 �����ص�lod��Ϣ����õ�������V����	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// ��ͼ���
//	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);										///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1[3];		///< ������ͼ��UV����
	float3 vMipmapLevel;	///< ������ͼ��Mipmap�ȼ�
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1[0] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1[1] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1[2] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2[0].xy = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].xy = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].xy = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);
	
	float3 vScaleTmp2 = vScaleTmp * 0.5f;
	avUVSet2[0].zw = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].zw = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].zw = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	// @}

	
	// ����������ɫ
	// @{
	// ����LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// ����۲췽���뷨��
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// �ϳɹ���
	float3 vDiffuseLight, vSpecularLight;
//	AccumLightingOnlyDiffuse(vViewNormal, g_fTerrainUseLightMap > 0.f ? vBlendTex.z : 1.f, vLightTex, vDiffuseLight);
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, g_fTerrainUseLightMap > 0.f ? vBlendTex.z : 1.f, vLightTex, vDiffuseLight, vSpecularLight);

	// �ϳ�������ɫ
//	float3 vColor = vDiffuseLight* vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial;
	float3 vColor = vDiffuseLight * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpecularLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
	// @}
	
#ifdef _SD_EDITOR
	vColor = lerp(vColor, float3(1.f, 0.f, 0.f), saturate(1.f - fTotalWeight) * g_fTerrainShowInvisibleLayers);
	vColor = lerp(vColor, float3(0.f, 1.f, 1.f), any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
#endif

	return float4(vColor, 0.f);
};

//---------------------------------------------------------------------------------------
// ƽ̹������ɫ
float4 PS_Main_Planar(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���,����һ���ǶȵĲ�������ɫ
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);

	clip(1.5f - dot(sign(vPlanarWeight), 1.f));
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// ����UV����TileMap(�����[0,1]�ָ���[0,255]��ͼ����������)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
	// @} 
	
	
	// BlendMap
	// @{
	// �����µ�UV(���������BlendTex�����ĵ�����Tile���ķ���ƫ��)
	//	1.���㵱ǰFragment���TexTile���ĵ��ƫ��(��λ:TexTile)
	//	2.����ǰFragment���TexTile���ĵ��ƫ��ת��Ϊ��BlendMapPixel����ƫ��(��λ:BlendMapPixel)
	//	3.��UVSet��ȥ��BlendMapPixel����ƫ��(ע��UVSet����û�д��������ƫ��)
	//
	// WZԭʽ:
	//	float2 tileCenterOffset = frac(vUVSet * (2048.0 / 4.0)) - 0.5;
	//	vUVSet -= tileCenterOffset * (1.0 / 2048.0);
	//
	// ����ʵ��:
	//	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5;
	//	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	//
	// �޶���(����������BlendMap��������ƫ��)
	//	float2 vUVSet2 = vUVSet + g_vRecipBlendMapSize * 0.5f;
	//
	float2 tileCenterOffset = frac(vUVSet * g_fTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_fRecipBlendMapSize;
	
	// ����UV����BlendMap
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
	// ��һ��Ȩ��
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// ��ͼ���
	// @{
	// ��������������
	//	1.GB����ϵ��X����Y��ǰZ����
	//	2.D3D����ϵ��X����Y����Z��ǰ
	//float2 vUVSet3 = float2(vWorldPos.x, -vWorldPos.y);
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f; 
	
	// �����µĵ���UV(??)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));

//	// ���㵱ǰ���ص��۲���ʸ��
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	//
//	// ���㵱ǰ����ӦȡLOD(�ȼ�����ǵ�ǰPixel��Ӧ������ռ�Pixel�ĳߴ�,Ȼ���2ȡ�����õ�LOD)
//	//	g_vFarPixelSize				�����ڴ�ֱԶƽ���ϵĶ�Ӧ�ߴ�(���Ƴߴ�,��ʵԶƽ����һ���������һ����)
//	//	g_vFarPixelSize * fDepth	�����ڵ�ǰ�����µĴ�ֱƽ���ϵĶ�Ӧ�ߴ�
//	//	dot(vWorldViewVector, vWorldNormal)	��ǰ�������ͶӰ��λ�ĽǶ�,����ͶӰ����ļн�����ֵ
//	// Ŀǰû�н���LOD�㼶֮��Ĺ��ˣ�����LOD֮����ɺܲ���Ȼ
//	//
//	// ��һ�ּ���mipmap�ķ�ʽ��
//	//	float2 vTexSubTileSize = float2(512.f, 512.f);		///< ��Ҫ����mipmap������ĳߴ�
//	//	float2 vTexUVSet = vUVSet3;							///< ���������Ĳ�����������
//	//	float2 dx = ddx(vTexUVSet * vTexSubTileSize);		///< ���������������	
//	//	float2 dy = ddy(vTexUVSet * vTexSubTileSize);
//	//	float d = max(dot(dx, dx), dot(dy, dy));   			///< 
//	//	float mipLevel = 0.5f * log2(d);					///< ȡ����
//	//	mipLevel = floor(mipLevel);   						///< ����ȡ��[0, N]
//	//	float2 vLodLevel = float2(-mipLevel, -mipLevel);
//	//
//	// �ο���
//	//		http://www.ogre3d.org/forums/viewtopic.php?f=5&t=67846
//	//		GPU Pro2, Large-Scale Terrain Rendering for Outdoor Games
//	//
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// ����ͼ��UV
//	//	.xyz �ֱ���3��Layer��id����õ�������U���� 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 �����ص�lod��Ϣ����õ�������V����	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		// WZԭ�治��Ҫround�����Ϳ��ԣ���ʱ��֪��ԭ��
//		vBlendTex.b *= (round(vIndices.b) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (round(vIndices.g) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (round(vIndices.r) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// ��ͼ���
//	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;
	
	float2 vTexSize = float2(512.f, 512.f);										///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1[3];		///< ������ͼ��UV����
	float3 vMipmapLevel;	///< ������ͼ��Mipmap�ȼ�
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1[0] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1[1] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1[2] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2[0].xy = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].xy = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].xy = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);
	
	float3 vScaleTmp2 = vScaleTmp * 0.5f;
	avUVSet2[0].zw = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].zw = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].zw = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	// @}
	
	
	// ����������ɫ
	// @{
	// ����LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// ����۲췽���뷨��
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// �ϳɹ���
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBlendTex.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// �ϳ�������ɫ
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
	// @}
	
#ifdef _SD_EDITOR
	vColor = lerp(vColor, float3(1.f, 0.f, 0.f), saturate(1.f - fTotalWeight) * g_fTerrainShowInvisibleLayers);
	vColor = lerp(vColor, float3(0.f, 1.f, 1.f), any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
#endif

	return float4(vColor, 0.f);
};

//---------------------------------------------------------------------------------------
// ���͵�����ɫ
float4 PS_Main_Seam(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���,С��һ���ǶȵĲ�������ɫ
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);
	
	clip(dot(sign(vPlanarWeight), 1.f) - 1.5f);
	
	// �������ռ䷨��(x,y,z)
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// ����UV����TileMap(�����[0,1]�ָ���[0,255]��ͼ����������)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
	// @} 
	
	
	// BlendMap
	// @{
	// �����µ�UV(���������BlendTex�����ĵ�����Tile���ķ���ƫ��)
	//	1.���㵱ǰFragment���TexTile���ĵ��ƫ��(��λ:TexTile)
	//	2.����ǰFragment���TexTile���ĵ��ƫ��ת��Ϊ��BlendMapPixel����ƫ��(��λ:BlendMapPixel)
	//	3.��UVSet��ȥ��BlendMapPixel����ƫ��(ע��UVSet����û�д��������ƫ��)
	//
	// WZԭʽ:
	//	float2 tileCenterOffset = frac(vUVSet * (2048.0 / 4.0)) - 0.5;
	//	vUVSet -= tileCenterOffset * (1.0 / 2048.0);
	//
	// ����ʵ��:
	//	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5;
	//	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	//
	// �޶���(����������BlendMap��������ƫ��)
	//	float2 vUVSet2 = vUVSet + g_vRecipBlendMapSize * 0.5f;
	//
	float2 tileCenterOffset = frac(vUVSet * g_fTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_fRecipBlendMapSize;
	
	// ����UV����BlendMap
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
	// ��һ��Ȩ��
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// ��ͼ���
	// @{
	// �����µĵ���UV
	//	1.XYƽ������,Yȡ������Ϊ��ͼ��Y���Ƿ���
	//	2.YZƽ������,Zȡ������Ϊ��ͼ��Y���Ƿ��ģ�ȡ����X���������
	//	3.XZƽ������,Zȡ������Ϊ��ͼ��Y���Ƿ��ģ�Xȡ������ΪXY��ʱ����������ͬ
	//	4.������Ϊɶ��ȥ0.5
	//float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	//float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	float2 vTerrainUV_XY = float2(vWorldPos.x, -vWorldPos.y);
	float2 vTerrainUV_YZ = float2(vWorldPos.y * sign(vWorldNormal.x - 0.5f), -vWorldPos.z);
	float2 vTerrainUV_XZ = float2(-vWorldPos.x * sign(vWorldNormal.y - 0.5f), -vWorldPos.z);
	
//	// ���㵱ǰ���ص��۲��ʸ��
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	// ���㵱ǰ����ӦȡLOD(�ȼ�����ǵ�ǰPixel��Ӧ������ռ�Pixel�ĳߴ�,Ȼ���2ȡ�����õ�LOD)
//	//	g_vFarPixelSize				�����ڴ�ֱԶƽ���ϵĶ�Ӧ�ߴ�(���Ƴߴ�,��ʵԶƽ����һ���������һ����)
//	//	g_vFarPixelSize * fDepth	�����ڵ�ǰ�����µĴ�ֱƽ���ϵĶ�Ӧ�ߴ�
//	//	dot(vWorldViewVector, vWorldNormal)	��ǰ�������ͶӰ��λ�ĽǶ�,����ͶӰ����ļн�����ֵ
//	// Ŀǰû�н���LOD�㼶֮��Ĺ��ˣ�����LOD֮����ɺܲ���Ȼ
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// ����ͼ��UV
//	//	.xyz �ֱ���3��Layer��id����õ�������U���� 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 �����ص�lod��Ϣ����õ�������V����	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// ��ͼ���
//	float4 vDiffuseGloss = SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.b +
//		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.g +
//		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);											///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	float2 vXDeriv = ddx(vTerrainUV_XY * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	float2 vYDeriv = ddy(vTerrainUV_XY * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1_XY[3];		///< ������ͼ��UV����
	float2 avUVSet1_YZ[3];		///< ������ͼ��UV����
	float2 avUVSet1_XZ[3];		///< ������ͼ��UV����
	float3 vMipmapLevel;		///< ������ͼ��Mipmap�ȼ�
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1_XY[0] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[0] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[0] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1_XY[1] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[1] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[1] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1_XY[2] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[2] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[2] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< ÿ��DiffuseMap��ԭʼ�ߴ���512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float2 avUVSet2_XY[4];
	float2 avUVSet2_YZ[4];
	float2 avUVSet2_XZ[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2_XY[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_XY[0], iTexId[0]);
	avUVSet2_XY[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_XY[1], iTexId[1]);
	avUVSet2_XY[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_XY[2], iTexId[2]);
	
	avUVSet2_YZ[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_YZ[0], iTexId[0]);
	avUVSet2_YZ[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_YZ[1], iTexId[1]);
	avUVSet2_YZ[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_YZ[2], iTexId[2]);
	
	avUVSet2_XZ[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_XZ[0], iTexId[0]);
	avUVSet2_XZ[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_XZ[1], iTexId[1]);
	avUVSet2_XZ[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_XZ[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.y) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.y) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.y) * vBlendTex.r;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask,g_fGlossMapMask));
#endif
	// @}
	
	
	// ����������ɫ
	// @{
	// ����LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// ����۲췽���뷨��
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// �ϳɹ���
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBlendTex.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// �ϳ�������ɫ
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
	// @}
	
	
#ifdef _SD_EDITOR
	vColor = lerp(vColor, float3(1.f, 0.f, 0.f), saturate(1.f - fTotalWeight) * g_fTerrainShowInvisibleLayers);
	vColor = lerp(vColor, float3(0.f, 1.f, 1.f), any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
#endif
	
	return float4(vColor, 0.f);
}
//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Simple_V2
<
	string Description = "Terrain_AtlasShading_Simple_V2";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Simple ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_V2
<
	string Description = "Terrain_AtlasShading_Planar_V2";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Seam_V2
<
	string Description = "Terrain_AtlasShading_Seam_V2";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Seam ();
	}
}
//---------------------------------------------------------------------------------------