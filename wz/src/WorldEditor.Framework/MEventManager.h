//*************************************************************************************************
// ����:	�¼�����,��Ҫ����C#/CLR֮��ͨ��
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
//*************************************************************************************************
#pragma once

namespace WorldEditorFramework 
{
	public ref class MEventManager
	{
	public:
		//__event void AppOpening();
		//__event void AppClosing();

		//__event void WorldOpening();
		//__event void WorldClosing();

		//__event void SceneNew();
		//__event void SceneOpening();
		//__event void SceneSaving();
		//__event void SceneClosing();

		//__event void ModuleChanged();
		//__event void EditModeChanged();

		//__event void DoCommand(ICommand* pkCommand);
		//__event void UndoCommand(ICommand* pkCommand);

		//__event void DocumentNew();
		//__event void DocumentOpen();
		//__event void DocumentClose();
		//__event void DocumentSave();
		//__event void DocumentRename();
		//__event void DocumentModified();

		// ����༭�¼�
		//__event void EntityCreate();
		//__event void EntityAddToScene();
		//__event void EntityRemoveFromScene();
		//__event void SelectEntity();
		//__event void DeselectEntity();
		//__event void SelectedEntitiesChanged(Object* kObjectArr[]);

		//void RaiseAppOpening();
		//void RaiseAppClosing();

		//void RaiseWorldOpening();
		//void RaiseWorldClosing();

		//void RaisSceneNew();
		//void RaisSceneOpening();
		//void RaisSceneSaving();
		//void RaisSceneClosing();
	};
}