//*************************************************************************************************
// 内容:	资源系统基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-19
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_INTERFACE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_INTERFACE_H__
#include <sdSingleton.h>
#include "IResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// 资源创建参数对象(用于避免函数参数过多)
//-------------------------------------------------------------------------------------------------
class sdResourceParam : public NiMemObject
{
public:
	sdResourceParam(EResType eResType, 
		EResReqPrior eResPrior, 
		EResUsageType eResUsage,
		const std::string& kFileName,		///< 文件路径(绝对路径)
		bool bAbsPath = true,				///< 默认为绝对路径
		bool bReleaseOnGC = true,
		const std::string& kChangedFileName = std::string(),
		const std::string& kModelRootName = std::string())
		: m_eResType(eResType), m_eResPrior(eResPrior), m_eResUsage(eResUsage)
		, m_bAbsPath(bAbsPath), m_bReleaseOnGC(bReleaseOnGC)
		, m_kFileName(kFileName), m_kChangedFileName(kChangedFileName), m_kModelRootName(kModelRootName)
	{
		
	}
	
	// 属性访问
	EResType GetResType() const { return m_eResType;}
	EResReqPrior GetResPrior() const { return m_eResPrior;}
	EResUsageType GetUsageType() const { return m_eResUsage;}
	const std::string& GetFileName() const { return m_kFileName;}
	const std::string& GetChangedFileName() const { return m_kChangedFileName;}
	const std::string& GetModelRootName() const { return m_kModelRootName;}
	bool IsReleaseOnGC() const { return m_bReleaseOnGC;}
	bool IsAbsPath() const { return m_bAbsPath;}

protected:
	EResType			m_eResType;			///< 资源类型	
	EResReqPrior		m_eResPrior;		///< 资源加载优先级
	EResUsageType		m_eResUsage;		///< 资源使用类型
	const std::string	m_kFileName;		///< 输入参数,文件名
	const std::string	m_kChangedFileName;
	const std::string	m_kModelRootName;
	bool				m_bReleaseOnGC;
	bool				m_bAbsPath;			///< 是否使用绝对路径	
};
//-------------------------------------------------------------------------------------------------
// IResourceEventListener, 资源加载回调接口
//-------------------------------------------------------------------------------------------------
class IResourceEventListener : public NiMemObject
{
public:
	// 资源加载完成
	virtual void OnLoaded(IResource* pkResource) = 0;
};
//-------------------------------------------------------------------------------------------------
// IResourceSystem, 资源系统接口
//-------------------------------------------------------------------------------------------------
class IResourceSystem : public Base::sdTSingleton<IResourceSystem>
{
public:
	virtual ~IResourceSystem() {};

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;

	bool IsInitialized() { return m_bInitialized;}

	// 异步加载(Asynchronous)
	// @{
	// 异步加载NIF/KFM
	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResourceEventListener* pkObject) = 0;

	// 异步加载NIF/KFM
	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResource::ResTicket& kResTicket,
		IResourceEventListener* pkObject) = 0;

	// 异步卸载NIF/KFM
	virtual void AsyncFreeResource(IResource::ResTicket kResTicket) = 0;

	// 异步加载Texture
	virtual NiSourceTexturePtr AsyncLoadTexture(const std::string& szFileName) = 0;
	// @}

	// 同步加载(Synchronous)
	// @{
	// 同步加载NIF文件
	virtual NiNodePtr SyncLoadNif(const char* szFileName) = 0;

	// 同步加载Texture文件
	virtual NiSourceTexturePtr SyncLoadTexture(const char* szFileName, bool bMipmap = true) = 0;

	//
	virtual NiSourceTexturePtr	LoadTexture(const std::string& szFileName, bool bMipmap = true) = 0;
	virtual NiTexturePtr		LoadTextureByD3D(const std::string& szFileName) = 0;
	virtual NiSourceCubeMapPtr	LoadCubeTexture(const std::string& szFileName) = 0;
	virtual NiPixelDataPtr		LoadTextureToMemory(const std::string& szFileName) = 0;
	// @}

	// 垃圾回收
	virtual void GarbageCollect() = 0;

	// 资源路径转换到绝对路径
	virtual bool Convert2ResPath(const char* szPath, char* szNewPath) = 0;

	// 获取资源管理器接口
	static IResourceSystem* GetResourceSystem(){ return (IResourceSystem*)InstancePtr();}

protected:
	bool m_bInitialized;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif