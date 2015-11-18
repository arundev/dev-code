#include "sdEnginePCH.h"
#include "sdLayerMap.h"
#include "IResourceSystem.h"
#include "sdMemoryTexture.h"

#include <NiFilename.h>

using namespace std;
using namespace Engine;
using namespace Engine::ResourceSystem;
//-------------------------------------------------------------------------------------------------
sdLayerMap::sdLayerMap(const char* szName, const char* szDiffuseMap, const char* szNormalMap, float fRepeatSize, float fMipmapBias)
: m_fRepeatSize(fRepeatSize)
, m_fMipmapBias(fMipmapBias)
{
	NIASSERT(szName);
	NIASSERT(szDiffuseMap);
	NIASSERT(szNormalMap);

	strncpy(m_szName, szName, MAX_LAYER_NAME_LEN);

	SetDiffuseMapPath(szDiffuseMap);
	SetNormalMapPath(szNormalMap);
}
//-------------------------------------------------------------------------------------------------
sdLayerMap::~sdLayerMap()
{
	
}
//-------------------------------------------------------------------------------------------------
void sdLayerMap::SetDiffuseMapPath(const char* szName)
{
	NIASSERT(szName)
	strncpy(m_szDiffuseMap, szName, MAX_PATH_LEN);

	// ��������
	m_pkDiffuseMap = LoadPixelDataToD3DTexture(szName);
	NIASSERT(m_pkDiffuseMap)
}
//-------------------------------------------------------------------------------------------------
void sdLayerMap::SetNormalMapPath(const char* szName)
{
	NIASSERT(szName)
	strncpy(m_szNormalMap, szName, MAX_PATH_LEN);

	// ��������
	m_pkNormalMap = LoadPixelDataToD3DTexture(szName);
	NIASSERT(m_pkNormalMap);
}
//-------------------------------------------------------------------------------------------------
sdMemoryTexturePtr sdLayerMap::LoadPixelDataToD3DTexture(const char* szName)
{
	//
	// ���´���ο���GB/NiDX9SourceTextureData
	// (NiImageConverter��֧��ת����DXTϵ�и�ʽ)
	//
	//
	// ������չ���ж��ļ���ʽ(Ŀǰ֧��bmp/dds/jpg/png/tga)
	NiFilename kFileName(szName);
	const char* pcExt = kFileName.GetExt();
	if (NiStricmp(pcExt, ".bmp") && NiStricmp(pcExt, ".dds") && NiStricmp(pcExt, ".jpg") && NiStricmp(pcExt, ".png") && NiStricmp(pcExt, ".tga"))
		return 0;

	// ���ļ�,�����ļ����ݵ��ڴ�
	NiFile* pkFile = NiFile::GetFile(szName, NiFile::READ_ONLY);
	if ((!pkFile) || (!(*pkFile)))
	{
		NiDelete pkFile;
		return 0;
	}

	unsigned int uiBufferLength = pkFile->GetFileSize();
	if (uiBufferLength == 0)
	{
		NiDelete pkFile;
		return 0;
	}

	unsigned char* pucBuffer = NiAlloc2(unsigned char, uiBufferLength, NiMemHint::TEXTURE);
	pkFile->Read(pucBuffer, uiBufferLength);

	NiDelete pkFile;
	pkFile = NULL;

	// �����ļ�ͷ��Ϣ
	D3DXIMAGE_INFO kImageInfo;
	HRESULT hr = D3DXGetImageInfoFromFileInMemory((VOID*)pucBuffer, uiBufferLength, &kImageInfo);
	if (FAILED(hr))
	{
		NiFree(pucBuffer);
		pucBuffer = NULL;

		return 0;
	}

	// ��ȡ�ļ���Ϣ��D3D����
	LPDIRECT3DTEXTURE9 pkD3DTexture = NULL;
	if (kImageInfo.ResourceType == D3DRTYPE_TEXTURE)
	{
		// ��ȡ��Ⱦ�豸
		NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
		NIASSERT(spRenderer);

		LPDIRECT3DDEVICE9 spD3DDevice = spRenderer->GetD3DDevice();
		NIASSERT(spD3DDevice);

		// ��ȡ
		hr = D3DXCreateTextureFromFileInMemoryEx(spD3DDevice,
			(VOID*)pucBuffer, uiBufferLength, 
			kImageInfo.Width, kImageInfo.Height, kImageInfo.MipLevels,
			0, D3DFMT_DXT5, D3DPOOL_SYSTEMMEM, 
			D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pkD3DTexture);
	}    
	else
	{
		hr = D3DERR_INVALIDCALL;
	}

	NiFree(pucBuffer);
	pucBuffer = NULL;

	if (FAILED(hr) || pkD3DTexture == NULL)
		return 0;

	// ���´����ڴ�����
	sdMemoryTexturePtr pkMemoryTexture = NiNew sdMemoryTexture(pkD3DTexture);
	NIASSERT(pkMemoryTexture);

	return pkMemoryTexture;
}
//-------------------------------------------------------------------------------------------------