//*************************************************************************************************
// 内容:	渲染对象内存分配器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:	2014-05-07
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_ALLOC_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_ALLOC_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 渲染对象内存分配器(一个简单的内存分配器)
//  1.预先分配大块内存,在使用时直接在这些内存上来分配,这样可以减少内存碎片
//  2.仅仅供渲染器内部每帧帧内使用的对象
//	3.仅仅支持顺序分配和全部销毁,不支持特定对象的销毁
//  4.支持自动增加,这里增加的内存以链表形式链接起来
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
	uint m_uiCapacity;	///< Buffer大小,byte
	uint m_uiSize;		///< 已使用大小,byte
	uint m_uiNumObject;	///< 已分配的对象数目

	uint m_uiIncreaseStep;
	sdRenderObjectAlloc* m_pkIncreaseChild;
};
NiSmartPointer(sdRenderObjectAlloc);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif