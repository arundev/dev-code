//*************************************************************************************************
// ����:	����������ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2013-02-20
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_MANAGER_INTERFACE_H__
#define _SD_ENGINE_TEXTURE_MANAGER_INTERFACE_H__

#include <sdSingleton.h>
#include "sdSourceTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// ���ػص�������
//---------------------------------------------------------------------------------------
//class ISourceTextureLoadListener : public NiMemObject
//{
//public:
//	virtual void OnLoadingStatus(sdSourceTexture::ETexStatus eTexStatus) = 0;
//};
//---------------------------------------------------------------------------------------
// ITextureManager
//
// ���������
//	1.����SourceTexture����ļ���ж�ز���
//	2.����SourceTexture��������֤ͬһ�������������������ֻ��һ��
//---------------------------------------------------------------------------------------
class ITextureManager : public Base::sdTSingleton<ITextureManager>
{
public:
	virtual ~ITextureManager(){}

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;
	virtual void Update() = 0;
 
	/** \brief ��������������ָ����SourceTexture(�ڲ��ӿ�)
	*
	*	@remark 
	*	@param[in]	spTexture		�������
	*	@param[in]	ucSkipMipmapLvl	����������Mipmap
	*	@param[in]	bAsyn			�Ƿ�ͬ������
	*	@return						���ҵ����������(����Ѿ�ע��ͬ�ļ����Ķ����򷵻�false)
	*	@see	ITextureManager
	*	@note	�����ж���߳�ͬʱ����
	*/
	virtual bool LoadSourceTexture(sdSourceTexturePtr spTexture,
		uchar ucSkipMipmapLvl,
		bool bAsyn = true) = 0;

	virtual bool DecreaseSourceTexture(sdSourceTexturePtr spTexture,
		uchar ucSkipMipmapLvl,
		bool bAsyn = true) = 0;

	/** \brief �����������ָ����SourceTexture(�������򴴽�֮)
	*
	*	@remark 
	*	@param[in]	pcFileName		����·��
	*	@param[in]	kCreateParams	����������
	*	@param[out] bCreate			
	*	@return						���ҵ����������(����Ѿ�ע��ͬ�ļ����Ķ����򷵻�false)
	*	@see	ITextureManager
	*	@note	�����ж���߳�ͬʱ����,�ڲ�������ԭ�Ӳ���
	*/
	virtual sdSourceTexturePtr GetSourceTexture(const char* pcFileName, 
		const sdSourceTexture::CreateParams& kCreateParams,
		const bool& bCreate) = 0;

	/** \brief �������ע��SourceTexture
	*
	*	@remark
	*	@param[in] pkSourceTexture	�������
	*	@return						�Ƿ�ע��ɹ�(����Ѿ�ע��ͬ�ļ����Ķ����򷵻�false)
	*	@see	ITextureManager
	*	@note	
	*/
	virtual bool RegisterSourceTexture(sdSourceTexturePtr spSourceTexture) = 0;

	/** \brief ���������ע��SourceTexture
	*
	*	@remark
	*	@param[in] pkSourceTexture	�������	
	*	@return						�Ƿ�ע��ɹ�
	*	@see	ITextureManager
	*	@note	
	*/
	virtual bool UnregisterSourceTexture(sdSourceTexturePtr spSourceTexture) = 0;

	//
	//virtual void AddSynLoadTexture(NiSourceTexture* spTexture) = 0;

	// ����ж������,
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
	// ��ȡ�������ӿ�
	static ITextureManager* GetTextureManager(){ return InstancePtr();}
};
//---------------------------------------------------------------------------------------
// ITextureLodManager
//
// ��������LOD�����ݵ�ǰ�����ڴ��С����̬ȷ����������Lod�ȼ�����ʵ�ֶ������ڴ�Ŀ���
// Ŀǰ�����е�������õ��ǵ�һ��LODֵ�����������Ľ�Ϊ�����ڴ�����ȡ��ͬ�Ĵ�����ԺͿ��Ʋ���
//---------------------------------------------------------------------------------------
class ITextureLodManager : public Base::sdTSingleton<ITextureLodManager>
{
public:
	virtual ~ITextureLodManager(){}

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// ����
	virtual void Update() = 0;

	// 
	virtual uchar GetBaseSkipLevel() = 0;
	virtual void SetBaseSkipLevel(uchar uiLevel) = 0;

	virtual void SetEnableDynamicLevel(bool bEnable) = 0;
	virtual bool GetEnableDynamicLevel() = 0;

	// ��ȡ��ǰLOD�ȼ�
	virtual uchar GetSkipLevel() = 0;

public:
	// ��ȡ�������ӿ�
	static ITextureLodManager* GetTextureLodManager(){ return InstancePtr();}
};
//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif