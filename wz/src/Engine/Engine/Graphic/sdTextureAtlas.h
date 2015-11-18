//*************************************************************************************************
// ����:	��ͼͼ��
//---------------------------------------------------------
// ����:		
// ����:		2012-08-14
// ����޸�:	2013-05-15
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_ATLAS_H__
#define _SD_ENGINE_TEXTURE_ATLAS_H__
#include "sdTexture.h"
#include "sdSurfaceAtlas.h"

#include <sdVector2.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
class sdLuaReadUtil;
class sdLuaWriteUtil;
ENGINE_NAMESPACE_END_UTIL
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// �Ӷ���Texture������һ�Ŵ����ͼͼ��,Ҫ��:
//	1.ÿҳ��ͼ����D3D�ڴ�����
//	2.ÿҳ��ͼ����ͼ������ͬ�ĸ�ʽ
//	3.ÿҳ��ͼΪ��ͼ��ָ���ߴ�(�ߴ������pow(2,n),�Ҵ�С���)
//
// �������ɵ�ͼ��:
//	1.�����½ǿ�ʼ
//	2.��Mipmap0��ʼ,�Ӵ�С
// 
// �������ɵ�ͼ����Ŀ¼����:
//	1.ÿһ�д���һ��LOD
//	2.ÿһ�д���һ��������Ϣ,���ϵ����Ǹ���LOD��Ϣ
//
// �̳���NiTexture��������豸������Ϣ
//
// ��������WZ,�����Ķ��ο�,ʵ�����б䶯:
// @{
//
// How to use table texture:
//	lodLevel = max(log2(tan(fov.xy / 2.f) * 2.f * distance / uvRepeat * texSize / screenSize.xy / max(dot(V, N), 0.001f)))
//			 = max(log2(tan(fov.xy / 2.f) * 2.f * distance / screenSize.xy / max(dot(V, N), 0.001)) * (texSize / uvRepeat))
//			 = max(log2(tan(fov.xy / 2.f) * 2.f * distance / screenSize.xy / max(dot(V, N), 0.001))) + log2(texSize / uvRepeat)
//				
//	lodFactor = max(log2(tan(fov.xy / 2.0) * 2.0 * distance / screenSize.xy / max(dot(V, N), 0.001)));
//	lodOffset = log2(texSize / uvRepeat)
//	lodBias   = 0.0
//	lodLevel  = lodFactor + lodOffset + lodBias;
//
//	Now lodFactor is a value independent to subtexture, it can be calculate in shader.
//	lodOffset is subtexture relative values, they can be passed into shader through shader constants.
//	lodBias is also passed into shader through constant.
//
//	Actually, lodLevel = lodFactor + lodOffset + lodBias;
//	
//	Normally,we only sampler from level 0 to level 6.
//	So we map the lodLevel for [0,6] to [0.f, 1.f], and use it as table texture's uv.x.
//	And we map the texture index to [0.f, 1.f] and use it as table texture's uv.y.
//
//	lodFactor������texture,������shader���������
//	lodOffset��texture���,��Ҫ���ݵ�shader
//	lodBias�ǹ̶�ƫ��,��Ҫ���ݵ�shader
//
// ʵ����,���ǽ���ʹ0~6����mipmap,
//	1.ӳ��[0,6]��[0.0, 1.0], ��Ӧtable texture��v����
//	2.ӳ��texture id��[0.0 1.0], ��Ӧtable texture��u����
//
// @}
//
class sdTextureAtlas : public sdTexture
{
public:
	sdTextureAtlas(D3DFORMAT eFormat);
	~sdTextureAtlas();

	// ����ͼ��
	bool	Update();

	// ͼ��������ͼ����
	// @{
	// ���β��Ӻ�ɾ������(ʧ�ܷ���-1,��0xffffffff)
	uint	AddTexture(NiTexture* spTexture, float fUVRepeat = 1.0f, float fMipmapBias = 0.0f,  uint uiNumLevel = 0);
	bool	RemoveTexture();

	// ������λ����Ӻ�ɾ������
	uint	InsertTexture(uint uiIndex, NiTexture* spTexture, float fUVRepeat = 1.0f, float fMipmapBias = 0.0f,  uint uiNumLevel = 0);
	bool	RemoveTexture(uint uiIndex);

	// ������������
	bool	SwapTexture(uint uiLIndex, uint uiRIndex);

	// ͼ�����Բ���
	NiTexture*	GetTextute(uint uiIndex);
	bool		SetTexture(uint uiIndex, NiTexture* spTexture);

	float		GetUVRepeat(uint uiIndex);
	void		SetUVRepeat(uint uiIndex, float fUVRepeat);

	float		GetMipmapBias(uint uiIndex);
	void		SetMipmapBias(uint uiIndex, float fMipmapBias);
	// @}

	// ����LODƫ��(ʵ���ϻ���Mipmap)
	float		GetLodBias() const { return m_fLodBias;};
	void		SetLodBias(float fLodBias);

	// ���Է���
	NiTexture*	GetGBTexture() const { return m_spAtlasTexture;}
	NiTexture*	GetGBTextureTable() const { return m_spAtlasTable;}

	void	GetAtlasTableParam(Base::Math::sdVector2& kTexIdToU, Base::Math::sdVector2& kLevelToV);

	// �����뱣��
	bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	
protected:
	//
	bool	UpdateAtlasAndTable();
	bool	UpdateRenderTexture();

	// ����������������ͼ����D3D����
	bool	CreateAtlasTexture();
	void	DestroyAtlasTexture();

	// ������������ͼ�����ұ��D3D����
	bool	CreateAtlasTableTexture();
	void	DestroyAtlasTableTexture();

	// �豸�ص�����
	virtual bool OnDeviceLost();
	virtual bool OnDeviceReset(bool bBefore);

	// �����Ѿ���������ؼ���
	static void GetCoordinate(uint uiNumAllAllocatedPixels, uint& uiX, uint& uiY);

protected:
	struct stTextureInfo;

	// ��ͼ������Texture��һ��Mipmap,��ӦSurfaceAtlas�е�һ��Tile
	struct stLevelInfo
	{
		uint	uiSize;						///< ��ǰ�㼶�ߴ�
		IDirect3DSurface9*	spD3DSurface;	///< ��ǰ�㼶D3D����

		// SurfaceAtlas���
		uint	uiTileIndex;	///< ��������SurfaceAtlas
		uint	uiLeft;			///< ����õ�����SurfaceAtlas����λ��
		uint	uiTop;	

		// ������
		stTextureInfo*	pkTexInfo;

		stLevelInfo() : uiSize(0), spD3DSurface(NULL), uiTileIndex(-1), uiLeft(0), uiTop(0), pkTexInfo(NULL) {}
	};
	typedef std::vector<stLevelInfo> LevelInfoVec;
	typedef std::vector<stLevelInfo>::iterator LevelInfoVecItr;

	// ��ͼ����һ������Texture,���ܰ������Mipmap����
	struct stTextureInfo
	{
		// ����
		NiTexture*			spTexture;		///< ��������
		IDirect3DTexture9*	spD3DTexture;	///< ���������D3D����
		LevelInfoVec		kLevelInfoVec;	///< ��������Mipmap��Ϣ
		uint				uiTotalPixels;	///< ����ʹ�õ�Mipmap�����ش�С
			
		// ����
		float		fUVRepeat;
		float		fMipmapBias;

		stTextureInfo(): spTexture(NULL), spD3DTexture(NULL), uiTotalPixels(0), fUVRepeat(1.0f), fMipmapBias(0.0f) {}
	};
	typedef std::vector<stTextureInfo*> TextureInfoVec;
	typedef std::vector<stTextureInfo*>::iterator TextureInfoVecItr;

	// LevelInfo����������
	static bool LevelGreater(stLevelInfo* lhs, stLevelInfo* rhs);

	// ��ͼ��Ŀ¼
	struct stTableInfo
	{
		uint	uiWidth;		///< AtlasTableTexture�ߴ�
		uint	uiHeight;
		uint	uiTextureCount;	///< Atlas������ҳ��(��ͬ����ͬ������2��)
		uint	uiLevelCount;	///< Atlas���Mipmap����

		// ����Shader����
		float	vTexId2U[2];
		float	vLevel2V[2];

		stTableInfo() : uiWidth(0), uiHeight(0), uiTextureCount(0), uiLevelCount(0){ vTexId2U[0] = vTexId2U[1] = 0; vLevel2V[0] = vLevel2V[1] = 0;}
	};

protected:
	// ͼ��������ͼ��Ϣ
	TextureInfoVec	m_kTextureInfoVec;	///< ����������ͼ
	uint	m_uiTotalPixels;			///< ��ǰAtlas����Page��������Ŀ
	bool	m_bAtlasDirty;				///< ��ǰAtlas����Page�Ƿ񱻸�����
	bool	m_bAtlasTextureDirty;		///< ��ǰ���AtlasTexture�Ƿ���Ҫ����

	// ͼ��Ŀ¼��Ϣ
	stTableInfo	m_kTableInfo;
	bool	m_bAtlasTableDirty;			///< ��ǰAtlasTable�Ƿ񱻸�����
	bool	m_bAtlasTableTextureDirty;	///< ��ǰ���AtlasTableTexture�Ƿ���Ҫ����
	float	m_fLodBias;

	// ͼ��ƴ�ӹ���
	sdSurfaceAtlasPtr	m_pkSurfaceAtlas;

	// ����ͼ��
	D3DFORMAT	m_eFormat;					///< ͼ�������ʽ
	uint		m_uiAtlasSize;				///< ͼ������ߴ�
	NiTexturePtr		m_spAtlasTexture;	///< ͼ���������
	IDirect3DTexture9*	m_spD3DAtlasTexture;///< ͼ���������D3D����

	// ����ͼ��Ŀ¼
	NiTexturePtr		m_spAtlasTable;		///< ͼ��Ŀ¼�������
	IDirect3DTexture9*	m_spD3DAtlasTable;	///< ͼ��Ŀ¼�������D3D����

	// ��Ⱦ�豸
	NiDX9Renderer*		m_spRenderer;
	LPDIRECT3DDEVICE9	m_spD3DDevice;
};
NiSmartPointer(sdTextureAtlas);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif