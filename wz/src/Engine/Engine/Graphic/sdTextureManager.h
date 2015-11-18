//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-25
// 最后修改:
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
// \TODO 纹理字典考虑改进为Hash表进行管理
//-------------------------------------------------------------------------------------------------
class sdTextureManager : public ITextureManager
{
public:
	sdTextureManager();
	virtual ~sdTextureManager();

	// 虚函数继承
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
	bool m_bInitialized;		///< 是否被初始化
	bool m_bForceSynLoad;		///< 强制同步加载
	uchar m_ucMinLoadPriority;	///< 最低加载优先级
	uchar m_ucMaxLoadPriority;	///< 最高加载优先级

	// 加载队列
	sdPixelLoadTaskQueue* m_pkPixelLoadTaskQueue;

	// 纹理字典(保存所有从FileName到SourceTexture的映射)
	typedef TFixedStringMap<sdSourceTexturePtr> SourceTextureDict;
	typedef TFixedStringMap<sdSourceTexturePtr>::iterator SourceTextureDictItr;
	SourceTextureDict m_kSourceTextureDict; 

	Base::Threading::sdCriticalSection m_kLock;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif