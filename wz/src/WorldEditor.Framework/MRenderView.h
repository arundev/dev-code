//*************************************************************************************************
// ����:	�༭������
//---------------------------------------------------------
// ����:		
// ����:		2012-11-07
// ����޸�:
//*************************************************************************************************
#pragma once

namespace WorldEditorFramework 
{
	// ��Ⱦ��ͼ����
	public enum class MRenderViewType
	{
		E_BASE_VIEW = 0,
		E_SCENE_VIEW,           // ������ͼ
		E_PREVIEW_VIEW,         // Ԥ����ͼ
	};

	// ��Ⱦ��ͼ�İ����ӿ�,����ͨ����Ⱦ����Ⱦ����Ӧ�༭����һ���ӿ�(Form/Control)
	//  - ���Լ�������������Ϣ����
	//  - ���Լ��������Ϣ
	public ref class MRenderView
	{
	public:
		MRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// ������Ϣ����
		virtual void WndProc(System::Windows::Forms::Message% msg);

		// 
		//virtual void Update();
		//virtual void Render();

	protected:
		System::IntPtr	m_hWndPtr;
		MRenderViewType	m_eRenderViewType;	
	};
}