//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-02-25
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_MANAGER_H__
#define _SD_ENGINE_TEXTURE_MANAGER_H__

#include "ITextureManager.h"
#include "sdFixedStringExt.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdPixelLoadTaskQueue;
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// \TODO �����ֵ俼�ǸĽ�ΪHash����й���
//-------------------------------------------------------------------------------------------------
class sdTextureManager : public ITextureManager
{
public:
	sdTextureManager();
	virtual ~sdTextureManager();

	// �麯���̳�
	virtual bool Initialize();
	virtual void Destroy();
	virtual bool IsInitialized();
	virtual void Update();

	bool LoadSourceTexture(sdSourceTexturePtr spTexture, uchar ucSkipMipmapLvl, bool bAsyn = true);

	virtual sdSourceTexturePtr GetSourceTexture(const char* pcFileName, const sdSourceTexture::CreateParams& kCreateParams, bool& bCreate);
	virtual bool RegisterSourceTexture(sdSourceTexturePtr spSourceTexture);
	virtual bool UnregisterSourceTexture(sdSourceTexturePtr spSourceTexture);

	//

protected:
	bool m_bInitialized;		///< �Ƿ񱻳�ʼ��
	bool m_bForceSynLoad;		///< ǿ��ͬ������
	uchar m_ucMinLoadPriority;	///< ��ͼ������ȼ�
	uchar m_ucMaxLoadPriority;	///< ��߼������ȼ�

	// ���ض���
	sdPixelLoadTaskQueue* m_pkPixelLoadTaskQueue;

	// �����ֵ�(�������д�FileName��SourceTexture��ӳ��)
	typedef TFixedStringMap<sdSourceTexturePtr> SourceTextureDict;
	typedef TFixedStringMap<sdSourceTexturePtr>::iterator SourceTextureDictItr;
	SourceTextureDict m_kSourceTextureDict; 

	Base::Threading::sdCriticalSection m_kLock;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif