//*************************************************************************************************
// ����:	�ļ�����
//---------------------------------------------------------
// ����:		
// ����:		2014-01-09
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SOURCE_TEXTURE_H__
#define _SD_ENGINE_SOURCE_TEXTURE_H__
#include "IDataStream.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdSourceTexture;
NiSmartPointer(sdSourceTexture);
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// sdSourceTexture
//
// ����Managed������ͼ�Ķ�̬�������
//	1.m_ucPriority
//	2.m_eResStatus
//	3.m_uiTimeStamp
//	4.m_uiFileSize/m_pucFileData
//		
// ��Ҫע�����:
//	1.����û�ж���Դ״̬�仯��ԭ�Ӳ���(����ԭ�Ӳ�����������)
//	2.����ȼ������첽���ض���,����ͬ�����ز���ɹ�
//	3.ȱ�ټ��ػص�����
//	4.������Դ������Mesh/Skeleton��һ����
//		���������Ǵ���̬�ģ�ֻ��Ҫ��һ�����ݼ���
//		Mesh/Skeleton�ᱻ�޸ģ�ÿ�����ݶ���Ҫһ�ݿ���(��Ȼ������ʹ��дʱ����)
//-------------------------------------------------------------------------------------------------
class sdSourceTexture : public NiSourceTexture
{
	NiDeclareRTTI;
	NiDeclareStream;
public:
	// ����״̬
	typedef enum ETEXSTATUS
	{
		E_TEX_INVALID,		///< ��ʼ״̬
		E_TEX_PARAM_INITED,	///< ������ʼ�����
		E_TEX_PIX_PRE_LOAD,	///< �ȴ��������ݵ��ڴ�
		E_TEX_PIX_LOADING,	///< ���ڼ����������ݵ��ڴ�
		E_TEX_PIX_LOADED,	///< �ɹ������������ݵ��ڴ�
		E_TEX_CREATED,		///< �豸�������ɹ�
		E_TEX_IGNORED,		///< ȡ��״̬(�����Ǵ����豸����ʧ��֮���)
		E_TEX_ERROR,		///< �������
	}ETexStatus;

	// Mipmap����ʽ
	typedef enum EMIPMAPTYPE
	{
		E_MIPMAP_DEFAULT,			///< Mipmapʹ��ȱʡ��ʽ������
		E_MIPMAP_AUTO,				///< Mipmap��ϵͳ�Զ�����
		E_MIPMAP_MANUAL,			///< Mipmap�ֶ�����
	}EMipmapType;

	// ����������
	class CreateParams : public NiMemObject
	{
	public:
		FormatPrefs m_kPrefs;					///< 
		bool m_bAsyn;							///< �Ƿ��첽����
		bool m_bLoadDirectToRendererHint;		///< ֱ�ӴӴ��̼�������ʱ,�Ƿ�ֱ�Ӵ���D3D����
		bool m_bSrcRendererDataIsPersistent;	///< �Ƿ��PersistentData�����������
		uchar m_ucPriority;						///< �������ȼ�
		EMIPMAPTYPE m_eMipmapType;				///< Mipmap����ʽ
		uchar m_ucSkipMipmapLvl;				///< Mipmap�����ĵȼ�(E_MIPMAP_MANUAL��ʽ����Ч)
		uchar m_ucMipmapLvl;					///< Mipmap�ȼ�(E_MIPMAP_MANUAL��ʽ����Ч)

	public:
		CreateParams() : m_bAsyn(true)
			,m_bLoadDirectToRendererHint(true)
			,m_bSrcRendererDataIsPersistent(false)
			,m_ucPriority(0)
			,m_eMipmapType(E_MIPMAP_AUTO)
			,m_ucSkipMipmapLvl(0)
			,m_ucMipmapLvl(0)
		{
	
		}
	};

public:
	/** \brief ����SourceTexture����(�ᱻ���̵߳���)
	*
	*	@remark
	*	@param[in] kFilename
	*	@param[in] kParams	
	*	@return	
	*	@see	sdSourceTexture
	*	@note	����ο���NiSourceTexture::Create()
	*			1.�Բ�ͬ�Ĳ�������ͬһ���������ص������ȴ���������
	*/
	static sdSourceTexturePtr Create(const NiFixedString& kFilename, CreateParams& kParams);

	/** \brief ���¼�������(ͬ������,�漰��Դ״̬��ȡ�����,Ŀǰ��֧�ּ������޸Ĳ���)
	*
	*	@remark
	*	@param[in]	ucMipmapSkipLvl	�����ĵ�mipmap�ȼ�
	*	@param[in]	ucMipmapLvl		�����mipmap�ȼ�
	*	@return	
	*	@see	sdSourceTexture
	*	@note	�����ڲ���ֹ����
	*/
	bool SyncReloadResource(uchar ucSkipMipmapLvl, uchar ucMipmapLvl);

	/** \brief ���¼�������(�첽����,�漰��Դ״̬��ȡ�����,Ŀǰ��֧�ּ������޸Ĳ���)
	*
	*	@remark
	*	@param[in]	ucMipmapSkipLvl	�����ĵ�mipmap�ȼ�
	*	@param[in]	ucMipmapLvl		�����mipmap�ȼ�
	*	@return	
	*	@see	sdSourceTexture
	*	@note	�����ڲ���ֹ����
	*/
	bool AsyncReloadResource(uchar ucSkipMipmapLvl, uchar ucMipmapLvl);

	/** \brief ���ػص�(�����̵߳���,���ڸ�����Դ״̬)
	*
	*	@remark
	*	@param[in]	eTexStatus	��ǰ��ɵ���Դ״̬
	*	@return					֪ͨ�Ƿ�ɹ�
	*	@see	sdSourceTexture
	*	@note	
	*/
	bool OnLoadingStatus(ETexStatus eTexStatus);

