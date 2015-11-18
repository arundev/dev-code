//*************************************************************************************************
// 内容:	文件纹理渲染器数据(参考自WZ::Client::Gamebryo的修改,有很多冗余有待于整合)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-01-15
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_DX9_SOURCE_TEXTURE_DATA_H__
#define _SD_ENGINE_DX9_SOURCE_TEXTURE_DATA_H__

#include <d3d9.h>

class NiSourceTexture;
class NiDX9Renderer;
class NiPersistentSrcTextureRendererData;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
class IDataStream;
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// sdDX9SourceTextureData，修订NiSourceTextureData的行为
//	1.
//---------------------------------------------------------------------------------------
class sdDX9SourceTextureData : public NiDX9SourceTextureData
{
public:
	/** \brief 创建SourceTextureData对象
	*		1.数据来源有四种:PersistentData\PixelData\DataStream\File
	*		2.内部有两种方式来创建：直接从数据创建D3D纹理、创建D3D纹理再拷贝数据
	*
	*	@remark
	*	@param[in] pkTexture	纹理对象
	*	@param[in] pkRenderer	渲染器对象
	*	@param[in] uiSkipLevels	跳过的Mipmap等级
	*	@return					设备相关纹理数据
	*	@see	sdDX9SourceTextureData
	*	@note	代码取自NiDX9SourceTextureData::Create()
	*/
	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
		NiDX9Renderer* pkRenderer,
		unsigned uiSkipLevels = 0);

//	/** \brief 从D3DTexture创建SourceTextureData对象
//	*
//	*	@remark
//	*	@param[in] pkTexture	纹理对象
//	*	@param[in] pkRenderer	渲染器对象
//	*	@param[in] d3dTexture	D3D纹理对象
//	*	@return					设备相关纹理数据
//	*	@see	sdDX9SourceTextureData
//	*	@note	代码取自NiDX9SourceTextureData::Create()
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture);
//
//	/** \brief 从PersistentData创建SourceTextureData对象
//	*
//	*	@remark
//	*	@param[in] pkTexture		纹理对象
//	*	@param[in] pkRenderer		渲染器对象
//	*	@param[in] d3dTexture，		D3D纹理对象
//	*	@param[in] pkPersistentData	纹理内存数据对象
//	*	@param[in] uiSkipLevels	
//	*	@return						设备相关纹理数据
//	*	@see	sdDX9SourceTextureData
//	*	@note	
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture,
//		NiPersistentSrcTextureRendererData* pkPersistentData,
//		unsigned uiSkipLevels = 0);
//
//	/** \brief 从PixelData创建SourceTextureData对象
//	*
//	*	@remark
//	*	@param[in] pkTexture		纹理对象
//	*	@param[in] pkRenderer		渲染器对象
//	*	@param[in] d3dTexture，		D3D纹理对象
//	*	@param[in] pkPixelData		纹理内存数据对象
//	*	@param[in] uiSkipLevels	
//	*	@return						设备相关纹理数据
//	*	@see	sdDX9SourceTextureData
//	*	@note	
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture,
//		NiPixelData* pkPixelData,
//		unsigned uiSkipLevels = 0);
//
//	/** \brief 从IDataStream创建SourceTextureData对象
//	*
//	*	@remark
//	*	@param[in] pkTexture		纹理对象
//	*	@param[in] pkRenderer		渲染器对象
//	*	@param[in] d3dTexture，		D3D纹理对象
//	*	@param[in] pkDataStream		纹理内存数据对象
//	*	@param[in] uiSkipLevels	
//	*	@return						设备相关纹理数据
//	*	@see	sdDX9SourceTextureData
//	*	@note	
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture,
//		Engine::FileSystem::IDataStream* pkDataStream,
//		unsigned uiSkipLevels = 0);

protected:
	sdDX9SourceTextureData(NiSourceTexture* pkTexture, 
		NiDX9Renderer* pkRenderer);

	/** \brief 从PixelData创建D3D纹理对象(只是创建没有拷贝数据)
	*
	*	@remark
	*	@param[in] kPixelData	内存纹理数据
	*	@param[in] uiSkipLevel	跳过的Mipmap等级
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	代码取自NiDX9SourceTextureData::CreateSurf()
	*/
	bool CreateSurfEx(const NiPixelData& kPixels, 
		unsigned int uiSkipLevel = 0);

	/** \brief 从PersistentData创建D3D纹理对象(只是创建没有拷贝数据)
	*
	*	@remark
	*	@param[in] pkPersistentSrcRendererData	内存纹理数据
	*	@param[in] eD3DFmt						纹理创建格式
	*	@param[in] uiSkipLevel					跳过的Mipmap等级
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	代码取自NiDX9SourceTextureData::CreateSurfFromRendererData()
	*/
	bool CreateSurfFromRendererDataEx(
		const NiDX9PersistentSrcTextureRendererData* pkPersistentSrcRendererData,
		D3DFORMAT eD3DFmt,
		unsigned int uiSkipLevel = 0);

	/** \brief 从IDataStream创建D3D纹理对象(只是创建没有拷贝数据)
	*
	*	@remark
	*	@param[in] pkDataStream	内存纹理数据
	*	@param[in] uiSkipLevel	跳过的Mipmap等级
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	bool CreateSurfFromDataStreamEx(
		const Engine::FileSystem::IDataStream* pkDataStream,
		unsigned int uiSkipLevel = 0);

	/** \brief 从IDataStream拷贝数据到D3D纹理对象
	*
	*	@remark
	*	@param[in] pkDataStream	内存纹理数据
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	void CopyStreamDataToSurfaceEx(const Engine::FileSystem::IDataStream* pkDataStream);

	/** \brief 从指定磁盘文件创建设备纹理数据
	*		1.支持从指定Mipmap开始加载(未验证)
	*
	*	@remark
	*	@param[in] pcFilename	文件路径
	*	@param[in] pkRenderer	渲染器
	*	@param[in] kPrefs		加载格式控制
	*	@param[in] uiSkipLevel	
	*	@return					设备相关纹理数据
	*	@see	sdDX9SourceTextureData
	*	@note	代码取自NiDX9SourceTextureData::LoadTextureFile()
	*/
	bool LoadTextureFileEx(const char* pcFilename, 
		NiDX9Renderer* pkRenderer,
		const NiTexture::FormatPrefs& kPrefs, 
		unsigned int uiSkipLevel = 0);

	/** \brief 检查纹理尺寸是否符合Render的标准
	*
	*	@remark
	*	@param[in] uiWidth，uiHeight
	*	@param[in] pkRenderer
	*	@return	
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	static bool CheckTextureDimensions(unsigned int uiWidth, 
		unsigned int uiHeight, NiDX9Renderer* pkDX9Renderer);

	/** \brief 检查纹理格式是否符合Render的标准
	*
	*	@remark
	*	@param[in] eD3DFmt
	*	@param[in] pkRenderer
	*	@return	
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	static bool CheckTextureFormat(D3DFORMAT eD3DFmt, 
		NiDX9Renderer* pkDX9Renderer);

protected:
	//D3DPOOL m_d3dCreatePool;		///< D3D纹理创建的Pool方式
	//D3DFORMAT m_d3dCreateFormat;	///< D3D纹理创建的格式
};
NiSmartPointer(sdDX9SourceTextureData);
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif