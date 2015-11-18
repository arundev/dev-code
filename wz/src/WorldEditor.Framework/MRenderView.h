//*************************************************************************************************
// 内容:	编辑帮助类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-07
// 最后修改:
//*************************************************************************************************
#pragma once

namespace WorldEditorFramework 
{
	// 渲染视图类型
	public enum class MRenderViewType
	{
		E_BASE_VIEW = 0,
		E_SCENE_VIEW,           // 场景视图
		E_PREVIEW_VIEW,         // 预览视图
	};

	// 渲染视图的帮助接口,处理通过渲染器渲染并对应编辑器的一个视口(Form/Control)
	//  - 有自己独立的输入消息处理
	//  - 有自己的相机信息
	public ref class MRenderView
	{
	public:
		MRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// 输入消息处理
		virtual void WndProc(System::Windows::Forms::Message% msg);

		// 
		//virtual void Update();
		//virtual void Render();

	protected:
		System::IntPtr	m_hWndPtr;
		MRenderViewType	m_eRenderViewType;	
	};
}