//*************************************************************************************************
// 内容:	纹理加载帮助类，用于编辑器中加载显示纹理
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-08
// 最后修改:
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
public ref class MTextureHelper
{
public:
	// 读取为Image格式
	//static System::Drawing::Image^	LoadImageFromFile(System::String^ strFile);
	//static Image*	LoadImageFromMemory(System::Byte pkData[], uint uiLen);

	// 读取为HBITMAP格式
	static System::IntPtr	LoadImageHBMP(System::String^ strFile);
	static void				ReleaseImageHBMP(System::IntPtr pkImage);
};
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK