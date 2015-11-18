//*************************************************************************************************
// ����:	��Ⱦ�����ڴ������
//---------------------------------------------------------
// ����:		
// ����:		2012-06-30
// ����޸�:	2014-05-07
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_ALLOC_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_ALLOC_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ��Ⱦ�����ڴ������(һ���򵥵��ڴ������)
//  1.Ԥ�ȷ������ڴ�,��ʹ��ʱֱ������Щ�ڴ���������,�������Լ����ڴ���Ƭ
//  2.��������Ⱦ���ڲ�ÿ֡֡��ʹ�õĶ���
//	3.����֧��˳������ȫ������,��֧���ض����������
//  4.֧���Զ�����,�������ӵ��ڴ���������ʽ��������
//-----------------------------------------------------------------------------
// Only used for render objects memory space allocating.
// Only support allocated render object memory in sequence, deallocate all.
// Don't support deallocate memory in specified address.
// Support auto memory increasing.
// All memory size is byte size
//-------------------------------------------------------------------------------------------------
class sdRenderObjectAlloc : public NiRefObject
{
public:
	sdRenderObjectAlloc(uint uiCapicaty, uint uiIncreaseStep);
	~sdRenderObjectAlloc();

	void*	Alloc(uint uiSize);
	void	DeallocAll();

	uint Capacity() const;
	uint Size() const;
	uint NumObjects() const;

protected:
	char*	m_pcBuffer;
	uint m_uiCapacity;	///< Buffer��С,byte
	uint m_uiSize;		///< ��ʹ�ô�С,byte
	uint m_uiNumObject;	///< �ѷ���Ķ�����Ŀ

	uint m_uiIncreaseStep;
	sdRenderObjectAlloc* m_pkIncreaseChild;
};
NiSmartPointer(sdRenderObjectAlloc);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif