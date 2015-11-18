#include "WorldEditor.FrameworkPCH.h"
#include "MWorldEditorFSM.h"
#include "sdWorldEditor.h"

//
#include <sdEditMode.h>
#include <sdWorldEditFSM.h>

//
using namespace GameEditEx;

namespace WorldEditorFramework 
{
	//-----------------------------------------------------------------------------
	MWorldEditorFSM::MWorldEditorFSM()
	{
		m_pkWorldEditFSM = sdWorldEditor::Instance().GetWorldEditFSM();
	}
	//-----------------------------------------------------------------------------
	void MWorldEditorFSM::SetEditState(int iState)
	{
		switch(iState)
		{
		case 6:
			m_pkWorldEditFSM->SetState(sdEditMode::E_EDITMODE_TERRAIN_DEFORM);
			break;
		case 7:
			m_pkWorldEditFSM->SetState(sdEditMode::E_EDITMODE_TERRAIN_SURFACE);
			break;
		}
	}
	//-----------------------------------------------------------------------------
}