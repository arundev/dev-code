#include "WorldEditor.FrameworkPCH.h"
#include "MWorldEditor.h"

//
#include "sdWorldEditor.h"

namespace WorldEditorFramework 
{
	//-----------------------------------------------------------------------------
	MWorldEditor::MWorldEditor()
	{

	}
	//-----------------------------------------------------------------------------
	MWorldEditor::~MWorldEditor()
	{

	}
	//-----------------------------------------------------------------------------
	bool MWorldEditor::Initialize(System::IntPtr hWndPtr)
	{
		m_pkWorldEditor = new sdWorldEditor;
		NIASSERT(m_pkWorldEditor);
		m_pkWorldEditor->Initialize((HWND)hWndPtr.ToPointer());

		return true;
	}
	//-----------------------------------------------------------------------------
	void MWorldEditor::Destroy()
	{
		
	}
	//-----------------------------------------------------------------------------
	void MWorldEditor::Update()
	{
		m_pkWorldEditor->Update();
	}
	//-----------------------------------------------------------------------------
	bool MWorldEditor::SaveScene()
	{
		return m_pkWorldEditor->SaveScene();
	}
	//-----------------------------------------------------------------------------
	bool MWorldEditor::OpenScene()
	{
		return m_pkWorldEditor->OpenScene();
	}
	//-----------------------------------------------------------------------------
	//void MWorldEditor::WndProc(System::Windows::Forms::Message% msg)
	//{
	//	m_pkWorldEditor->WndProc(msg.Msg, (HWND)(msg.HWnd.ToPointer()), msg.WParam.ToInt32(), msg.LParam.ToInt32());
	//}
	//-----------------------------------------------------------------------------
}