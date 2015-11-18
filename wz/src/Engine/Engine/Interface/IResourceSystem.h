//*************************************************************************************************
// ����:	��Դϵͳ�����ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-12-19
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_INTERFACE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_INTERFACE_H__
#include <sdSingleton.h>
#include "IResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// ��Դ������������(���ڱ��⺯����������)
//-------------------------------------------------------------------------------------------------
class sdResourceParam : public NiMemObject
{
public:
	sdResourceParam(EResType eResType, 
		EResReqPrior eResPrior, 
		EResUsageType eResUsage,
		const std::string& kFileName,		///< �ļ�·��(����·��)
		bool bAbsPath = true,				///< Ĭ��Ϊ����·��
		bool bReleaseOnGC = true,
		const std::string& kChangedFileName = std::string(),
		const std::string& kModelRootName = std::string())
		: m_eResType(eResType), m_eResPrior(eResPrior), m_eResUsage(eResUsage)
		, m_bAbsPath(bAbsPath), m_bReleaseOnGC(bReleaseOnGC)
		, m_kFileName(kFileName), m_kChangedFileName(kChangedFileName), m_kModelRootName(kModelRootName)
	{
		
	}
	
	// ���Է���
	EResType GetResType() const { return m_eResType;}
	EResReqPrior GetResPrior() const { return m_eResPrior;}
	EResUsageType GetUsageType() const { return m_eResUsage;}
	const std::string& GetFileName() const { return m_kFileName;}
	const std::string& GetChangedFileName() const { return m_kChangedFileName;}
	const std::string& GetModelRootName() const { return m_kModelRootName;}
	bool IsReleaseOnGC() const { return m_bReleaseOnGC;}
	bool IsAbsPath() const { return m_bAbsPath;}

protected:
	EResType			m_eResType;			///< ��Դ����	
	EResReqPrior		m_eResPrior;		///< ��Դ�������ȼ�
	EResUsageType		m_eResUsage;		///< ��Դʹ������
	const std::string	m_kFileName;		///< �������,�ļ���
	const std::string	m_kChangedFileName;
	const std::string	m_kModelRootName;
	bool				m_bReleaseOnGC;
	bool				m_bAbsPath;			///< �Ƿ�ʹ�þ���·��	
};
//-------------------------------------------------------------------------------------------------
// IResourceEventListener, ��Դ���ػص��ӿ�
//-------------------------------------------------------------------------------------------------
class IResourceEventListener : public NiMemObject
{
public:
	// ��Դ�������
	virtual void OnLoaded(IResource* pkResource) = 0;
};
//-------------------------------------------------------------------------------------------------
// IResourceSystem, ��Դϵͳ�ӿ�
//-------------------------------------------------------------------------------------------------
class IResourceSystem : public Base::sdTSingleton<IResourceSystem>
{
public:
	virtual ~IResourceSystem() {};

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;

	bool IsInitialized() { return m_bInitialized;}

	// �첽����(Asynchronous)
	// @{
	// �첽����NIF/KFM
	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResourceEventListener* pkObject) = 0;

	// �첽����NIF/KFM
	virtual IResourcePtr AsyncLoadResource(const sdResourceParam& kResParam, 
		IResource::ResTicket& kResTicket,
		IResourceEventListener* pkObject) = 0;

	// �첽ж��NIF/KFM
	virtual void AsyncFreeResource(IResource::ResTicket kResTicket) = 0;

	// �첽����Texture
	virtual NiSourceTexturePtr AsyncLoadTexture(const std::string& szFileName) = 0;
	// @}

	// ͬ������(Synchronous)
	// @{
	// ͬ������NIF�ļ�
	virtual NiNodePtr SyncLoadNif(const char* szFileName) = 0;

	// ͬ������Texture�ļ�
	virtual NiSourceTexturePtr SyncLoadTexture(const char* szFileName, bool bMipmap = true) = 0;

	//
	virtual NiSourceTexturePtr	LoadTexture(const std::string& szFileName, bool bMipmap = true) = 0;
	virtual NiTexturePtr		LoadTextureByD3D(const std::string& szFileName) = 0;
	virtual NiSourceCubeMapPtr	LoadCubeTexture(const std::string& szFileName) = 0;
	virtual NiPixelDataPtr		LoadTextureToMemory(const std::string& szFileName) = 0;
	// @}

	// ��������
	virtual void GarbageCollect() = 0;

	// ��Դ·��ת��������·��
	virtual bool Convert2ResPath(const char* szPath, char* szNewPath) = 0;

	// ��ȡ��Դ�������ӿ�
	static IResourceSystem* GetResourceSystem(){ return (IResourceSystem*)InstancePtr();}

protected:
	bool m_bInitialized;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif