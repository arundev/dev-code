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

	// 检查图层数量
	if (m_kLayerMapVec.size() >= ms_uiMaxLayerCount)
		return false;

	// 检查是否与已有图层重名
	if (GetLayerMap(pkLayerMap->GetName()))
		return false;

	// 添加到图层列表最后一层
	m_kLayerMapVec.push_back(pkLayerMap);

	// 编辑状态下
	if (m_bEnableEditMaterial)
	{
		// 创建混合掩码
		uchar* pucLayerAlpha = NiAlloc(uchar, m_uiBlendTexSize * m_uiBlendTexSize);
		NIASSERT(pucLayerAlpha);
		::ZeroMemory(pucLayerAlpha, m_uiBlendTexSize * m_uiBlendTexSize);

		// 添加到混合掩码列表
		m_kLayerAlphaVec.push_back(pucLayerAlpha);

		// 添加图层的纹理到图集工具
		m_pkDiffuseAtlas->AddTexture(pkLayerMap->GetDiffuseMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
		m_pkNormalAtlas->AddTexture(pkLayerMap->GetNormalMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::InsertLayerMap(sdLayerMap* pkLayerMap, uint uiIndex)
{
	NIASSERT(pkLayerMap);

	// 检查图层数量
	if (m_kLayerMapVec.size() >= ms_uiMaxLayerCount)
		return false;

	// 检查索引是否超限
	if (uiIndex >= m_kLayerMapVec.size())
		return false;

	// 检查是否与已有图层重名
	if (GetLayerMap(pkLayerMap->GetName()))
		return false;

	// 添加到图层列表
	m_kLayerMapVec.insert(m_kLayerMapVec.begin() + uiIndex, pkLayerMap);

	// 创建混合掩码
	uchar* puiLayerAlpha = new uchar[m_uiBlendTexSize * m_uiBlendTexSize];
	NIASSERT(puiLayerAlpha);
	::ZeroMemory(puiLayerAlpha, m_uiBlendTexSize * m_uiBlendTexSize);

	// 添加到混合掩码列表
	m_kLayerAlphaVec.insert(m_kLayerAlphaVec.begin() + uiIndex, puiLayerAlpha);

	// 添加图层的纹理到图集工具
	m_pkDiffuseAtlas->InsertTexture(uiIndex, pkLayerMap->GetDiffuseMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());
	m_pkNormalAtlas->InsertTexture(uiIndex, pkLayerMap->GetNormalMap(), pkLayerMap->GetRepeatSize(), pkLayerMap->GetMipmapBias());

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::RemoveLayerMap()
{
	// 检查图层数量
	if (m_kLayerMapVec.empty())
		return false;

	// 从图集工具移除
	m_pkDiffuseAtlas->RemoveTexture();
	m_pkNormalAtlas->RemoveTexture();

	// 销毁混合掩码
	uchar* pucLayerAlpha = m_kLayerAlphaVec.back();
	NiFree(pucLayerAlpha);

	// 从混合掩码列表移除
	m_kLayerAlphaVec.pop_back();

	// 从图层列表移除(对象被清零)
	m_kLayerMapVec.pop_back();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::RemoveLayerMap(uint uiIndex)
{
	// 检查图层数量
	if (m_kLayerMapVec.empty())
		return false;

	// 检查索引是否超限
	if (uiIndex >= m_kLayerMapVec.size())
		return false;

	// 从图集工具移除
	m_pkDiffuseAtlas->RemoveTexture(uiIndex);
	m_pkNormalAtlas->RemoveTexture(uiIndex);

	// 销毁混合掩码
	uchar* pucLayerAlpha = m_kLayerAlphaVec[uiIndex];
	NiFree(pucLayerAlpha);

	// 从混合掩码列表移除(对象被清零)
	m_kLayerAlphaVec.erase(m_kLayerAlphaVec.begin() + uiIndex);

	// 从图层列表移除
	m_kLayerMapVec.erase(m_kLayerMapVec.begin() + uiIndex);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::SwapLayerMap(uint uiLIndex, uint uiRIndex)
{
	if (uiLIndex == uiRIndex)
		return true;

	// 检查图层数量
	if (m_kLayerMapVec.empty())
		return false;

	// 检查索引是否超限
	if (uiLIndex >= m_kLayerMapVec.size() || uiRIndex >= m_kLayerMapVec.size())
		return false;

	// 在图集工具内交换
	m_pkDiffuseAtlas->SwapTexture(uiLIndex, uiRIndex);
	m_pkNormalAtlas->SwapTexture(uiLIndex, uiRIndex);

	// 在混合掩码列表内交换
	uchar* pucLayerAlpha = m_kLayerAlphaVec[uiLIndex];
	m_kLayerAlphaVec[uiLIndex] = m_kLayerAlphaVec[uiRIndex];
	m_kLayerAlphaVec[uiRIndex] = pucLayerAlpha;

	// 在图层列表内交换
	sdLayerMapPtr pkLayerAlpha = m_kLayerMapVec[uiLIndex];
	m_kLayerMapVec[uiLIndex] = m_kLayerMapVec[uiRIndex];
	m_kLayerMapVec[uiRIndex] = pkLayerAlpha;

	return true;
}
//-------------------------------------------------------------------------------------------------
uchar sdTerrain::GetBlendMapData(uint uiX, uint uiY, uint uiLayer)
{
	// 检查图层索引
	if (uiLayer >= m_kLayerAlphaVec.size())
		return 0;

	// 访问对应图层混合数据
	uchar* pucWeightData = m_kLayerAlphaVec[uiLayer];
	NIASSERT(pucWeightData);

	return *(pucWeightData + uiY * m_uiBlendTexSize + uiX); 
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::SetBlendMapData(uint uiX, uint uiY, uint uiLayer, uchar uiWeight)
{
	// 检查图层索引
	if (uiLayer >= m_kLayerAlphaVec.size())
		return;

	// 修改对应图层混合数据
	uchar* pucWeightData = m_kLayerAlphaVec[uiLayer];
	NIASSERT(pucWeightData);

	*(pucWeightData + uiY * m_uiBlendTexSize + uiX) = uiWeight; 
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::UpdateBlendMap(float fCenterX, float fCenterY, float fRadius)
{
	// 非编辑状态直接返回
	if (!m_bEnableEditMaterial)
		return;

	NIASSERT(m_pkDynamicBlendMap);
	NIASSERT(m_pkDynamicTileMap);

	// 转换坐标系
	// @{
	// 计算范围(世界坐标)
	float fStartX = fCenterX - fRadius;
	float fStartY = fCenterY - fRadius;
	float fEndX	= fCenterX + fRadius;
	float fEndY = fCenterY + fRadius;

	// 计算范围(BlendMap坐标)
	float fTotalScale = m_fScale * m_fMeterPerPixel;
	float fTotalScaleDiv = 1.f / fTotalScale;
	float fBStartX	= (fStartX - m_kOrigin.x) * fTotalScaleDiv;
	float fBStartY	= (fStartY - m_kOrigin.y) * fTotalScaleDiv;
	float fBEndX	= (fEndX - m_kOrigin.x) * fTotalScaleDiv;
	float fBEndY	= (fEndY - m_kOrigin.y) * fTotalScaleDiv;

	// 钳位到范围内整数值(BlendMap坐标)
	if (fBStartX > (float)m_uiBlendTexSize)	return;
	if (fBStartY > (float)m_uiBlendTexSize)	return;
	if (fBEndX < 0.0f)	return;
	if (fBEndY < 0.0f)	return;

	if (fBStartX < 0)	fBStartX = 0.0f;
	if (fBStartY < 0)	fBStartY = 0.0f;
	if (fBEndX > (float)m_uiBlendTexSize)	fBEndX = (float)m_uiBlendTexSize;
	if (fBEndY > (float)m_uiBlendTexSize)	fBEndY = (float)m_uiBlendTexSize;
 
	uint uiBStartX	= (uint)sdMath::Floor(fBStartX);		///< 向下取整，扩大更新范围
	uint uiBStartY	= (uint)sdMath::Floor(fBStartY);
	uint uiBEndX	= (uint)sdMath::Ceil(fBEndX);			///< 向上取整，扩大更新范围
	uint uiBEndY	= (uint)sdMath::Ceil(fBEndY);

	// 计算范围(TileMap坐标)
	uint uiStartTileX	= (uint)sdMath::Floor(uiBStartX / (float)m_uiTexTileSize);							///< 向下取整，扩大更新范围
	uint uiStartTileY	= (uint)sdMath::Floor(uiBStartY / (float)m_uiTexTileSize);
	uint uiEndTileX		= (uint)sdMath::Floor((uiBEndX + m_uiTexTileSize - 1) / (float)m_uiTexTileSize);	///< 向上取整，扩大更新范围
	uint uiEndTileY		= (uint)sdMath::Floor((uiBEndY + m_uiTexTileSize - 1) / (float)m_uiTexTileSize);
	// @}


	// 更新混合贴图
	// @{
	// 锁定
	//	1.需要注意到,DynamicBlendMap锁定的区域应该是DynamicTileMap对应的区域
	sdDynamicTexture::sdLockedRect kTileRect = m_pkDynamicTileMap->LockRegion(uiStartTileX, uiStartTileY, uiEndTileX - uiStartTileX, uiEndTileY - uiStartTileY);
	NIASSERT(kTileRect.pBits);

	sdDynamicTexture::sdLockedRect kBlendRect = m_pkDynamicBlendMap->LockRegion(uiStartTileX * m_uiTexTileSize, uiStartTileY * m_uiTexTileSize, (uiEndTileX - uiStartTileX) * m_uiTexTileSize, (uiEndTileY - uiStartTileY) * m_uiTexTileSize);
	NIASSERT(kBlendRect.pBits);

	// 清空区域
	//	1.貌似使用iPitch也木有问题，应该是D3D只会更新指定的区域到显存
	//	2.TileMap像素大小是4byte,BlendMap像素大小是2byte
	uchar* pucTileData = (uchar*)kTileRect.pBits;
	for (uint i = uiStartTileY; i < uiEndTileY; ++i)
	{
		::ZeroMemory(pucTileData, (uiEndTileX - uiStartTileX) * 4);	///< WZ原文为kTileRect.uiPitch
		pucTileData += kTileRect.uiPitch;
	}

	uchar* pucBlendData = (uchar*)kBlendRect.pBits;
	for (uint i = uiStartTileY; i < uiEndTileY; ++i)
	{
		::ZeroMemory(pucBlendData, (uiBEndX - uiBStartX) * 2);		///< WZ原文为kBlendRect.uiPitch
		pucBlendData += kBlendRect.uiPitch;
	}

	// 没有可用图层，则在清空区域之后直接返回
	if (m_kLayerMapVec.empty())
	{
		m_pkDynamicTileMap->UnlockRegion();
		m_pkDynamicBlendMap->UnlockRegion();
		return;
	}

	// 遍历受影响的Tile像素,更新TileMap和BlendMap
	uint uiNumLayers = m_kLayerMapVec.size();
	uint uiTexTileSizeEx = m_uiTexTileSize + 2;		///< 向四周扩展1个像素

	std::vector<int> kVisLayers;
	kVisLayers.reserve(uiNumLayers);

	std::vector<uchar>	kWeights(uiTexTileSizeEx * uiTexTileSizeEx * uiNumLayers, 0);		///< 保存TexTile扩展之后所有图层的权重信息
	std::vector<uint>	kTotalWeights(uiNumLayers, 0);										///< 保存TexTile内各个图层的权重信息累积

	for (uint uiTileY = uiStartTileY, uiY = 0; uiTileY < uiEndTileY; ++uiTileY, ++uiY)
	{
		for (uint uiTileX = uiStartTileX, uiX = 0; uiTileX < uiEndTileX; ++ uiTileX, ++uiX)
		{
			//
			::ZeroMemory(&kWeights[0], sizeof(uchar) * kWeights.size());
			::ZeroMemory(&kTotalWeights[0], sizeof(uint) * kTotalWeights.size());
			kVisLayers.clear();

			//
			// 计算当前TexTile对应的BlendMap的各个Pixel的各Layer的权重信息
			// 
			// @{
			// 计算当前TexTile对应的BlendMap位置
			uint uiStartBlendX = uiTileX * m_uiTexTileSize;
			uint uiStartBlendY = uiTileY * m_uiTexTileSize;

			// 计算当前TexTile内部相对起始位置(在非边缘部分,Tile对应的BlendMap向外扩展一个像素单位)
			int iStartX = (0 == uiTileX) ? 0 : -1;
			int iStartY = (0 == uiTileY) ? 0 : -1;
			int iEndX = (m_uiTileMapSize - 1 == uiTileX) ? m_uiTexTileSize : m_uiTexTileSize + 1;
			int iEndY = (m_uiTileMapSize - 1 == uiTileY) ? m_uiTexTileSize : m_uiTexTileSize + 1;

			// 计算当前TexTile向外扩展之后的整个区域的所有图层的权重信息
			uchar* pucPixelWeightsLine = &kWeights[0];
			for (int iY = iStartY; iY < iEndY; ++iY)
			{
				uchar* pucPixelWeights = pucPixelWeightsLine;
				for (int iX = iStartX; iX < iEndX; ++iX)
				{
					// 提取当前BlendMapTex像素的所有图层的权重信息
					uint uiNumVisLayers = GetWeights((uint)(uiStartBlendX + iX), (uint)(uiStartBlendY + iY), pucPixelWeights);
				
					// 累积TexTile内所有BlendMapTex像素的各个图层的总权重信息(不包括扩展部分)
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

			// 扫描当前Tile的累积权重数组,按累计权重从大到小的顺序进行插入排序,并加入可见图层列表(前三个可见)
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

			// 填充当前Tile的索引数据到TileMap
			// (4是每Pixel有4Byte，A8R8G8B8)
			uchar* pucDstTileData = (uchar*)kTileRect.pBits + uiY * kTileRect.uiPitch + uiX * 4;
			for (uint uiLayer = 0; uiLayer < kVisLayers.size(); ++uiLayer)
			{
				pucDstTileData[uiLayer] = kVisLayers[uiLayer];
			}

			// 填充当前Tile的权重数据到BlendMap的对应区域的每个像素
			// (2是每Pixel有2Byte，A4R4G4B4)
			uchar* pucDstPixelWeightsLine = (uchar*)kBlendRect.pBits + (uiY * m_uiTexTileSize) * kBlendRect.uiPitch + (uiX * m_uiTexTileSize) * 2;
			for (uint uiBlendY = 0; uiBlendY < m_uiTexTileSize; ++uiBlendY)
			{	
				uchar* pucDstPixelWeightsData = pucDstPixelWeightsLine;
				for (uint uiBlendX = 0; uiBlendX < m_uiTexTileSize; ++uiBlendX)
				{
					// 遍历可见图层,取出各层权重信息,适当进行混合并保存
					uchar ucTempWeights[4] = { 0, 0, 0, 15};	///< 最后一个为LightMap通道
					stFilterMap& kFilterMap = m_kTileFilterMapVec[uiBlendY * m_uiTexTileSize + uiBlendX];
					for (uint uiLayer = 0; uiLayer < kVisLayers.size(); ++uiLayer)
					{
						float fWeight = 0.0f;
						uint uiVisLayer = kVisLayers[uiLayer];

						//
						// 直接使用权值，可能会有问题，例如：
						//	Layer1/2/3在整个Tile占优势，但是在某个BlendPixel权值均为0的情况
						//	可能需要在最后将剩余权重按比例分配给各个通道
						//fWeight = kWeights[((uiBlendY + 1) * uiTexTileSizeEx + (uiBlendX + 1)) * uiNumLayers + uiVisLayer];
						// 
						// WZ原始做法：
						//	按照预计算的采样点和权值来进行加权取样
						//	可能依然存在权重为0的情况
						fWeight += kWeights[kFilterMap.uiIdx[0] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[0];
						fWeight += kWeights[kFilterMap.uiIdx[1] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[1];
						fWeight += kWeights[kFilterMap.uiIdx[2] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[2];
						fWeight += kWeights[kFilterMap.uiIdx[3] * uiNumLayers + uiVisLayer] * kFilterMap.fWeight[3];

						// 钳位到[0.0, 255.0]
						fWeight = NiClamp(fWeight, 0.0f, 255.0f);

						// 缩放到[0, 15]
						ucTempWeights[uiLayer] = (uchar)(fWeight / 255.0f * 15.0f);
					}

					// 最后将剩余权重新分配
					//	方案一：分配给权值最大的通道
					//	方案二：平均分配给各个通道
					//	方案三: 按比例分配给各个通道
					//if (ucTotalWeight < 15)
					//{
					//	uchar ucTotalWeight = ucTempWeights[0] + ucTempWeights[1] + ucTempWeights[2];
					//	uchar ucLastWeight = 15 - ucTotalWeight;
					//}

					//***********************************************
					// 调试
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
					// 调试
					//***********************************************
					//if (ucTempWeights[0] + ucTempWeights[1] + ucTempWeights[2] == 0)
					//{
					//	int cc = 0;
					//}
					//***********************************************

					// 压缩数据并填充到到目标
					pucDstPixelWeightsData[0] = (ucTempWeights[1] << 4) | ucTempWeights[0];
					pucDstPixelWeightsData[1] = (ucTempWeights[3] << 4) | ucTempWeights[2];

					pucDstPixelWeightsData += 2;
				}

				pucDstPixelWeightsLine += kBlendRect.uiPitch;
			}
		}
	}

	// 遍历受影响的Tile像素,更新TileMap和BlendMap的LightMap信息
	std::vector<uchar>	kLightMapData(uiTexTileSizeEx * uiTexTileSizeEx, 0);	///< 保存TexTile扩展之后的光照图信息 
	for (uint uiTileY = uiStartTileY, uiY = 0; uiTileY < uiEndTileY; ++uiTileY, ++uiY)
	{
		for (uint uiTileX = uiStartTileX, uiX = 0; uiTileX < uiEndTileX; ++ uiTileX, ++uiX)
		{
			//
			::ZeroMemory(&kLightMapData[0], sizeof(uchar) * kLightMapData.size());

			// 计算当前TexTile对应的BlendMap位置
			uint uiStartBlendX = uiTileX * m_uiTexTileSize;
			uint uiStartBlendY = uiTileY * m_uiTexTileSize;

			// 计算当前TexTile内部相对起始位置(在非边缘部分,Tile对应的BlendMap向外扩展一个像素单位)
			int iStartX = (0 == uiTileX) ? 0 : -1;
			int iStartY = (0 == uiTileY) ? 0 : -1;
			int iEndX = (m_uiTileMapSize - 1 == uiTileX) ? m_uiTexTileSize : m_uiTexTileSize + 1;
			int iEndY = (m_uiTileMapSize - 1 == uiTileY) ? m_uiTexTileSize : m_uiTexTileSize + 1;

			// 计算当前TexTile向外扩展之后的整个区域的LightMap信息
			//	1. 需要注意的是，要纠正iStartX和iStartY为负时引起的起始偏移
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

			// 填充当前Tile的LightMap信息到BlendMap的对应区域的每个像素
			// (2是每Pixel有2Byte，A4R4G4B4) 
			uchar* pucDstPixelWeightsLine = (uchar*)kBlendRect.pBits + (uiY * m_uiTexTileSize) * kBlendRect.uiPitch + (uiX * m_uiTexTileSize) * 2;
			for (uint uiBlendY = 0; uiBlendY < m_uiTexTileSize; ++uiBlendY)
			{	
				uchar* pucDstPixelWeightsData = pucDstPixelWeightsLine;
				for (uint uiBlendX = 0; uiBlendX < m_uiTexTileSize; ++uiBlendX)
				{
					//	按照预计算的采样点和权值来进行加权取样
					float fLightMapData = 0.0f;
					stFilterMap& kFilterMap = m_kTileFilterMapVec[uiBlendY * m_uiTexTileSize + uiBlendX];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[0]] * kFilterMap.fWeight[0];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[1]] * kFilterMap.fWeight[1];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[2]] * kFilterMap.fWeight[2];
					fLightMapData += kLightMapData[kFilterMap.uiIdx[3]] * kFilterMap.fWeight[3];

					// 钳位到[0.0, 255.0]
					fLightMapData = NiClamp(fLightMapData, 0.0f, 255.0f);

					// 缩放到[0, 15]
					uchar ucLightMapData = (uchar)(fLightMapData / 255.0f * 15.0f);

					// 压缩数据并填充到到目标
					pucDstPixelWeightsData[1] = (ucLightMapData << 4) | (pucDstPixelWeightsData[1] & 0xf);

					pucDstPixelWeightsData += 2;
				}

				pucDstPixelWeightsLine += kBlendRect.uiPitch;
			}
		}
	}

	// 解锁
	m_pkDynamicTileMap->UnlockRegion();
	m_pkDynamicBlendMap->UnlockRegion();
	// @}	
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::RepackMaps()
{
	// 重新设置设置AtlasTexture的输入图层信息
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

	// 更新AtlasTexture
	if (m_pkDiffuseAtlas)	m_pkDiffuseAtlas->Update();
	if (m_pkNormalAtlas)	m_pkNormalAtlas->Update();

	// 更新TerrainMaterial
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

	// 从高ID到低ID提取混合值
	float fAlphaScale = 1.0f;	///< 当前剩余权重
	int iIndex = m_kLayerAlphaVec.size() - 1;
	for (; iIndex > 0 && fAlphaScale > 0.01f; --iIndex)
	{
		uchar* pucAlpha = m_kLayerAlphaVec[iIndex];
		NIASSERT(pucAlpha);

		// 计算图层权重[0,255]到[0, 1.f]
		uchar ucLayerAlpha = *(pucAlpha + uiY * m_uiBlendTexSize + uiX);
		float fLayerAlpha = ucLayerAlpha / 255.0f;

		// 缩放,四舍五入[0, 1.f]到[0,255]
		pucWeights[iIndex] = (uchar)(fLayerAlpha * fAlphaScale * 255.0f + 0.5f);

		// 修订AlphaScale
		fAlphaScale *= (1.0f - fLayerAlpha);
	}

	// 特殊处理第一个图层
	if (fAlphaScale > 0.01f)
	{
		// 分配剩余权重值给第一个图层
		pucWeights[0] = (uchar)(fAlphaScale * 255.0f + 0.5f);
		return m_kLayerAlphaVec.size();
	}
	else
	{
		return m_kLayerAlphaVec.size() - (iIndex + 1);
	}
}
//-------------------------------------------------------------------------------------------------