	//// �������Բ���
	//void SetSourcePixelData(NiPixelData* pkPixelData){ m_spSrcPixelData = pkPixelData;}

	// ������(����֧��ֱ�Ӵ�������������)
	void SetDataStream(FileSystem::IDataStream* pkDataStream){ m_spDataStream = pkDataStream;}
	FileSystem::IDataStream* GetDataStream() { return m_spDataStream;}

	// ���Բ���
	EMipmapType GetMipmapType() const { return m_eMipmapType;}
	uchar GetMipmapLvl() const { return m_ucMipmapLvl;}
	uchar GetSkipMipmapLevel() const { return m_ucSkipMipmapLvl;}
	uchar GetPriority()const  { return m_ucPriority;}

	ETexStatus GetTexStatus() const {return m_eTexStatus;	}
	void SetTexStatus(ETexStatus eTexStatus){ m_eTexStatus = eTexStatus;}

	///< ����������
	///< @{
	// ��ǰ�����Ƿ�λ�ڼ��ض���
	//uint TrySetLoadingLock(){return AtomicSetFlagBit(E_TEX_IN_LOADING_MASK_SHIFT);}
	//uint TryResetLoadingLock(){	return AtomicResetFlagBit(E_TEX_IN_LOADING_MASK_SHIFT);	}
	///< @}

protected:
	// ���
	//typedef enum ETEXSTATUSLOCK
	//{
	//	E_TEX_STATUS_UNLOCKED,
	//	E_TEX_STATUS_LOCKED,
	//}ETexStatusLock;

	enum
	{
		// ����λ�ڼ��ض��е������(����ԭ�ӻ���������������صĲ���)
		E_TEX_IN_LOADING_MASK		= 0x00010000,	

		E_TEX_IN_RELOADING_FUNC_MASK_SHIFT	= 0x1,	///< ÿ����������AsyncReloadResource������ֹ������
		E_TEX_IN_LOADING_MASK_SHIFT			= 0x16,	///< ����λ�ڼ��ض��е������λλ��
	};

protected:
	sdSourceTexture();
	virtual ~sdSourceTexture();

	//bool TryLockTexStatus()
	//{
	//	return (::InterlockedCompareExchange((volatile LONG*)(&m_uiTexStatusLock), E_TEX_STATUS_LOCKED,	E_TEX_STATUS_UNLOCKED) == E_TEX_STATUS_UNLOCKED);
	//}

	//void UnlockTexStatus()
	//{
	//	::InterlockedCompareExchange((volatile LONG*)(&m_uiTexStatusLock), E_TEX_STATUS_UNLOCKED, E_TEX_STATUS_LOCKED);
	//}

	// �����ñ�ǵ�ָ��λԭ�ӵ�����Ϊ1/0
	uint AtomicSetFlagBit(uint uiBitPos)
	{
		return ::InterlockedBitTestAndSet((LONG volatile *)m_uiFlags, uiBitPos);
	}

	uint AtomicResetFlagBit(uint uiBitPos)
	{
		return ::InterlockedBitTestAndReset((LONG volatile *)m_uiFlags, uiBitPos);
	}

	uint TestFlagBit(uint uiBitPos)
	{
		return (m_uiFlags & (1 << uiBitPos));
	}

	// ��ǰ
	uint TrySetAsyncReloadFuncLock(){return AtomicSetFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);}
	uint TryResetAsyncReloadFuncLock(){	return AtomicResetFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);	}
	uint TestAsyncReloadFuncLock(){ return TestFlagBit(E_TEX_IN_RELOADING_FUNC_MASK_SHIFT);}

protected:
	///< ��Դ״̬
	///<	1.���ʹ�����������Ļ�,��������״̬�ı�Ǩ
	///<	2.���϶�д������֤ԭ�Ӳ��������Ч��
	///< ����ʹ�õ�
	///< @{
	// ����״̬(ֻ�ܴӵ͵��߱�Ǩ��ֻ�����߳��ڴ���ʱ������ʱ�������̸߳Ķ�)
	ETexStatus m_eTexStatus;

	// ����������ȼ�(ֻ���ڴ���ʱ��ֵһ��)
	uchar m_ucPriority;			

	// ����Mipmap�Ĺ���ʽ(ֻ���ڴ���ʱ��ֵһ��)
	EMipmapType m_eMipmapType;

	// ����Mipmap���Ʋ���(��̬������ֻ�д���ʱ������ʱ�������̸߳Ķ�)
	uchar m_ucMipmapLvl;		///< ���ص�Mipmap�ȼ�(0 means whole size, 1 means one level)
	uchar m_ucSkipMipmapLvl;	///< ����Mipmap�����ĵȼ�

	// ״̬��
	//volatile uint m_uiTexStatusLock;

	// ���
	volatile uint m_uiFlags;
	///< @}

	// ����������,���������첽����
	Engine::FileSystem::IDataStreamPtr m_spDataStream;

	static EMIPMAPTYPE ms_eDefaultMipmapType;	///< Mipmap�Ĺ���ʽ
	static uchar ms_ucMinSkipMipmapLvl;			///< Mipmap����Ч��Χ
	static uchar ms_ucMaxSkipMipmapLvl;			///<
	static uchar ms_ucMinLoadPriority;			///< �������ȼ���Χ
	static uchar ms_ucMaxLoadPriority;			///<

	static Base::Threading::sdCriticalSection m_kCreateLock;	///< ��������(��֤��������ԭ����)
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif