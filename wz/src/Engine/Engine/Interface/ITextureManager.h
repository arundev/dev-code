//*************************************************************************************************
// 内容:	纹理管理器接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-20
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_MANAGER_INTERFACE_H__
#define _SD_ENGINE_TEXTURE_MANAGER_INTERFACE_H__

#include <sdSingleton.h>
#include "sdSourceTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// 加载回调监听器
//---------------------------------------------------------------------------------------
//class ISourceTextureLoadListener : public NiMemObject
//{
//public:
//	virtual void OnLoadingStatus(sdSourceTexture::ETexStatus eTexStatus) = 0;
//};
//---------------------------------------------------------------------------------------
// ITextureManager
//
// 纹理管理器
//	1.负责SourceTexture纹理的加载卸载策略
//	2.负责SourceTexture创建，保证同一个参数创建的纹理对象只有一份
//---------------------------------------------------------------------------------------
class ITextureManager : public Base::sdTSingleton<ITextureManager>
{
public:
	virtual ~ITextureManager(){}

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;
	virtual void Update() = 0;
 
	/** \brief 向管理器请求加载指定的SourceTexture(内部接口)
	*
	*	@remark 
	*	@param[in]	spTexture		纹理对象
	*	@param[in]	ucSkipMipmapLvl	纹理跳过的Mipmap
	*	@param[in]	bAsyn			是否同步加载
	*	@return						查找到的纹理对象(如果已经注册同文件名的对象则返回false)
	*	@see	ITextureManager
	*	@note	可能有多个线程同时调用
	*/
	virtual bool LoadSourceTexture(sdSourceTexturePtr spTexture,
		uchar ucSkipMipmapLvl,
		bool bAsyn = true) = 0;

	virtual bool DecreaseSourceTexture(sdSourceTexturePtr spTexture,
		uchar ucSkipMipmapLvl,
		bool bAsyn = true) = 0;

	/** \brief 向管理器请求指定的SourceTexture(不存在则创建之)
	*
	*	@remark 
	*	@param[in]	pcFileName		纹理路径
	*	@param[in]	kCreateParams	纹理创建参数
	*	@param[out] bCreate			
	*	@return						查找到的纹理对象(如果已经注册同文件名的对象则返回false)
	*	@see	ITextureManager
	*	@note	可能有多个线程同时调用,内部必须是原子操作
	*/
	virtual sdSourceTexturePtr GetSourceTexture(const char* pcFileName, 
		const sdSourceTexture::CreateParams& kCreateParams,
		const bool& bCreate) = 0;

	/** \brief 向管理器注册SourceTexture
	*
	*	@remark
	*	@param[in] pkSourceTexture	纹理对象
	*	@return						是否注册成功(如果已经注册同文件名的对象则返回false)
	*	@see	ITextureManager
	*	@note	
	*/
	virtual bool RegisterSourceTexture(sdSourceTexturePtr spSourceTexture) = 0;

	/** \brief 向管理器反注册SourceTexture
	*
	*	@remark
	*	@param[in] pkSourceTexture	纹理对象	
	*	@return						是否反注册成功
	*	@see	ITextureManager
	*	@note	
	*/
	virtual bool UnregisterSourceTexture(sdSourceTexturePtr spSourceTexture) = 0;

	//
	//virtual void AddSynLoadTexture(NiSourceTexture* spTexture) = 0;

	// 发送卸载请求,
	virtual bool Unload(NiSourceTexture* spTexture) = 0;

	//
	//virtual bool TaskIsProcessing(NiSourceTexture* spTexture) = 0;

	//virtual void StopBackgroundTask() = 0;
	//virtual void StartBackgroundTask() = 0;

	//
	//virtual void SetForceSynLoad(bool bValue) = 0;
	//virtual void SetLoadPriority(uchar ucMin, uchar ucMax) = 0;


	virtual void ReloadCache(uchar ucLevel) = 0;

	//
	virtual int GetTextureSize(int& iDxt1, int&iDxt3, int& iDxt5, int& iSynTexSize) = 0;

public:
	// 获取管理器接口
	static ITextureManager* GetTextureManager(){ return InstancePtr();}
};
//---------------------------------------------------------------------------------------
// ITextureLodManager
//
// 管理纹理LOD，根据当前纹理内存大小，动态确定所有纹理Lod等级，以实现对纹理内存的控制
// 目前对所有的纹理采用的是单一的LOD值，后续期望改进为按照内存配额采取不同的处理策略和控制参数
//---------------------------------------------------------------------------------------
class ITextureLodManager : public Base::sdTSingleton<ITextureLodManager>
{
public:
	virtual ~ITextureLodManager(){}

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// 更新
	virtual void Update() = 0;

	// 
	virtual uchar GetBaseSkipLevel() = 0;
	virtual void SetBaseSkipLevel(uchar uiLevel) = 0;

	virtual void SetEnableDynamicLevel(bool bEnable) = 0;
	virtual bool GetEnableDynamicLevel() = 0;

	// 获取当前LOD等级
	virtual uchar GetSkipLevel() = 0;

public:
	// 获取管理器接口
	static ITextureLodManager* GetTextureLodManager(){ return InstancePtr();}
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif