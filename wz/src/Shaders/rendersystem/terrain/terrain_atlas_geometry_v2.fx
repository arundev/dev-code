//*************************************************************************************************
// ����:	����Normal/Depth���Ƶ�G-Buffer(V2,ruantianlong modifiy)
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
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// ��ǰ���ӦԶ�ü����ϵĵ����������
//	float3	vUVFarClipViewPos	: TEXCOORD2;	// ��ǰ���ӦԶ�ü����ϵĵ�Ĺ۲�����
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
	//kOutput.vUVFarClipViewPos  = mul(float4(vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
// ��Ⱦ����,��Ⱦ�������ߺͷ�����ͼ(�°棬��Ҫ�ǸĽ���Mipmap����)
float4 PS_Main_Near_BaseNormalAndNormalMap_New(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
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
	// ��������������
	//	1.GB����ϵ��X����Y��ǰZ����
	//	2.D3D����ϵ��X����Y����Z��ǰ
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	
	// �����µĵ���UV(??)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	
	// ���㵱ǰ������߿ռ�
	float3 vWorldBinormal 	= cross(float3(vPlanarVec.xy, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	// ���߻��(NormalAtlasTable, NormalAtlas)
	// @{
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
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fNormalAtlasLevelScale + g_fNormalAtlasLevelOffset);
//	
//	// ��ͼ���
//	float3 vNormal = SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3).xyz * vBlendTex.b +
//		SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3).xyz * vBlendTex.g +
//		SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3).xyz * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);										///< ÿ��NormalMap��ԭʼ�ߴ���512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< ����2.8f�Ǹ��̶�ƫ�ò���
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
	
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
	
	// ��������ҳ��������ͼ���е�λ��
	int2 iTexRowCol[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;				///< ����ÿ�ߵ�ԭʼ������(ÿ��DiffuseMap��ԭʼ�ߴ���512)
	for (int i = 0; i < 3; ++i)
	{
		iTexRowCol[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);			///< ��������ҳ������
		iTexRowCol[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));	///< ��������ҳ������
	}

	// ������ҳ�Ĳ�������ת��������ͼ���ϵĲ�������
	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);							///< ȡ��mipmap�ĵȼ�(0,1,2,3...)
	float3 vMipmapScale = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);	///< ����mipmap���ԭʼ��������ű���(1,1/2,1/4,1/8...)
	avUVSet2[0].xy = ComputeUV(vMipmapScale[0], avUVSet1[0], iTexRowCol[0]);
	avUVSet2[1].xy = ComputeUV(vMipmapScale[1], avUVSet1[1], iTexRowCol[1]);
	avUVSet2[2].xy = ComputeUV(vMipmapScale[2], avUVSet1[2], iTexRowCol[2]);
	
	float3 vMipmapScale2 = vMipmapScale * 0.5f;						///< ������һ��mipmap���ԭʼ��������ű���
	avUVSet2[0].zw = ComputeUV(vMipmapScale2[0], avUVSet1[0], iTexRowCol[0]);
	avUVSet2[1].zw = ComputeUV(vMipmapScale2[1], avUVSet1[1], iTexRowCol[1]);
	avUVSet2[2].zw = ComputeUV(vMipmapScale2[2], avUVSet1[2], iTexRowCol[2]);

	// ��ͼ���
	float3 vWeight = frac(vMipmapLevel);							///< ȡ��mipmap�ĸ���ֵ������Ϊ����mipmap֮��Ĳ�ֵȨ��
	float3 vNormal = (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdNormalAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
	// �任DetailNormal(��[0,1]��[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// ����DetailNormal
	vNormal.xy *= g_fTerrainNormalScale;
	
	// ��Զ����������������(���ӵ㵽�ָ��߽������Թ���,����Ч���ܲ�,��������û��,����һ���Ż�)
	float fNormalSmooth = saturate(5.f - 5.f * length(vWorldPos - g_vViewPos) / g_fTerrainFarStart);
	vNormal.xy *= fNormalSmooth;
	
	// ��һ��
	vNormal = normalize(vNormal);
	//vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal�����߿ռ�ת������������(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// �任Normal���۲�ռ�
	// 	1.��Ҫ������ת�þ���,
	//	2.ViewMatrix��ת��������������,ƽ�Ʋ��ֲ���,����ֻ��Ҫ��ת�任
	//	3.g_mView��ת����(3x3)������������ת�þ��������Լ�
	float3 vViewNormal = mul(float4(vNormal, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal));
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_V2
<
	string Description = "Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_New";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Near_BaseNormalAndNormalMap_New ();
	}
}
//---------------------------------------------------------------------------------------