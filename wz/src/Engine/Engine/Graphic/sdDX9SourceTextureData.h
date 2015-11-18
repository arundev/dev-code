//*************************************************************************************************
// ����:	�ļ�������Ⱦ������(�ο���WZ::Client::Gamebryo���޸�,�кܶ������д�������)
//---------------------------------------------------------
// ����:		
// ����:		2014-01-15
// ����޸�:
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
// sdDX9SourceTextureData���޶�NiSourceTextureData����Ϊ
//	1.
//---------------------------------------------------------------------------------------
class sdDX9SourceTextureData : public NiDX9SourceTextureData
{
public:
	/** \brief ����SourceTextureData����
	*		1.������Դ������:PersistentData\PixelData\DataStream\File
	*		2.�ڲ������ַ�ʽ��������ֱ�Ӵ����ݴ���D3D��������D3D�����ٿ�������
	*
	*	@remark
	*	@param[in] pkTexture	�������
	*	@param[in] pkRenderer	��Ⱦ������
	*	@param[in] uiSkipLevels	������Mipmap�ȼ�
	*	@return					�豸�����������
	*	@see	sdDX9SourceTextureData
	*	@note	����ȡ��NiDX9SourceTextureData::Create()
	*/
	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
		NiDX9Renderer* pkRenderer,
		unsigned uiSkipLevels = 0);

//	/** \brief ��D3DTexture����SourceTextureData����
//	*
//	*	@remark
//	*	@param[in] pkTexture	�������
//	*	@param[in] pkRenderer	��Ⱦ������
//	*	@param[in] d3dTexture	D3D�������
//	*	@return					�豸�����������
//	*	@see	sdDX9SourceTextureData
//	*	@note	����ȡ��NiDX9SourceTextureData::Create()
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture);
//
//	/** \brief ��PersistentData����SourceTextureData����
//	*
//	*	@remark
//	*	@param[in] pkTexture		�������
//	*	@param[in] pkRenderer		��Ⱦ������
//	*	@param[in] d3dTexture��		D3D�������
//	*	@param[in] pkPersistentData	�����ڴ����ݶ���
//	*	@param[in] uiSkipLevels	
//	*	@return						�豸�����������
//	*	@see	sdDX9SourceTextureData
//	*	@note	
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture,
//		NiPersistentSrcTextureRendererData* pkPersistentData,
//		unsigned uiSkipLevels = 0);
//
//	/** \brief ��PixelData����SourceTextureData����
//	*
//	*	@remark
//	*	@param[in] pkTexture		�������
//	*	@param[in] pkRenderer		��Ⱦ������
//	*	@param[in] d3dTexture��		D3D�������
//	*	@param[in] pkPixelData		�����ڴ����ݶ���
//	*	@param[in] uiSkipLevels	
//	*	@return						�豸�����������
//	*	@see	sdDX9SourceTextureData
//	*	@note	
//	*/
//	static sdDX9SourceTextureData* Create(sdSourceTexture* pkTexture, 
//		NiDX9Renderer* pkRenderer,
//		IDirect3DTexture9* d3dTexture,
//		NiPixelData* pkPixelData,
//		unsigned uiSkipLevels = 0);
//
//	/** \brief ��IDataStream����SourceTextureData����
//	*
//	*	@remark
//	*	@param[in] pkTexture		�������
//	*	@param[in] pkRenderer		��Ⱦ������
//	*	@param[in] d3dTexture��		D3D�������
//	*	@param[in] pkDataStream		�����ڴ����ݶ���
//	*	@param[in] uiSkipLevels	
//	*	@return						�豸�����������
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

	/** \brief ��PixelData����D3D�������(ֻ�Ǵ���û�п�������)
	*
	*	@remark
	*	@param[in] kPixelData	�ڴ���������
	*	@param[in] uiSkipLevel	������Mipmap�ȼ�
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	����ȡ��NiDX9SourceTextureData::CreateSurf()
	*/
	bool CreateSurfEx(const NiPixelData& kPixels, 
		unsigned int uiSkipLevel = 0);

	/** \brief ��PersistentData����D3D�������(ֻ�Ǵ���û�п�������)
	*
	*	@remark
	*	@param[in] pkPersistentSrcRendererData	�ڴ���������
	*	@param[in] eD3DFmt						��������ʽ
	*	@param[in] uiSkipLevel					������Mipmap�ȼ�
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	����ȡ��NiDX9SourceTextureData::CreateSurfFromRendererData()
	*/
	bool CreateSurfFromRendererDataEx(
		const NiDX9PersistentSrcTextureRendererData* pkPersistentSrcRendererData,
		D3DFORMAT eD3DFmt,
		unsigned int uiSkipLevel = 0);

	/** \brief ��IDataStream����D3D�������(ֻ�Ǵ���û�п�������)
	*
	*	@remark
	*	@param[in] pkDataStream	�ڴ���������
	*	@param[in] uiSkipLevel	������Mipmap�ȼ�
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	bool CreateSurfFromDataStreamEx(
		const Engine::FileSystem::IDataStream* pkDataStream,
		unsigned int uiSkipLevel = 0);

	/** \brief ��IDataStream�������ݵ�D3D�������
	*
	*	@remark
	*	@param[in] pkDataStream	�ڴ���������
	*	@return					
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	void CopyStreamDataToSurfaceEx(const Engine::FileSystem::IDataStream* pkDataStream);

	/** \brief ��ָ�������ļ������豸��������
	*		1.֧�ִ�ָ��Mipmap��ʼ����(δ��֤)
	*
	*	@remark
	*	@param[in] pcFilename	�ļ�·��
	*	@param[in] pkRenderer	��Ⱦ��
	*	@param[in] kPrefs		���ظ�ʽ����
	*	@param[in] uiSkipLevel	
	*	@return					�豸�����������
	*	@see	sdDX9SourceTextureData
	*	@note	����ȡ��NiDX9SourceTextureData::LoadTextureFile()
	*/
	bool LoadTextureFileEx(const char* pcFilename, 
		NiDX9Renderer* pkRenderer,
		const NiTexture::FormatPrefs& kPrefs, 
		unsigned int uiSkipLevel = 0);

	/** \brief �������ߴ��Ƿ����Render�ı�׼
	*
	*	@remark
	*	@param[in] uiWidth��uiHeight
	*	@param[in] pkRenderer
	*	@return	
	*	@see	sdDX9SourceTextureData
	*	@note	
	*/
	static bool CheckTextureDimensions(unsigned int uiWidth, 
		unsigned int uiHeight, NiDX9Renderer* pkDX9Renderer);

	/** \brief ��������ʽ�Ƿ����Render�ı�׼
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
	//D3DPOOL m_d3dCreatePool;		///< D3D��������Pool��ʽ
	//D3DFORMAT m_d3dCreateFormat;	///< D3D�������ĸ�ʽ
};
NiSmartPointer(sdDX9SourceTextureData);
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif