//*************************************************************************************************
// ����:	������ذ����࣬���ڱ༭���м�����ʾ����
//---------------------------------------------------------
// ����:		
// ����:		2013-05-08
// ����޸�:
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
public ref class MTextureHelper
{
public:
	// ��ȡΪImage��ʽ
	//static System::Drawing::Image^	LoadImageFromFile(System::String^ strFile);
	//static Image*	LoadImageFromMemory(System::Byte pkData[], uint uiLen);

	// ��ȡΪHBITMAP��ʽ
	static System::IntPtr	LoadImageHBMP(System::String^ strFile);
	static void				ReleaseImageHBMP(System::IntPtr pkImage);
};
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK