//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PIXEL_LOADER_TASK_QUEUE_H__
#define _SD_ENGINE_PIXEL_LOADER_TASK_QUEUE_H__

#include <sdCriticalSection.h>
#include "sdFixedStringExt.h"

#define MAX_TEXTURE_PRIORITY 5	///< (0~4)

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
NiSmartPointer(sdSourceTexture);
class sdTextureCache;
class sdPixelLoader;
NiSmartPointer(sdPixelLoader);
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//---------------------------------------------------------------------------------------
// sdPixelLoadTaskQueue,�����������
//
// ע�⣺
//	1.��֧��ȡ������
//	2.���������ض���֮��֧�ֲ����޸�
//---------------------------------------------------------------------------------------
class sdPixelLoadTaskQueue : public NiRefObject
{
public:
	// ��������
	class PixelLoadTask : public NiMemObject
	{
	public:
		char				m_cMipmap;		///< �����mipmap�ȼ�
		sdSourceTexturePtr	m_pkTexture;	///< �������
	};
	typedef std::list<PixelLoadTask> PixelLoadTaskList;
	typedef std::list<PixelLoadTask>::iterator PixelLoadTaskListItr;

	//
	class PixelLoadTaskIndicator : public NiMemObject
	{
	public:
		uint	m_uiPreloadCount;
		uint	m_uiLoadingCount;
	};

public:
	sdPixelLoadTaskQueue(sdTextureCache* pkTextureCache);
	~sdPixelLoadTaskQueue();

	/** \brief ��������������(���̵߳���)
	*
	*	@remark
	*	@param[in] spTexture		��������������
	*	@param[in] ucSkipMipmapLvl	Ŀ��Mipmap�ȼ�
	*	@return						����Ƿ�ɹ�
	*	@see	sdSourceTexture
	*	@note	
	*/
	bool AddLoadTask(sdSourceTexturePtr pkTexture, uchar ucSkipMipmapLvl);

	// @breif	�������������(���̵߳���)
	// @param[in] pkTexture			��������������
	// @param[in] ucSkipMipmapLvl	Ŀ��Mipmap�ȼ�
	// @return						����Ƿ�ɹ�
	bool AddDecreaseTask(sdSourceTexture* pkTexture, uchar ucSkipMipmapLvl);

	// @brief	��ȡ��ǰ����������(�����̵߳���)
	// @param[out]	pkTaskes
	// @param[in]	uiTaskWanted
	// @return		
	uint GetRecentPreloadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted);
	uint GetRecentLoadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted);

	// @brief ֪ͨ�����������(�����̵߳���)
	// @param[in]	pkTaskes
	// @param[in]	uiTaskNum
	// @return
	void NotifyCompleteTask(PixelLoadTask* pkTaskes, uint uiTaskNum);

protected:
	// ���ڴ�������D3D�豸����
	bool CreateD3DTexture(sdSourceTexture* pkSourceTexture);

protected:
	// �����ض���(�������ݻ��ڴ��̵ĵ��������)
	PixelLoadTaskList m_kPreloadListVec[MAX_TEXTURE_PRIORITY + 1];			
	PixelLoadTaskIndicator m_kLoadIndicatorList[MAX_TEXTURE_PRIORITY + 1];

	// �������豸�������(���������Ѿ������ص��ڴ浫�ǻ���Ҫ�����豸����)
	PixelLoadTaskList m_kLoadedList;

	// �����������ݿ�(��֤���������Ψһ��)
	typedef Engine::TFixedStringMap<sdSourceTexture*> SourceTextureMap;
	typedef Engine::TFixedStringMap<sdSourceTexture*>::iterator SourceTextureMapItr;
	SourceTextureMap m_kTaskDB;

	// ���������
	sdPixelLoaderPtr m_pkPixelLoader;

	// �߳���(��֤�Ա�����ʵ�Ψһ��)
	Base::Threading::sdCriticalSection m_kLock;
};

//---------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif