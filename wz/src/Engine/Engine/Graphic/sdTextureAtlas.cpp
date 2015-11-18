#include "sdEnginePCH.h"
#include "sdTextureAtlas.h"
#include "sdLuaFile.h"

#include <sdMath.h>

using namespace Base::Math;
using namespace Engine::Util;

#define MAX_MIPMAP_LOD			6

// 图集最大尺寸
#define MAX_ATLAS_TEXTURE_SIZE	4096	

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// LevelInfo的排序算子
bool sdTextureAtlas::LevelGreater(stLevelInfo* lhs, stLevelInfo* rhs)
{
	NIASSERT(lhs && rhs && lhs != rhs);

	// 首先比较尺寸(尺寸大的优先)
	if (lhs->uiSize != rhs->uiSize)
		return lhs->uiSize > rhs->uiSize;

	// 其次比较Mipmap数量(Mipmap多的优先)
	if (lhs->pkTexInfo->uiTotalPixels != rhs->pkTexInfo->uiTotalPixels)
		return lhs->pkTexInfo->uiTotalPixels > rhs->pkTexInfo->uiTotalPixels;

	// 最后比较地址
	return lhs->pkTexInfo->spTexture > rhs->pkTexInfo->spTexture;
}
//-------------------------------------------------------------------------------------------------
// \TODO 实验其他的平面镶嵌方式
void sdTextureAtlas::GetCoordinate(uint uiNumAllAllocatedPixels, uint& uiX, uint& uiY)
{
	// 猜想是一个Z曲线
	static const uchar uiODDBits[256]	= {
		#define	OB1(n, m)	(n + m)
		#define OB2(n, m)	OB1(0, m),			OB1(1, m)
		#define OB3(n, m)	OB2(0, m + (n<<1)), OB2(1, m + (n<<1))
		#define OB4(n, m)	OB3(0, m),			OB3(1, m)
		#define OB5(n, m)	OB4(0, m + (n<<2)), OB4(1, m + (n<<2))
		#define OB6(n, m)	OB5(0, m),			OB5(1, m)
		#define OB7(n)		OB6(0, (n<<3)),		OB6(1, (n<<3))
		#define OB8(n)		OB7(0),				OB7(1)
		
		OB8(0), OB8(1),
	};

	static const uchar uiEvenBits[256]	= {
		#define EB1(n, m)	(m)
		#define EB2(n, m)	EB1(0, n + m),		EB1(1, n+m)
		#define EB3(n, m)	EB2(0, m),			EB2(1, m)
		#define EB4(n, m)	EB3(0, m + (n<<1)),	EB3(1, m + (n<<1))
		#define EB5(n, m)	EB4(0, m),			EB4(1, m)
		#define EB6(n, m)	EB5(0, m + (n<<2)),	EB5(1, m + (n<<2))
		#define EB7(n, m)	EB6(0, m),			EB6(1, m)
		#define EB8(n)		EB7(0, (n<<3)),		EB7(1, (n<<3))
				
		EB8(0), EB8(1),
	};

	uchar* ucPtr =  (uchar*)&uiNumAllAllocatedPixels;
	uiY = uiEvenBits[ucPtr[0]] + (uiEvenBits[ucPtr[1]] << 4) + (uiEvenBits[ucPtr[2]] << 8) + (uiEvenBits[ucPtr[3]] << 12);
	uiX = uiODDBits[ucPtr[0]] + (uiODDBits[ucPtr[1]] << 4) + (uiODDBits[ucPtr[2]] << 8) + (uiODDBits[ucPtr[3]] << 12);
}
//-------------------------------------------------------------------------------------------------
sdTextureAtlas::sdTextureAtlas(D3DFORMAT eFormat)
: m_eFormat(eFormat)
, m_uiAtlasSize(0)
, m_uiTotalPixels(0)
, m_fLodBias(0.f)
{
	// 获取渲染设备
	m_spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(m_spRenderer);

	m_spD3DDevice = m_spRenderer->GetD3DDevice();

	// 创建AtlasTexture和AtlasTable的GB纹理
	m_spAtlasTexture = NiDX9Direct3DTexture::Create(m_spRenderer);
	NIASSERT(m_spAtlasTexture);

	m_spAtlasTable	 = NiDX9Direct3DTexture::Create(m_spRenderer);
	NIASSERT(m_spAtlasTable);

	m_spD3DAtlasTexture = NULL;
	m_spD3DAtlasTable = NULL;

	m_bAtlasDirty = false;
	m_bAtlasTextureDirty = false;
	m_bAtlasTableDirty = false;
	m_bAtlasTableTextureDirty = false;
}
//-------------------------------------------------------------------------------------------------
sdTextureAtlas::~sdTextureAtlas()
{
	while (!m_kTextureInfoVec.empty())
	{
		RemoveTexture();
	}
	m_kTextureInfoVec.clear();

	DestroyAtlasTexture();
	DestroyAtlasTableTexture();
}
//-------------------------------------------------------------------------------------------------
uint sdTextureAtlas::AddTexture(NiTexture *spTexture, float fUVRepeat, float fMipmapBias,  uint uiNumLevel)
{
	return InsertTexture(m_kTextureInfoVec.size(), spTexture, fUVRepeat, fMipmapBias, uiNumLevel);
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::RemoveTexture()
{
	return RemoveTexture(m_kTextureInfoVec.size() - 1);
}
//-------------------------------------------------------------------------------------------------
uint sdTextureAtlas::InsertTexture(uint uiIndex, NiTexture *spTexture, float fUVRepeat, float fMipmapBias,  uint uiNumLevel)
{
	// 检查输入参数
	// @{
	if (uiIndex > m_kTextureInfoVec.size())	return -1;	
	if (NULL == spTexture)	return -1;
	if (fUVRepeat <= 0.0f)	return -1;

	// 检查贴图是否已经存在
	bool bExist = false;
	TextureInfoVecItr itr_tex = m_kTextureInfoVec.begin();
	TextureInfoVecItr itr_tex_end = m_kTextureInfoVec.end();
	for (; itr_tex != itr_tex_end; ++itr_tex)
	{
		if ((*itr_tex)->spTexture == spTexture)
		{
			bExist = true;
			break;
		}
	}

	// 获取D3D纹理,检查纹理是否满足要求
	//	1.是一张D3D纹理
	//	2.该纹理是一张D3D内存纹理
	//	3.该纹理格式与贴图集相同
	//	4.该纹理是一张方形纹理
	//	5.该纹理尺寸为Power(2,n)
	NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)spTexture->GetRendererData();
	IDirect3DTexture9* spD3DTexture = (IDirect3DTexture9*)spDX9TextureData->GetD3DTexture();
	
	if (NULL == spD3DTexture)	return -1;
	if (D3DRTYPE_TEXTURE != spD3DTexture->GetType())	return -1;

	D3DSURFACE_DESC kDesc;
	spD3DTexture->GetLevelDesc(0, &kDesc);
	if (kDesc.Format != m_eFormat)			return -1;
	if (D3DPOOL_SYSTEMMEM != kDesc.Pool)	return -1;
	if ((kDesc.Width != kDesc.Height) || !NiIsPowerOf2(kDesc.Width))	return -1;
	// @}

	// 新建纹理页
	// @{
	// 计算LOD数量
	uint uiTexNumLevel = spD3DTexture->GetLevelCount();
	if (0 != uiNumLevel)
		uiNumLevel = min(uiNumLevel, uiTexNumLevel);
	else
		uiNumLevel = uiTexNumLevel;

	// 填充输入Texture
	stTextureInfo* pkTexPageInfo = new stTextureInfo;
	NIASSERT(pkTexPageInfo);
	pkTexPageInfo->spTexture		= spTexture;

	pkTexPageInfo->spD3DTexture		= spD3DTexture;
	spD3DTexture->AddRef();			///< GetD3DTexture没有添加引用计数，这里需要手动添加

	pkTexPageInfo->fUVRepeat		= fUVRepeat;
	pkTexPageInfo->fMipmapBias		= fMipmapBias;

	for (uint i = 0; i < uiNumLevel; ++i)
	{
		IDirect3DSurface9* d3dSurface;
		HRESULT hr = spD3DTexture->GetSurfaceLevel(i, &d3dSurface);
		NIASSERT(SUCCEEDED(hr));

		D3DSURFACE_DESC d3dSurfaceDesc;
		hr = d3dSurface->GetDesc(&d3dSurfaceDesc);
		NIASSERT(SUCCEEDED(hr));

		// 跳过尺寸过小的mipmap
		if (d3dSurfaceDesc.Width < 4)
		{
			d3dSurface->Release();
			break;
		}

		NIASSERT(NiIsPowerOf2(d3dSurfaceDesc.Width));
		NIASSERT(d3dSurfaceDesc.Width == d3dSurfaceDesc.Height);

		stLevelInfo kLevelInfo;
		kLevelInfo.uiSize		= d3dSurfaceDesc.Width;
		kLevelInfo.spD3DSurface = d3dSurface;	///< GetSurfaceLevel函数已经自动添加引用
		kLevelInfo.pkTexInfo	= pkTexPageInfo;

		pkTexPageInfo->kLevelInfoVec.push_back(kLevelInfo);
		pkTexPageInfo->uiTotalPixels += d3dSurfaceDesc.Width * d3dSurfaceDesc.Height;
	}

	// 检查最终总大小
	if (!bExist && m_uiTotalPixels + pkTexPageInfo->uiTotalPixels > MAX_ATLAS_TEXTURE_SIZE * MAX_ATLAS_TEXTURE_SIZE)
	{
		LevelInfoVecItr itr = pkTexPageInfo->kLevelInfoVec.begin();
		LevelInfoVecItr itr_end = pkTexPageInfo->kLevelInfoVec.end();
		for (; itr!=itr_end; ++itr)
		{
			itr->spD3DSurface->Release();
			itr->spD3DSurface = NULL;
		}

		pkTexPageInfo->spD3DTexture->Release();
		pkTexPageInfo->spD3DTexture = NULL;

		delete pkTexPageInfo;
		pkTexPageInfo = NULL;

		return -1;
	}

	m_uiTotalPixels += pkTexPageInfo->uiTotalPixels;
	// @}

	// 添加纹理页到列表
	// @{
	if (uiIndex == m_kTextureInfoVec.size())
	{
		m_kTextureInfoVec.push_back(pkTexPageInfo);
	}
	else
	{
		m_kTextureInfoVec.insert(m_kTextureInfoVec.begin() + uiIndex, pkTexPageInfo);
	}
	// @}

	//
	m_bAtlasDirty = true;
	m_bAtlasTableDirty = true;

	return uiIndex;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::RemoveTexture(uint uiIndex)
{
	// 检查输入参数
	if (m_kTextureInfoVec.empty())
		return false;

	if (uiIndex >= m_kTextureInfoVec.size())	
		return false;	

	// 查找其余索引是否存在相同的输入纹理
	// @{
	bool bExistSame = false;
	for (uint i = 0; i < uiIndex; ++i)
	{
		if (m_kTextureInfoVec[i]->spTexture == m_kTextureInfoVec[uiIndex]->spTexture)
		{
			bExistSame = true;
			break;
		}
	}

	if (!bExistSame)
	{
		for (uint i = uiIndex+1; i < m_kTextureInfoVec.size(); ++i)
		{
			if (m_kTextureInfoVec[i]->spTexture == m_kTextureInfoVec[uiIndex]->spTexture)
			{
				bExistSame = true;
				break;
			}
		}
	}
	// @}

	// 移除并销毁纹理页
	// @{
	stTextureInfo* spTextureInfo = m_kTextureInfoVec[uiIndex];
	LevelInfoVecItr itr = spTextureInfo->kLevelInfoVec.begin();
	LevelInfoVecItr itr_end = spTextureInfo->kLevelInfoVec.end();
	for (; itr!=itr_end; ++itr)
	{
		itr->spD3DSurface->Release();
		itr->spD3DSurface = NULL;
	}

	spTextureInfo->spD3DTexture->Release();
	spTextureInfo->spD3DTexture = NULL;

	if (!bExistSame)
		m_uiTotalPixels -= spTextureInfo->uiTotalPixels;

	delete spTextureInfo;
	m_kTextureInfoVec.erase(m_kTextureInfoVec.begin() + uiIndex);
	// @}

	//
	m_bAtlasDirty = true;
	m_bAtlasTableDirty = true;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::SwapTexture(uint uiLIndex, uint uiRIndex)
{
	// 检查输入参数
	if (m_kTextureInfoVec.empty())
		return false;

	if (uiLIndex >= m_kTextureInfoVec.size() || uiRIndex >= m_kTextureInfoVec.size())	
		return false;

	// 索引相同直接返回
	if (uiLIndex == uiRIndex)
		return true;

	//
	stTextureInfo* pkTexInfo = m_kTextureInfoVec[uiLIndex];
	m_kTextureInfoVec[uiLIndex] = m_kTextureInfoVec[uiRIndex];
	m_kTextureInfoVec[uiRIndex] = pkTexInfo;

	//
	m_bAtlasDirty = true;
	m_bAtlasTableDirty = true;

	return true;
}
//-------------------------------------------------------------------------------------------------
NiTexture* sdTextureAtlas::GetTextute(uint uiIndex)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return NULL;

	return m_kTextureInfoVec[uiIndex]->spTexture;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::SetTexture(uint uiIndex, NiTexture *spTexture)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return false;

	stTextureInfo* pkTextureInfo = m_kTextureInfoVec[uiIndex];
	if (pkTextureInfo->spTexture == spTexture)
		return true;

	float fOldUVRepeat = pkTextureInfo->fUVRepeat;
	float fOldMipmapBias = pkTextureInfo->fMipmapBias;
	if (InsertTexture(uiIndex, spTexture, fOldUVRepeat, fOldMipmapBias) == -1)
		return false;

	RemoveTexture(uiIndex + 1);
	return true;
}
//-------------------------------------------------------------------------------------------------
float sdTextureAtlas::GetUVRepeat(uint uiIndex)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return 1.0f;

	return m_kTextureInfoVec[uiIndex]->fUVRepeat;
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::SetUVRepeat(uint uiIndex, float fUVRepeat)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return;

	stTextureInfo* pkTextureInfo = m_kTextureInfoVec[uiIndex];
	if (fabs(pkTextureInfo->fUVRepeat - fUVRepeat) < 0.00001f)
		return;

	pkTextureInfo->fUVRepeat = fUVRepeat;

	m_bAtlasTableDirty = true;
}
//-------------------------------------------------------------------------------------------------
float sdTextureAtlas::GetMipmapBias(uint uiIndex)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return 0.0f;

	return m_kTextureInfoVec[uiIndex]->fMipmapBias;
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::SetMipmapBias(uint uiIndex, float fMipmapBias)
{
	if (uiIndex >= m_kTextureInfoVec.size())
		return;

	stTextureInfo* pkTextureInfo = m_kTextureInfoVec[uiIndex];
	if (fabs(pkTextureInfo->fMipmapBias - fMipmapBias) < 0.00001f)
		return;

	pkTextureInfo->fMipmapBias = fMipmapBias;

	m_bAtlasTableDirty = true;
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::SetLodBias(float fLodBias)
{
	if (fabs(m_fLodBias - fLodBias) < 0.00001f)
		return;

	m_fLodBias = fLodBias;

	m_bAtlasTableDirty = true;
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::GetAtlasTableParam(sdVector2& kTexIdToU, sdVector2& kLevelToV)
{
	kTexIdToU.m_fX = m_kTableInfo.vTexId2U[0];
	kTexIdToU.m_fY = m_kTableInfo.vTexId2U[1];
	kLevelToV.m_fX = m_kTableInfo.vLevel2V[0];
	kLevelToV.m_fY = m_kTableInfo.vLevel2V[1];
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::Update()
{
	// 如果输入AltasTexture,和输入AtlasTexture有变动,则更新之
	if (!UpdateAtlasAndTable())
		return false;

	// 如果输出AtlasTexture和AtlasTableTexture需要更新则更新之
	if (!UpdateRenderTexture())
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::UpdateAtlasAndTable()
{
	// 当前输入为空
	if (0 == m_uiTotalPixels || m_kTextureInfoVec.empty())
		return true;

	// 如果当前输入AtlasTexture被更新了, 重新插入所有Mipmap到SurfaceAtlas
	// @{
	if (m_bAtlasDirty)
	{
		// 计算更新之后应该使用的纹理尺寸
		// (个人觉得计算不准确,应该在长度和宽度两方面共同考虑,至少应该是输入Page的大小的整数倍)
		uint uiAtlasSize = Base::Math::sdMath::Power2_Ceil((uint)ceilf(sqrt((float)m_uiTotalPixels)));
		NIASSERT(uiAtlasSize > 2 && uiAtlasSize <= MAX_ATLAS_TEXTURE_SIZE);

		m_uiAtlasSize = uiAtlasSize;

		// 检查当前AtlasSurface尺寸,不匹配则销毁重新创建之
		if (m_pkSurfaceAtlas && uiAtlasSize != m_pkSurfaceAtlas->GetWidth())
		{
			DestroyAtlasTexture();
			m_pkSurfaceAtlas = 0;
		}

		if (!m_pkSurfaceAtlas)
		{
			m_pkSurfaceAtlas =  NiNew sdSurfaceAtlas(uiAtlasSize, uiAtlasSize, m_eFormat);
			NIASSERT(m_pkSurfaceAtlas);
		}

		// 销毁AtlasSurface所有Tile
		m_pkSurfaceAtlas->DeallocAllTiles();

		// 查找出重复的Texture
		std::map<uint, uint> kRedirect;
		TextureInfoVecItr itr_tex = m_kTextureInfoVec.begin();
		TextureInfoVecItr itr_tex_begin = m_kTextureInfoVec.begin();
		TextureInfoVecItr itr_tex_end = m_kTextureInfoVec.end();
		for (; itr_tex != itr_tex_end; ++itr_tex)
		{
			stTextureInfo* spTexInfo = *itr_tex;

			TextureInfoVecItr itr_tex_inner = m_kTextureInfoVec.begin();
			for (; itr_tex_inner!= itr_tex; ++itr_tex_inner)
			{
				if ((*itr_tex_inner)->spTexture == spTexInfo->spTexture)
				{
					kRedirect.insert(std::make_pair(itr_tex - itr_tex_begin, itr_tex_inner - itr_tex_begin));
				}
			}
		}

		// 提取出AtlasTexture的所有Mipmap(相同的Texture被跳过)
		typedef std::vector<stLevelInfo*> LevelInfoPtrVec;
		typedef std::vector<stLevelInfo*>::iterator LevelInfoPtrVecItr;
		LevelInfoPtrVec kAllLevelInfoPtrVec;
		itr_tex = m_kTextureInfoVec.begin();
		for (; itr_tex != itr_tex_end; ++itr_tex)
		{
			// 不再处理重复的Texture
			if (kRedirect.find(itr_tex - itr_tex_begin) != kRedirect.end())
				continue;

			//
			stTextureInfo* pkTexInfo = *itr_tex;
			NIASSERT(pkTexInfo);
			NIASSERT(!pkTexInfo->kLevelInfoVec.empty());

			LevelInfoVecItr itr_level = pkTexInfo->kLevelInfoVec.begin();
			LevelInfoVecItr itr_level_end = pkTexInfo->kLevelInfoVec.end();
			for (; itr_level != itr_level_end; ++itr_level)
				kAllLevelInfoPtrVec.push_back(&(*itr_level));
		}

		// 排序Mipmap
		sort(kAllLevelInfoPtrVec.begin(), kAllLevelInfoPtrVec.end(), sdTextureAtlas::LevelGreater);

		// 将所有Mipmap加入SurfaceAtlas
		uint uiCurTotalAllocatedPixels = 0;
		LevelInfoPtrVecItr itr_level_ptr = kAllLevelInfoPtrVec.begin();
		LevelInfoPtrVecItr itr_level_ptr_end = kAllLevelInfoPtrVec.end();
		for (; itr_level_ptr != itr_level_ptr_end; ++itr_level_ptr)
		{
			stLevelInfo* spLevelInfo = *itr_level_ptr;
			NIASSERT(spLevelInfo);

			// 计算坐标
			uint uiX, uiY;
			GetCoordinate(uiCurTotalAllocatedPixels, uiX, uiY);
		
			// 上下,左右颠倒
			uint uiLeft = m_uiAtlasSize - uiX - spLevelInfo->uiSize;
			uint uiTop = m_uiAtlasSize - uiY - spLevelInfo->uiSize;

			// 加入到SurfaceAtlas并绑定
			spLevelInfo->uiLeft = uiLeft;
			spLevelInfo->uiTop	= uiTop;
			spLevelInfo->uiTileIndex = m_pkSurfaceAtlas->AllocTile(uiLeft, uiTop, spLevelInfo->uiSize, spLevelInfo->uiSize);
			m_pkSurfaceAtlas->BindSourceSurfaceToTile(spLevelInfo->uiTileIndex, spLevelInfo->spD3DSurface, 0, 0);
		
			//
			uiCurTotalAllocatedPixels += spLevelInfo->uiSize * spLevelInfo->uiSize;
		}

		// 为重复的Texture的所有Level填充Index
		std::map<uint, uint>::iterator itr_map_ui2ui;
		for (itr_map_ui2ui = kRedirect.begin(); itr_map_ui2ui != kRedirect.end(); ++itr_map_ui2ui)
		{
			stTextureInfo* pkDstTexInfo = m_kTextureInfoVec[itr_map_ui2ui->first];
			LevelInfoVec& kDstLevelInfo = pkDstTexInfo->kLevelInfoVec;

			stTextureInfo* pkSrcTexInfo = m_kTextureInfoVec[itr_map_ui2ui->second];
			LevelInfoVec& kSrcLevelInfo = pkSrcTexInfo->kLevelInfoVec;
	
			for (uint i = 0; i < kSrcLevelInfo.size(); ++i)
			{
				kDstLevelInfo[i].uiTileIndex = kSrcLevelInfo[i].uiTileIndex;
				kDstLevelInfo[i].uiLeft = kSrcLevelInfo[i].uiLeft;
				kDstLevelInfo[i].uiTop = kSrcLevelInfo[i].uiTop;
			}
		}

		//
		m_bAtlasDirty = false;
		m_bAtlasTextureDirty = true;
	}
	// @}


	// 如果输入AtlasTable被更新了
	// @{
	if (m_bAtlasTableDirty)
	{
		// 计算Lod范围，影响参数包括：
		//	1.纹理尺寸
		//	2.纹理UVRepeat和MipmapBias
		//	3.整体LodBias
		int iMinLodOffset = 1000;
		int iMaxLodOffset = -1000;
		TextureInfoVecItr itr_tex = m_kTextureInfoVec.begin();
		TextureInfoVecItr itr_tex_end = m_kTextureInfoVec.end();
		for (; itr_tex != itr_tex_end; ++itr_tex)
		{
			stTextureInfo* pkTexInfo = *itr_tex;
			stLevelInfo* pkLevelInfo = &pkTexInfo->kLevelInfoVec[0];

			int iLodOffset = (int)(log(pkLevelInfo->uiSize / pkTexInfo->fUVRepeat) / log(2.0f) + pkTexInfo->fMipmapBias);
			
			iMinLodOffset = min(iMinLodOffset, iLodOffset);
			iMaxLodOffset = max(iMaxLodOffset, iLodOffset);
		}

		//iMinLodOffset += (int)m_fLodBias;
		//iMaxLodOffset += (int)m_fLodBias;

		// 
		const int iMaxMipmapLod = 6;
		int iMinLodFactor = 0 - iMaxLodOffset;
		int iMaxLodFactor = MAX_MIPMAP_LOD - iMinLodOffset;

		//
		m_kTableInfo.uiTextureCount = m_kTextureInfoVec.size();
		m_kTableInfo.uiLevelCount = MAX_MIPMAP_LOD;
	
		// 计算AtlasTable纹理尺寸(向上取power(2,n)),检查当前AtlasTable尺寸,不匹配则销毁重新创建之
		uint uiWidth = Base::Math::sdMath::Power2_Ceil(m_kTableInfo.uiTextureCount);
		uint uiHeight = Base::Math::sdMath::Power2_Ceil(iMaxLodFactor - iMinLodFactor + 1) * 4;
		if (uiWidth != m_kTableInfo.uiWidth || uiHeight != m_kTableInfo.uiHeight)
		{
			DestroyAtlasTableTexture();
			
			m_kTableInfo.uiWidth = uiWidth;
			m_kTableInfo.uiHeight = uiHeight;
		}

		// 用于Shader计算时使用:
		m_kTableInfo.vTexId2U[0] = 1.0f / uiWidth;
		m_kTableInfo.vTexId2U[1] = 0.5f / uiWidth;
		m_kTableInfo.vLevel2V[0] = 1.0f / uiHeight;
		m_kTableInfo.vLevel2V[1] = -iMinLodFactor / (float)uiHeight;

		//
		m_bAtlasTableDirty = false;
		m_bAtlasTableTextureDirty = true;
	}
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::UpdateRenderTexture()
{
	// 设备丢失则直接返回
	if (m_spRenderer->IsDeviceLost())
		return true;


	// AtlasTexture处理
	// @{
	// 输入纹理列表为空则直接返回
	if (0 == m_uiTotalPixels || m_kTextureInfoVec.empty())
		return true;

	// 输出AtlasTexture的D3D纹理非空,且非脏状态直接返回
	if (!m_bAtlasTextureDirty && NULL != m_spD3DAtlasTexture)
		return true;

	// 输出AtlasTexture的D3D纹理不存在则创建之
	if (NULL == m_spD3DAtlasTexture)
	{
		if (!CreateAtlasTexture())
			return false;
	}

	// 进行拼接处理输出到AtlasTexture
	m_pkSurfaceAtlas->UpdateTargetSurface();
	// @}


	// AtlasTable处理
	// @{
	// 输出AtlasTable的D3D纹理不为空,且非脏状态直接返回
	if (!m_bAtlasTableTextureDirty && NULL != m_spD3DAtlasTable)
		return true;

	// 输出AtlasTable的D3D纹理不存在则创建之
	if (NULL == m_spD3DAtlasTable)
	{
		if (!CreateAtlasTableTexture())
			return false;
	} 

	// 处理并生成AtlasTable
	D3DLOCKED_RECT kLockedRect;
	HRESULT hr = m_spD3DAtlasTable->LockRect(0, &kLockedRect, 0, D3DLOCK_DISCARD);
	NIASSERT(SUCCEEDED(hr));

	float* pfDataLine = (float*)kLockedRect.pBits;
	for (uint y = 0; y < m_kTableInfo.uiHeight; ++y)
	{
		float* pfDataPixel = pfDataLine;
		for (uint x = 0; x < m_kTableInfo.uiWidth; ++x)
		{
			// 获取当前纹理(每行为了power(2,n)对齐引起的超出部分的像素使用最后一层有效纹理来计算)
			stTextureInfo* pkTexInfo = m_kTextureInfoVec[min(x, m_kTableInfo.uiTextureCount -1)];

			// 计算当前LOD
			//	j / mTableInfo.height = lodFactor * mTableinfo.levelToV.x + mTableinfo.levelToV.y
			//	lodLevel = lodFactor + lodOffset
			float fLodOffset = log(pkTexInfo->kLevelInfoVec[0].uiSize / pkTexInfo->fUVRepeat) / log(2.0f);
			float fLodFactor = (((float)y) / (m_kTableInfo.uiHeight/* - 1.f */) - m_kTableInfo.vLevel2V[1]) / m_kTableInfo.vLevel2V[0];
			int iLevel = (int)(fLodFactor + fLodOffset /* + m_fLodBias */ + pkTexInfo->fMipmapBias);
			uint uiLevel = NiClamp(iLevel, 0, pkTexInfo->kLevelInfoVec.size() - 1);

			// 提取Tile的信息并保存
			stLevelInfo* pkLevel = &pkTexInfo->kLevelInfoVec[uiLevel];
			NIASSERT(-1 != pkLevel->uiTileIndex);

			pfDataPixel[0] = 1.0f / pkTexInfo->fUVRepeat;
			pfDataPixel[1] = ((float)pkLevel->uiSize - 1.0f) / (float)m_pkSurfaceAtlas->GetWidth();
			pfDataPixel[2] = ((float)pkLevel->uiLeft + 0.5f) / (float)m_pkSurfaceAtlas->GetWidth();
			pfDataPixel[3] = ((float)pkLevel->uiTop +  0.5f) / (float)m_pkSurfaceAtlas->GetHeight();

			// 下一个像素
			pfDataPixel += 4;
		}

		// 下一行(每个float是4个字节)
		pfDataLine += (kLockedRect.Pitch / 4);
	}

	m_spD3DAtlasTable->UnlockRect(0);
	m_bAtlasTableTextureDirty = false;
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::CreateAtlasTexture()
{
	if (m_spD3DAtlasTexture)
		return false;

	// 创建AtlasTexture的D3DTexture
	// (WZ曾经为D3DUSAGE_DYNAMIC/D3DPOOL_DEFAULT，后来被改成0/D3DPOOL_MANAGED)
	HRESULT hr = m_spD3DDevice->CreateTexture(m_uiAtlasSize, m_uiAtlasSize, 1, 0, m_eFormat, D3DPOOL_MANAGED, &m_spD3DAtlasTexture, 0);
	if (FAILED(hr))
		return false;

	// 清空D3DTexture
	//D3DLOCKED_RECT kLockedRect;
	//m_spD3DAtlasTexture->LockRect(0, &kLockedRect, NULL, 0);
	//::ZeroMemory(kLockedRect.pBits, m_uiAtlasSize * m_uiAtlasSize);	///< 貌似这里Size不对
	//m_spD3DAtlasTexture->UnlockRect(0);

	// 包装D3DTexture到GBTexture
	NiDX9Direct3DTextureData::Create((NiDX9Direct3DTexture*)(NiTexture*)m_spAtlasTexture, m_spRenderer, m_spD3DAtlasTexture);

	// 获取D3DSurface
	IDirect3DSurface9* spD3DAtlasSurface;
	hr = m_spD3DAtlasTexture->GetSurfaceLevel(0, &spD3DAtlasSurface);
	if (FAILED(hr))
		return false;

	// 绑定D3DSurface到AtlasSurface
	if (!m_pkSurfaceAtlas->BindTargetSurface(spD3DAtlasSurface))
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::DestroyAtlasTexture()
{
	if (m_spD3DAtlasTexture)
	{
		// 从AtlasSurface解除
		m_pkSurfaceAtlas->UnbindTargetSurface();

		// 从AtlasTexture解除
		NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)m_spAtlasTexture->GetRendererData();
		if (spDX9TextureData)
		{
			m_spAtlasTexture->SetRendererData(NULL);
			NiDelete spDX9TextureData;
		}

		// 销毁D3DTexture
		m_spD3DAtlasTexture->Release();
		m_spD3DAtlasTexture = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::CreateAtlasTableTexture()
{
	if (m_spD3DAtlasTable)
		return false;

	// 创建AtlasTable的D3D
	HRESULT hr = m_spD3DDevice->CreateTexture(m_kTableInfo.uiWidth, m_kTableInfo.uiHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &m_spD3DAtlasTable, 0);
	if (FAILED(hr))
		return false;

	// 绑定到AtlasTable
	NiDX9Direct3DTextureData::Create((NiDX9Direct3DTexture*)(NiTexture*)m_spAtlasTable, m_spRenderer, m_spD3DAtlasTable);

	return true; 
}
//-------------------------------------------------------------------------------------------------
void sdTextureAtlas::DestroyAtlasTableTexture()
{
	if (m_spD3DAtlasTable)
	{
		// 从AtlasTable解除
		NiDX9TextureData* spDX9TextureData = (NiDX9TextureData*)m_spAtlasTable->GetRendererData();
		if (spDX9TextureData)
		{
			m_spAtlasTable->SetRendererData(NULL);
			NiDelete spDX9TextureData;
		}

		// 销毁AtlasTable的D3D纹理
		m_spD3DAtlasTable->Release();
		m_spD3DAtlasTable = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::OnDeviceLost()
{
	DestroyAtlasTexture();
	DestroyAtlasTableTexture();
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::OnDeviceReset(bool bBefore)
{
	if (bBefore)
	{
		// 设备重设前
		DestroyAtlasTexture();
		DestroyAtlasTableTexture();
		return true;
	}
	else
	{
		// 设备已经重设
		UpdateRenderTexture();
		return true;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureAtlas::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	kLuaStream.WriteNodeBegin("TableInfo");
	kLuaStream.WriteData("Width", m_kTableInfo.uiWidth, 15);
	kLuaStream.WriteData("Height", m_kTableInfo.uiHeight, 15);
	kLuaStream.WriteData("TexCount", m_kTableInfo.uiTextureCount, 15);
	kLuaStream.WriteData("LevelCount", m_kTableInfo.uiLevelCount, 15);
	kLuaStream.WriteData("TexId2U", NiPoint2(m_kTableInfo.vTexId2U[0], m_kTableInfo.vTexId2U[1]), 15);
	kLuaStream.WriteData("Level2V", NiPoint2(m_kTableInfo.vLevel2V[0], m_kTableInfo.vLevel2V[1]), 15);
	kLuaStream.WriteNodeEnd();

	kLuaStream.WriteNodeBegin("TextureInfo");
	TextureInfoVecItr itr_tex_info = m_kTextureInfoVec.begin();
	TextureInfoVecItr itr_tex_info_end = m_kTextureInfoVec.end();
	for (; itr_tex_info < itr_tex_info_end; ++itr_tex_info)
	{
		stTextureInfo* pkTextureInfo = *itr_tex_info;
		kLuaStream.LoopInnerBegin();
		kLuaStream.WriteData("LevelCount", pkTextureInfo->kLevelInfoVec.size(), 15);	
		LevelInfoVecItr itr_level_info = pkTextureInfo->kLevelInfoVec.begin();
		LevelInfoVecItr itr_level_info_end = pkTextureInfo->kLevelInfoVec.end();
		for (; itr_level_info != itr_level_info_end; ++itr_level_info)
		{
			const stLevelInfo& kLevelInfo = *itr_level_info;
			kLuaStream.LoopInnerBegin();
			kLuaStream.WriteData("LevelSize", kLevelInfo.uiSize, 15);
			kLuaStream.WriteData("LevelLeft", kLevelInfo.uiSize, 15);
			kLuaStream.WriteData("LevelTop", kLevelInfo.uiSize, 15);
			kLuaStream.LoopInnerEnd();
		}
		kLuaStream.LoopInnerEnd();
	}
	kLuaStream.WriteNodeEnd();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE