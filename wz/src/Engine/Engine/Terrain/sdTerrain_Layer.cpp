#include "sdEnginePCH.h"
#include "sdTerrain.h"

using namespace Base::Math;
using namespace Engine;
using namespace Engine::RenderSystem;

uint sdTerrain::ms_uiMaxLayerCount = 48;
//-------------------------------------------------------------------------------------------------
uint sdTerrain::GetLayerIndex(const char* szName) const
{
	NIASSERT(szName);

	LayerMapVecConItr itr_layer_begin = m_kLayerMapVec.begin();
	LayerMapVecConItr itr_layer_end = m_kLayerMapVec.end();
	LayerMapVecConItr itr_layer = m_kLayerMapVec.begin();
	for (; itr_layer != itr_layer_end; ++itr_layer)
	{
		if (!strncmp((*itr_layer)->GetName(), szName, MAX_LAYER_NAME_LEN))
			return itr_layer - itr_layer_begin;
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------
sdLayerMap* sdTerrain::GetLayerMap(uint uiIndex) const
{
	if (uiIndex >= m_kLayerMapVec.size())
		return NULL;
	return m_kLayerMapVec[uiIndex];
}
//-------------------------------------------------------------------------------------------------
sdLayerMap* sdTerrain::GetLayerMap(const char* szName) const
{
	NIASSERT(szName);

	LayerMapVecConItr itr_layer = m_kLayerMapVec.begin();
	LayerMapVecConItr itr_layer_end = m_kLayerMapVec.end();
	for (; itr_layer != itr_layer_end; ++itr_layer)
	{
		if (!strncmp((*itr_layer)->GetName(), szName, MAX_LAYER_NAME_LEN))
			return *itr_layer;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::AppendLayerMap(sdLayerMap* pkLayerMap)
{
	NIASSERT(pkLayerMap);

	// ���ͼ������
	if (m_kLayerMapVec.size() >= ms_uiMaxLayerCount)
		return false;

	// ����Ƿ�������ͼ������
	if (GetLayerMap(pkLayerMap->GetName()))
		return false;

	// ��ӵ�ͼ���б����һ��
	m_kLayerMapVec.push_back(pkLayerMap);

	// �༭״̬��
	if (m_bEnableEditMaterial)
	{
		// �����������
		uchar* pucLayerAlpha = NiAlloc(uchar, m_uiBlendTexSize * m_uiBlendTexSize);
		NIASSERT(pucLayerAlpha);
		::ZeroMemory(pucLayerAlpha, m_uiBlendTexSize * m_uiBlendTexSize);

		// ��ӵ���������б�
		m_kLayerAlphaVec.push_back(pucLayerAlpha);

		// ���ͼ�������ͼ������
		m_pkDiffuseAtlas->AddTexture(pkLayerMap->GetDiffuseMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
		m_pkNormalAtlas->AddTexture(pkLayerMap->GetNormalMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::InsertLayerMap(sdLayerMap* pkLayerMap, uint uiIndex)
{
	NIASSERT(pkLayerMap);

	// ���ͼ������
	if (m_kLayerMapVec.size() >= ms_uiMaxLayerCount)
		return false;

	// ��������Ƿ���
	if (uiIndex >= m_kLayerMapVec.size())
		return false;

	// ����Ƿ�������ͼ������
	if (GetLayerMap(pkLayerMap->GetName()))
		return false;

	// ��ӵ�ͼ���б�
	m_kLayerMapVec.insert(m_kLayerMapVec.begin() + uiIndex, pkLayerMap);

	// �����������
	uchar* puiLayerAlpha = new uchar[m_uiBlendTexSize * m_uiBlendTexSize];
	NIASSERT(puiLayerAlpha);
	::ZeroMemory(puiLayerAlpha, m_uiBlendTexSize * m_uiBlendTexSize);

	// ��ӵ���������б�
	m_kLayerAlphaVec.insert(m_kLayerAlphaVec.begin() + uiIndex, puiLayerAlpha);

	// ���ͼ�������ͼ������
	m_pkDiffuseAtlas->InsertTexture(uiIndex, pkLayerMap->GetDiffuseMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
	m_pkNormalAtlas->InsertTexture(uiIndex, pkLayerMap->GetNormalMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::RemoveLayerMap()
{
	// ���ͼ������
	if (m_kLayerMapVec.empty())
		return false;

	// ��ͼ�������Ƴ�
	m_pkDiffuseAtlas->RemoveTexture();
	m_pkNormalAtlas->RemoveTexture();

	// ���ٻ������
	uchar* pucLayerAlpha = m_kLayerAlphaVec.back();
	NiFree(pucLayerAlpha);

	// �ӻ�������б��Ƴ�
	m_kLayerAlphaVec.pop_back();

	// ��ͼ���б��Ƴ�(��������)
	m_kLayerMapVec.pop_back();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::RemoveLayerMap(uint uiIndex)
{
	// ���ͼ������
	if (m_kLayerMapVec.empty())
		return false;

	// ��������Ƿ���
	if (uiIndex >= m_kLayerMapVec.size())
		return false;

	// ��ͼ�������Ƴ�
	m_pkDiffuseAtlas->RemoveTexture(uiIndex);
	m_pkNormalAtlas->RemoveTexture(uiIndex);

	// ���ٻ������
	uchar* pucLayerAlpha = m_kLayerAlphaVec[uiIndex];
	NiFree(pucLayerAlpha);

	// �ӻ�������б��Ƴ�(��������)
	m_kLayerAlphaVec.erase(m_kLayerAlphaVec.begin() + uiIndex);

	// ��ͼ���б��Ƴ�
	m_kLayerMapVec.erase(m_kLayerMapVec.begin() + uiIndex);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::SwapLayerMap(uint uiLIndex, uint uiRIndex)
{
	if (uiLIndex == uiRIndex)
		return true;

	// ���ͼ������
	if (m_kLayerMapVec.empty())
		return false;

	// ��������Ƿ���
	if (uiLIndex >= m_kLayerMapVec.size() || uiRIndex >= m_kLayerMapVec.size())
		return false;

	// ��ͼ�������ڽ���
	m_pkDiffuseAtlas->SwapTexture(uiLIndex, uiRIndex);
	m_pkNormalAtlas->SwapTexture(uiLIndex, uiRIndex);

	// �ڻ�������б��ڽ���
	uchar* pucLayerAlpha = m_kLayerAlphaVec[uiLIndex];
	m_kLayerAlphaVec[uiLIndex] = m_kLayerAlphaVec[uiRIndex];
	m_kLayerAlphaVec[uiRIndex] = pucLayerAlpha;

	// ��ͼ���б��ڽ���
	sdLayerMapPtr pkLayerAlpha = m_kLayerMapVec[uiLIndex];
	m_kLayerMapVec[uiLIndex] = m_kLayerMapVec[uiRIndex];
	m_kLayerMapVec[uiRIndex] = pkLayerAlpha;

	return true;
}
//-------------------------------------------------------------------------------------------------
uchar sdTerrain::GetBlendMapData(uint uiX, uint uiY, uint uiLayer)
{
	// ���ͼ������
	if (uiLayer >= m_kLayerAlphaVec.size())
		return 0;

	// ���ʶ�Ӧͼ��������
	uchar* pucWeightData = m_kLayerAlphaVec[uiLayer];
	NIASSERT(pucWeightData);

	return *(pucWeightData + uiY * m_uiBlendTexSize + uiX); 
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::SetBlendMapData(uint uiX, uint uiY, uint uiLayer, uchar uiWeight)
{
	// ���ͼ������
	if (uiLayer >= m_kLayerAlphaVec.size())
		return;

	// �޸Ķ�Ӧͼ��������
	uchar* pucWeightData = m_kLayerAlphaVec[uiLayer];
	NIASSERT(pucWeightData);

	*(pucWeightData + uiY * m_uiBlendTexSize + uiX) = uiWeight; 
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::UpdateBlendMap(float fCenterX, float fCenterY, float fRadius)
{
	// �Ǳ༭״ֱ̬�ӷ���
	if (!m_bEnableEditMaterial)
		return;

	NIASSERT(m_pkDynamicBlendMap);
	NIASSERT(m_pkDynamicTileMap);

	// ת������ϵ
	// @{
	// ���㷶Χ(��������)
	float fStartX = fCenterX - fRadius;
	float fStartY = fCenterY - fRadius;
	float fEndX	= fCenterX + fRadius;
	float fEndY = fCenterY + fRadius;

	// ���㷶Χ(BlendMap����)
	float fTotalScale = m_fScale * m_fMeterPerPixel;
	float fTotalScaleDiv = 1.f / fTotalScale;
	float fBStartX	= (fStartX - m_kOrigin.x) * fTotalScaleDiv;
	float fBStartY	= (fStartY - m_kOrigin.y) * fTotalScaleDiv;
	float fBEndX	= (fEndX - m_kOrigin.x) * fTotalScaleDiv;
	float fBEndY	= (fEndY - m_kOrigin.y) * fTotalScaleDiv;

	// ǯλ����Χ������ֵ(BlendMap����)
	if (fBStartX > (float)m_uiBlendTexSize)	return;
	if (fBStartY > (float)m_uiBlendTexSize)	return;
	if (fBEndX < 0.0f)	return;
	if (fBEndY < 0.0f)	return;

	if (fBStartX < 0)	fBStartX = 0.0f;
	if (fBStartY < 0)	fBStartY = 0.0f;
	if (fBEndX > (float)m_uiBlendTexSize)	fBEndX = (float)m_uiBlendTexSize;
	if (fBEndY > (float)m_uiBlendTexSize)	fBEndY = (float)m_uiBlendTexSize;
 
	uint uiBStartX	= (uint)sdMath::Floor(fBStartX);		///< ����ȡ����������·�Χ
	uint uiBStartY	= (uint)sdMath::Floor(fBStartY);
	uint uiBEndX	= (uint)sdMath::Ceil(fBEndX);			///< ����ȡ����������·�Χ
	uint uiBEndY	= (uint)sdMath::Ceil(fBEndY);

	// ���㷶Χ(TileMap����)
	uint uiStartTileX	= (uint)sdMath::Floor(uiBStartX / (float)m_uiTexTileSize);							///< ����ȡ����������·�Χ
	uint uiStartTileY	= (uint)sdMath::Floor(uiBStartY / (float)m_uiTexTileSize);
	uint uiEndTileX		= (uint)sdMath::Floor((uiBEndX + m_uiTexTileSize - 1) / (float)m_uiTexTileSize);	///< ����ȡ����������·�Χ
	uint uiEndTileY		= (uint)sdMath::Floor((uiBEndY + m_uiTexTileSize - 1) / (float)m_uiTexTileSize);
	// @}


	// ���»����ͼ
	// @{
	// ����
	//	1.��Ҫע�⵽,DynamicBlendMap����������Ӧ����DynamicTileMap��Ӧ������
	sdDynamicTexture::sdLockedRect kTileRect = m_pkDynamicTileMap->LockRegion(uiStartTileX, uiStartTileY, uiEndTileX - uiStartTileX, uiEndTileY - uiStartTileY);
	NIASSERT(kTileRect.pBits);

	sdDynamicTexture::sdLockedRect kBlendRect = m_pkDynamicBlendMap->LockRegion(uiStartTileX * m_uiTexTileSize, uiStartTileY * m_uiTexTileSize, (uiEndTileX - uiStartTileX) * m_uiTexTileSize, (uiEndTileY - uiStartTileY) * m_uiTexTileSize);
	NIASSERT(kBlendRect.pBits);

	// �������
	//	1.ò��ʹ��iPitchҲľ�����⣬Ӧ����D3Dֻ�����ָ���������Դ�
	//	2.TileMap���ش�С��4byte,BlendMap���ش�С��2byte
	uchar* pucTileData = (uchar*)kTileRect.pBits;
	for (uint i = uiStartTileY; i < uiEndTileY; ++i)
	{
		::ZeroMemory(pucTileData, (uiEndTileX - uiStartTileX) * 4);	///< WZԭ��ΪkTileRect.uiPitch
		pucTileData += kTileRect.uiPitch;
	}

	uchar* pucBlendData = (uchar*)kBlendRect.pBits;
	for (uint i = uiStartTileY; i < uiEndTileY; ++i)
	{
		::ZeroMemory(pucBlendData, (uiBEndX - uiBStartX) * 2);		///< WZԭ��ΪkBlendRect.uiPitch
		pucBlendData += kBlendRect.uiPitch;
	}

	// û�п���ͼ�㣬�����������֮��ֱ�ӷ���
	if (m_kLayerMapVec.empty())
	{
		m_pkDynamicTileMap->UnlockRegion();
		m_pkDynamicBlendMap->UnlockRegion();
		return;
	}

	// ������Ӱ���Tile����,����TileMap��BlendMap
	uint uiNumLayers = m_kLayerMapVec.size();
	uint uiTexTileSizeEx = m_uiTexTileSize + 2;		///< ��������չ1������

	std::vector<int> kVisLayers;
	kVisLayers.reserve(uiNumLayers);

	std::vector<uchar>	kWeights(uiTexTileSizeEx * uiTexTileSizeEx * uiNumLayers, 0);		///< ����TexTile��չ֮������ͼ���Ȩ����Ϣ
	std::vector<uint>	kTotalWeights(uiNumLayers, 0);										///< ����TexTile�ڸ���ͼ���Ȩ����Ϣ�ۻ�

	for (uint uiTileY = uiStartTileY, uiY = 0; uiTileY < uiEndTileY; ++uiTileY, ++uiY)
	{
		for (uint uiTileX = uiStartTileX, uiX = 0; uiTileX < uiEndTileX; ++ uiTileX, ++uiX)
		{
			//
			::ZeroMemory(&kWeights[0], sizeof(uchar) * kWeights.size());
			::ZeroMemory(&kTotalWeights[0], sizeof(uint) * kTotalWeights.size());
			kVisLayers.clear();

			//
			// ���㵱ǰTexTile��Ӧ��BlendMap�ĸ���Pixel�ĸ�Layer��Ȩ����Ϣ
			// 
			// @{
			// ���㵱ǰTexTile��Ӧ��BlendMapλ��
			uint uiStartBlendX = uiTileX * m_uiTexTileSize;
			uint uiStartBlendY = uiTileY * m_uiTexTileSize;

			// ���㵱ǰTexTile�ڲ������ʼλ��(�ڷǱ�Ե����,Tile��Ӧ��BlendMap������չһ�����ص�λ)
			int iStartX = (0 == uiTileX) ? 0 : -1;
			int iStartY = (0 == uiTileY) ? 0 : -1;
			int iEndX = (m_uiTileMapSize - 1 == uiTileX) ? m_uiTexTileSize : m_uiTexTileSize + 1;
			int iEndY = (m_uiTileMapSize - 1 == uiTileY) ? m_uiTexTileSize : m_uiTexTileSize + 1;

			// ���㵱ǰTexTile������չ֮����������������ͼ���Ȩ����Ϣ
			uchar* pucPixelWeightsLine = &kWeights[0];
			for (int iY = iStartY; iY < iEndY; ++iY)
			{
				uchar* pucPixelWeights = pucPixelWeightsLine;
				for (int iX = iStartX; iX < iEndX; ++iX)
				{
					// ��ȡ��ǰBlendMapTex���ص�����ͼ���Ȩ����Ϣ
					uint uiNumVisLayers = GetWeights((uint)(uiStartBlendX + iX), (uint)(uiStartBlendY + iY), pucPixelWeights);
				
					// �ۻ�TexTile������BlendMapTex���صĸ���ͼ�����Ȩ����Ϣ(��������չ����)
					if (iX != -1 && iX != m_uiTexTileSize  && iY != -1 && iY != m_uiTexTileSize)
					{
						for (uint uiLayer = uiNumLayers - uiNumVisLayers; uiLayer < uiNumLayers; ++uiLayer)
							kTotalWeights[uiLayer] += pucPixelWeights[uiLayer];
					}

					pucPixelWeights += uiNumLayers;
				}

				pucPixelWeightsLine += uiTexTileSizeEx * uiNumLayers;
			}
			// @}

			// ɨ�赱ǰTile���ۻ�Ȩ������,���ۼ�Ȩ�شӴ�С��˳����в�������,������ɼ�ͼ���б�(ǰ�����ɼ�)
			uint uiNumVisLayers = 0;
			for (uint uiLayer = 0; uiLayer < uiNumLayers; ++uiLayer)
			{
				if (0 == kTotalWeights[uiLayer])
					continue;

				++uiNumVisLayers;
				
				uint uiNum = kVisLayers.size();
				for (uint j = 0; j < 3; ++j)
				{
					if (j >= uiNum || kTotalWeights[uiLayer] > kTotalWeights[kVisLayers[j]])
					{
						kVisLayers.insert(kVisLayers.begin() + j, uiLayer);

						if (kVisLayers.size() > 3)
							kVisLayers.resize(3);

						break;
					}
				}
			}

			// ��䵱ǰTile���������ݵ�TileMap
			// (4��ÿPixel��4Byte��A8R8G8B8)
			uchar* pucDstTileData = (uchar*)kTileRect.pBits + uiY * kTileRect.uiPitch + uiX * 4;
			for (uint uiLayer = 0; uiLayer < kVisLayers.size(); ++uiLayer)
			{
				pucDstTileData[uiLayer] = kVisLayers[uiLayer];
			}

			// ��䵱ǰTile��Ȩ�����ݵ�BlendMap�Ķ�Ӧ�����ÿ������
			// (2��ÿPixel��2Byte��A4R4G4B4)
			uchar* pucDstPixelWeightsLine = (uchar*)kBlendRect.pBits + (uiY * m_uiTexTileSize) * kBlendRect.uiPitch + (uiX * m_uiTexTileSize) * 2;
			for (uint uiBlendY = 0; uiBlendY < m_uiTexTileSize; ++uiBlendY)
			{	
				uchar* pucDstPixelWeightsData = pucDstPixelWeightsLine;
				for (uint uiBlendX = 0; uiBlendX < m_uiTexTileSize; ++uiBlendX)
				{
					// �����ɼ�ͼ��,ȡ������Ȩ����Ϣ,�ʵ����л�ϲ�����
					uchar ucTempWeights[4] = { 0, 0, 0, 15};	///< ���һ��ΪLightMapͨ��
					stFilterMap& kFilterMap = m_kTileFilterMapVec[uiBlendY * m_uiTexTileSize + uiBlendX];
					for (uint uiLayer = 0; uiLayer < kVisLayers.size(); ++uiLayer)
					{
						float fWeight = 0.0f;
						uint uiVisLayer = kVisLayers[uiLayer];

						//
						// ֱ��ʹ��Ȩֵ�����ܻ������⣬���磺
						//	Layer1/2/3������Tileռ���ƣ�������ĳ��BlendPixelȨֵ��Ϊ0�����
						//	������Ҫ�����ʣ��Ȩ�ذ��������������ͨ��
						//fWeight = kWeights[((uiBlendY + 1) * uiTexTileSizeEx + (uiBlendX + 1)) * uiNumLayers + uiVisLayer];
						// 
						// WZԭʼ������
						//	����Ԥ����Ĳ������Ȩֵ�����м�Ȩȡ��
						//	������Ȼ����Ȩ��Ϊ0�����
						fWeight += kWeights[kFilterMap.uiIdx[0] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[0];
						fWeight += kWeights[kFilterMap.uiIdx[1] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[1];
						fWeight += kWeights[kFilterMap.uiIdx[2] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[2];
						fWeight += kWeights[kFilterMap.uiIdx[3] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[3];

						// ǯλ��[0.0, 255.0]
						fWeight = NiClamp(fWeight, 0.0f, 255.0f);

						// ���ŵ�[0, 15]
						ucTempWeights[uiLayer] = (uchar)(fWeight / 255.0f * 15.0f);
					}

					// ���ʣ��Ȩ���·���
					//	����һ�������Ȩֵ����ͨ��
					//	��������ƽ�����������ͨ��
					//	������: ���������������ͨ��
					//if (ucTotalWeight < 15)
					//{
					//	uchar ucTotalWeight = ucTempWeights[0] + ucTempWeights[1] + ucTempWeights[2];
					//	uchar ucLastWeight = 15 - ucTotalWeight;
					//}

					//***********************************************
					// ����
					//***********************************************
					//if (kVisLayers.size() == 1)
					//{
					//	ucTempWeights[0] = 15;
					//}
					//else if (kVisLayers.size() == 2)
					//{
					//	//ucTempWeights[0] = 8;
					//	//ucTempWeights[1] = 7;
					//	ucTempWeights[0] = 15;
					//	ucTempWeights[1] = 0;
					//}
					//else if (kVisLayers.size() == 3)
					//{
					//	//ucTempWeights[0] = 5;
					//	//ucTempWeights[1] = 5;
					//	//ucTempWeights[2] = 5;
					//	ucTempWeights[0] = 15;
					//	ucTempWeights[1] = 0;
					//	ucTempWeights[2] = 0;
					//}
					//***********************************************

					//***********************************************
					// ����
					//***********************************************
					//if (ucTempWeights[0] + ucTempWeights[1] + ucTempWeights[2] == 0)
					//{
					//	int cc = 0;
					//}
					//***********************************************

					// ѹ�����ݲ���䵽��Ŀ��
					pucDstPixelWeightsData[0] = (ucTempWeights[1] << 4) | ucTempWeights[0];
					pucDstPixelWeightsData[1] = (ucTempWeights[3] << 4) | ucTempWeights[2];

					pucDstPixelWeightsData += 2;
				}

				pucDstPixelWeightsLine += kBlendRect.uiPitch;
			}
		}
	}

	// ������Ӱ���Tile����,����TileMap��BlendMap��LightMap��Ϣ
	std::vector<uchar>	kLightMapData(uiTexTileSizeEx * uiTexTileSizeEx, 0);	///< ����TexTile��չ֮��Ĺ���ͼ��Ϣ 
	for (uint uiTileY = uiStartTileY, uiY = 0; uiTileY < uiEndTileY; ++uiTileY, ++uiY)
	{
		for (uint uiTileX = uiStartTileX, uiX = 0; uiTileX < uiEndTileX; ++ uiTileX, ++uiX)
		{
			//
			::ZeroMemory(&kLightMapData[0], sizeof(uchar) * kLightMapData.size());

			// ���㵱ǰTexTile��Ӧ��BlendMapλ��
			uint uiStartBlendX = uiTileX * m_uiTexTileSize;
			uint uiStartBlendY = uiTileY * m_uiTexTileSize;

			// ���㵱ǰTexTile�ڲ������ʼλ��(�ڷǱ�Ե����,Tile��Ӧ��BlendMap������չһ�����ص�λ)
			int iStartX = (0 == uiTileX) ? 0 : -1;
			int iStartY = (0 == uiTileY) ? 0 : -1;
			int iEndX = (m_uiTileMapSize - 1 == uiTileX) ? m_uiTexTileSize : m_uiTexTileSize + 1;
			int iEndY = (m_uiTileMapSize - 1 == uiTileY) ? m_uiTexTileSize : m_uiTexTileSize + 1;

			// ���㵱ǰTexTile������չ֮������������LightMap��Ϣ
			//	1. ��Ҫע����ǣ�Ҫ����iStartX��iStartYΪ��ʱ�������ʼƫ��
			uchar* pucPixelLightMapDataLine = &kLightMapData[0] + uiTexTileSizeEx * (iStartY + 1) + (iStartX + 1);
			for (int iY = iStartY; iY < iEndY; ++iY)
			{
				uchar* pucPixelWeights = pucPixelLightMapDataLine;
				for (int iX = iStartX; iX < iEndX; ++iX)
				{
					*pucPixelWeights = 255;
					++pucPixelWeights;
				}
				pucPixelLightMapDataLine += uiTexTileSizeEx;
			}

			// ��䵱ǰTile��LightMap��Ϣ��BlendMap�Ķ�Ӧ�����ÿ������
			// (2��ÿPixel��2Byte��A4R4G4B4) 
			uchar* pucDstPixelWeightsLine = (uchar*)kBlendRect.pBits + (uiY * m_uiTexTileSize) * kBlendRect.uiPitch + (uiX * m_uiTexTileSize) * 2;
			for (uint uiBlendY = 0; uiBlendY < m_uiTexTileSize; ++uiBlendY)
			{	
				uchar* pucDstPixelWeightsData = pucDstPixelWeightsLine;
				for (uint uiBlendX = 0; uiBlendX < m_uiTexTileSize; ++uiBlendX)
				{
					//	����Ԥ����Ĳ������Ȩֵ�����м�Ȩȡ��
					float fLightMapData = 0.0f;
					stFilterMap& kFilterMap = m_kTileFilterMapVec[uiBlendY * m_uiTexTileSize + uiBlendX];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[0]] * kFilterMap.fWeight[0];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[1]] * kFilterMap.fWeight[1];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[2]] * kFilterMap.fWeight[2];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[3]] * kFilterMap.fWeight[3];

					// ǯλ��[0.0, 255.0]
					fLightMapData = NiClamp(fLightMapData, 0.0f, 255.0f);

					// ���ŵ�[0, 15]
					uchar ucLightMapData = (uchar)(fLightMapData / 255.0f * 15.0f);

					// ѹ�����ݲ���䵽��Ŀ��
					pucDstPixelWeightsData[1] = (ucLightMapData << 4) | (pucDstPixelWeightsData[1] & 0xf);

					pucDstPixelWeightsData += 2;
				}

				pucDstPixelWeightsLine += kBlendRect.uiPitch;
			}
		}
	}

	// ����
	m_pkDynamicTileMap->UnlockRegion();
	m_pkDynamicBlendMap->UnlockRegion();
	// @}	
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::RepackMaps()
{
	// ������������AtlasTexture������ͼ����Ϣ
	LayerMapVecItr itr_layer = m_kLayerMapVec.begin();
	LayerMapVecItr itr_layer_end = m_kLayerMapVec.end();
	for (int i = 0; itr_layer != itr_layer_end; ++itr_layer, ++i)
	{
		sdLayerMap* pkLayerMap = *itr_layer;
		NIASSERT(pkLayerMap);

		// 
		m_pkDiffuseAtlas->SetTexture(i, pkLayerMap->GetDiffuseMap());
		m_pkDiffuseAtlas->SetUVRepeat(i, pkLayerMap->GetRepeatSize());
		m_pkDiffuseAtlas->SetMipmapBias(i, pkLayerMap->GetMipmapBias());
		m_pkDiffuseAtlas->SetLodBias(m_kRenderParams.diffuseLodBias);

		m_pkNormalAtlas->SetTexture(i, pkLayerMap->GetNormalMap());
		m_pkNormalAtlas->SetUVRepeat(i, pkLayerMap->GetRepeatSize());
		m_pkNormalAtlas->SetMipmapBias(i, pkLayerMap->GetMipmapBias());
		m_pkNormalAtlas->SetLodBias(m_kRenderParams.normalLodBias);
	}

	// ����AtlasTexture
	if (m_pkDiffuseAtlas)	m_pkDiffuseAtlas->Update();
	if (m_pkNormalAtlas)	m_pkNormalAtlas->Update();

	// ����TerrainMaterial
	// @{
	if (m_pkDiffuseAtlas)
	{
		m_spDiffuseAtlasMap = m_pkDiffuseAtlas->GetGBTexture();
		m_spDiffuseAtlasTable = m_pkDiffuseAtlas->GetGBTextureTable();

		m_kRenderParams.diffuseAtlasMap = m_pkDiffuseAtlas->GetGBTexture();
		m_kRenderParams.diffuseAtlasTableMap = m_pkDiffuseAtlas->GetGBTextureTable();

		sdVector2 kTexIdToU = sdVector2::ZERO;
		sdVector2 kLevelToV = sdVector2::ZERO;
		m_pkDiffuseAtlas->GetAtlasTableParam(kTexIdToU, kLevelToV);
		m_kRenderParams.diffuseAtlasTableParam.m_fX = kTexIdToU.m_fX;
		m_kRenderParams.diffuseAtlasTableParam.m_fY = kTexIdToU.m_fY;
		m_kRenderParams.diffuseAtlasTableParam.m_fZ = kLevelToV.m_fX;
		m_kRenderParams.diffuseAtlasTableParam.m_fW = kLevelToV.m_fY;
	}

	if (m_pkNormalAtlas)
	{
		m_kRenderParams.normalAtlasMap = m_pkNormalAtlas->GetGBTexture();
		m_kRenderParams.normalAtlasTableMap = m_pkNormalAtlas->GetGBTextureTable();

		m_spNormalAtlasMap = m_pkNormalAtlas->GetGBTexture();
		m_spNormalAtlasTable = m_pkNormalAtlas->GetGBTextureTable();

		sdVector2 kTexIdToU = sdVector2::ZERO;
		sdVector2 kLevelToV = sdVector2::ZERO;
		m_pkNormalAtlas->GetAtlasTableParam(kTexIdToU, kLevelToV);
		m_kRenderParams.normalAtlasTableParam.m_fX = kTexIdToU.m_fX;
		m_kRenderParams.normalAtlasTableParam.m_fY = kTexIdToU.m_fY;
		m_kRenderParams.normalAtlasTableParam.m_fZ = kLevelToV.m_fX;
		m_kRenderParams.normalAtlasTableParam.m_fW = kLevelToV.m_fY;
	}
	// @}
}
//-------------------------------------------------------------------------------------------------
uint sdTerrain::GetWeights(uint uiX, uint uiY, uchar* pucWeights)
{
	NIASSERT(pucWeights);

	// �Ӹ�ID����ID��ȡ���ֵ
	float fAlphaScale = 1.0f;	///< ��ǰʣ��Ȩ��
	int iIndex = m_kLayerAlphaVec.size() - 1;
	for (; iIndex > 0 && fAlphaScale > 0.01f; --iIndex)
	{
		uchar* pucAlpha = m_kLayerAlphaVec[iIndex];
		NIASSERT(pucAlpha);

		// ����ͼ��Ȩ��[0,255]��[0, 1.f]
		uchar ucLayerAlpha = *(pucAlpha + uiY * m_uiBlendTexSize + uiX);
		float fLayerAlpha = ucLayerAlpha / 255.0f;

		// ����,��������[0, 1.f]��[0,255]
		pucWeights[iIndex] = (uchar)(fLayerAlpha * fAlphaScale * 255.0f + 0.5f);

		// �޶�AlphaScale
		fAlphaScale *= (1.0f - fLayerAlpha);
	}

	// ���⴦���һ��ͼ��
	if (fAlphaScale > 0.01f)
	{
		// ����ʣ��Ȩ��ֵ����һ��ͼ��
		pucWeights[0] = (uchar)(fAlphaScale * 255.0f + 0.5f);
		return m_kLayerAlphaVec.size();
	}
	else
	{
		return m_kLayerAlphaVec.size() - (iIndex + 1);
	}
}
//-------------------------------------------------------------------------------------------------