#include "sdResourceSystemPCH.h"
#include "sdResourceSystem.h"
#include "sdNifResource.h"

#include <NiFilename.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
NiSourceTexturePtr sdResourceSystem::LoadTexture(const std::string& szFileName, bool bMipmap)
{
	// ��ȡ����·��
	NiFixedString kFileName(szFileName.c_str());

	// ���·���Ϸ���
	

	// ��������
	NiSourceTexture::FormatPrefs kFormatPrefs;
	if (!bMipmap)
	{
		kFormatPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
		kFormatPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;
	}

	NiSourceTexturePtr spTexture = NiSourceTexture::Create(kFileName, kFormatPrefs);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
NiSourceCubeMapPtr sdResourceSystem::LoadCubeTexture(const std::string &szFileName)
{
	// ��ȡ����·��
	NiFixedString kFileName(("E:\\project_game\\" + szFileName).c_str());

	// ���·���Ϸ���

	// ��������
	NiSourceCubeMapPtr spTexture = NiSourceCubeMap::Create(kFileName, NiRenderer::GetRenderer());
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
NiPixelDataPtr sdResourceSystem::LoadTextureToMemory(const std::string& szFileName)
{
	// ��ȡ����·��
	NiFixedString kFileName(szFileName.c_str());

	// ���·���Ϸ���


	// ��������
	NiImageConverter* pkImageConverter = NiDevImageConverter::GetImageConverter();
	NIASSERT(pkImageConverter);

	NiPixelData* pkPixelData = pkImageConverter->ReadImageFile(szFileName.c_str(), NULL);
	NIASSERT(pkPixelData); 

	return pkPixelData;
}
//-------------------------------------------------------------------------------------------------
NiTexturePtr sdResourceSystem::LoadTextureByD3D(const std::string& szFileName)
{
	// ��ȡ����·��
	NiFixedString kFileName(("E:\\project_game\\" + szFileName).c_str());

	// ���·���Ϸ���


	// ���ļ�,�����ļ����ݵ��ڴ�
	NiFile* pkFile = NiFile::GetFile(kFileName, NiFile::READ_ONLY);
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

	// ��ȡ�ļ���Ϣ��D3D����
	NiDX9Renderer* spDX9Renderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spDX9Renderer);

	LPDIRECT3DTEXTURE9 spD3DTexture = NULL;
	D3DXCreateTextureFromFileInMemoryEx(spDX9Renderer->GetD3DDevice(),
		(VOID*)pucBuffer, uiBufferLength, 0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &spD3DTexture);

	NiFree(pucBuffer);
	pucBuffer = NULL;

	// ��װΪGB����
	NiTexturePtr spGBTexture = spDX9Renderer->CreateNiTextureFromD3DTexture(spD3DTexture);
	NIASSERT(spGBTexture);

	spD3DTexture->Release();
	spD3DTexture = NULL;

	return spGBTexture;
}
//-------------------------------------------------------------------------------------------------
NiNodePtr sdResourceSystem::SyncLoadNif(const char* szFileName)
{
	NIASSERT(szFileName);

	// �����չ��
	NiFilename kFileName(szFileName);
	NiString kFileExt = kFileName.GetExt();
	kFileExt.ToUpper();
	if (!kFileExt.Equals(".NIF"))
		return 0;

	// ת��Ϊ����·��
	char szResPath[MAX_PATH_LEN];
	Convert2ResPath(szFileName, szResPath);

	// ����NIF
	sdNifResource kNifResource(szResPath, false, E_RES_USAGE_NIF_COMMON);
	if (kNifResource.LoadResource(true))
		return kNifResource.GetRootNode();
	else
		return 0;
}
//-------------------------------------------------------------------------------------------------
NiSourceTexturePtr sdResourceSystem::SyncLoadTexture(const char* szFileName, bool bMipmap /* = true */)
{
	NIASSERT(szFileName);

	// ת��Ϊ����·��
	char szResPath[MAX_PATH_LEN];
	Convert2ResPath(szFileName, szResPath);

	// ��������
	NiSourceTexture::FormatPrefs kFormatPrefs;
	if (!bMipmap)
	{
		kFormatPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
		kFormatPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;
	}

	NiSourceTexturePtr spTexture = NiSourceTexture::Create(szResPath, kFormatPrefs);
	NIASSERT(spTexture);

	return spTexture;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE