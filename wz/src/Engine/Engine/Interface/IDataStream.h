//*************************************************************************************************
// ����:	�������ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2014-01-14
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILE_SYSTEM_DATA_STREAM_INTERFACE_H__
#define _SD_ENGINE_FILE_SYSTEM_DATA_STREAM_INTERFACE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// �������ӿ�
//-------------------------------------------------------------------------------------------------
class IDataStream : public Base::sdRefObject
{
public:
	virtual ~IDataStream(){};

	/** \brief ��ȡ����
	*
	*	@remark	��ȡ����
	*	@param[in,out]	pvBuff	��ȡ����
	*	@param[in]		iCount	��ȡ�ߴ�
	*	@return					�ɹ���ȡ�����ݳߴ�
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Read(void* pvBuff, size_t iCount) = 0;

	/** \brief ��ȡһ������
	*
	*	@remark	��ȡһ������
	*	@param[in,out]	pvBuff	��ȡ����
	*	@param[in]		iCount	��ȡ�ߴ�
	*	@param[in]		delim	�н�����
	*	@return	�ɹ���ȡ�����ݳߴ�
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t ReadLine(void* pvBuff, size_t iMaxCount, const std::string& delim = "\n") = 0;

	// @breif ��ȡһ������
	virtual std::string GetLine(bool bTrimAfter = true) = 0;

	// @breif ����ָ����������
	// @param[in]	lCount
	virtual void Skip(long lCount) = 0;

	// @breif ����һ������
	// @param[in]	delim
	// @return
	virtual size_t SkipLine(const std::string& delim = "\n") = 0;

	/** \brief Ѱַ��ָ��λ��
	*
	*	@remark	Ѱַ��ָ��λ��
	*	@param[in] iOffset	Ѱַ����
	*	@param[in] iOrigin	Ѱַ��ʼλ��
	*	@return	�����Ƿ�ɹ�
	*	@see	IDataStream
	*	@note
	*/
	virtual bool Seek(int iOffset, int iOrigin) = 0;

	/** \brief ��ȡ��ǰλ��
	*
	*	@remark	��ȡ��ǰλ��
	*	@return	��ǰλ��
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Tell() const = 0;

	/** \brief �ж��Ƿ񵽴���������β
	*
	*	@remark	�ж��Ƿ񵽴���������β
	*	@return	�Ƿ񵽴���������β
	*	@see	IDataStream
	*	@note
	*/
	virtual bool Eof() const = 0;

	/** \brief ��ȡ�������ߴ�
	*
	*	@remark	��ȡ�������ߴ�
	*	@return	�������ߴ�
	*	@see	IDataStream
	*	@note
	*/
	virtual size_t Size() const = 0;

	/** \brief ��ȡ����
	*
	*	@remark	��ȡ����
	*	@return	ָ�����ݻ����ָ��
	*	@see	IDataStream
	*	@note
	*/
	virtual const char* GetBuff() const = 0;

	/** \brief �ر�������
	*
	*	@remark	�ر�������
	*	@see	IDataStream
	*	@note
	*/
	virtual void Close() = 0;
};
sdSmartPointer(IDataStream);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif