#include "WorldEditor.FrameworkPCH.h"
#include "MTextureHelper.h"
#include "../MMacros.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
//Image^ MTextureHelper::LoadImageFromFile(System::String^ strFile)
//{
//	Image^ pkImage = nullptr;
//
//	// 尝试使用
//	try
//	{
//		pkImage = Image::FromFile(strFile);	
//	}
//	catch (System::Exception^)
//	{
//	}
//
//	if (pkImage != nullptr)
//		return pkImage;
//
//	
//
//	return pkImage;
//}
//-------------------------------------------------------------------------------------------------
IntPtr MTextureHelper::LoadImageHBMP(String^ strFile)
{
	NiImageReader* spImageReader = NULL;
	if (strFile->EndsWith(".tga", StringComparison::InvariantCultureIgnoreCase))
	{
		spImageReader = NiNew NiTGAReader;
		NIASSERT(spImageReader);
	}
	else if (strFile->EndsWith(".dds", StringComparison::InvariantCultureIgnoreCase))
	{
		spImageReader = NiNew NiDDSReader;
		NIASSERT(spImageReader);
	}
	else if (strFile->EndsWith(".png", StringComparison::InvariantCultureIgnoreCase))
	{
		spImageReader = NiNew NiPNGReader;
		NIASSERT(spImageReader);
	}
	else if (strFile->EndsWith(".bmp", StringComparison::InvariantCultureIgnoreCase))
	{
		spImageReader = NiNew NiBMPReader;
		NIASSERT(spImageReader);
	}
	else
	{
		return IntPtr::Zero;
	}

	// 打开文件
	const char* szPath = MStringToCharPointer(strFile);
	NiFile* spFile = NiFile::GetFile(szPath, NiFile::READ_ONLY);
	MFreeCharPointer(szPath);
	if (!spFile)
	{
		NiDelete spImageReader;
		return IntPtr::Zero;
	}

	// 读取数据并关闭文件
	NiPixelDataPtr spPixelOptData = 0;
	NiPixelDataPtr spPixelData = spImageReader->ReadFile(*spFile, spPixelOptData);
	if (!spPixelData)
	{
		NiDelete spFile;
		NiDelete spImageReader;
		return IntPtr::Zero;
	}

	NiDelete spFile;
	spFile = NULL;

	NiDelete spImageReader;
	spImageReader = NULL;

	// 将数据转换为BMP格式
	NiPixelFormat kDestFmt(NiPixelFormat::FORMAT_RGBA, 
		NiPixelFormat::COMP_BLUE, NiPixelFormat::REP_NORM_INT, 8,
		NiPixelFormat::COMP_GREEN, NiPixelFormat::REP_NORM_INT, 8,
		NiPixelFormat::COMP_RED, NiPixelFormat::REP_NORM_INT, 8,
		NiPixelFormat::COMP_EMPTY, NiPixelFormat::REP_NORM_INT, 8);

	NiDevImageConverter kImageConverter;
	NiPixelDataPtr spPixelConvertedData = kImageConverter.ConvertPixelData(*spPixelData, kDestFmt, spPixelOptData, false);
	NIASSERT(spPixelConvertedData);

	HBITMAP hBitmap = ::CreateBitmap(
		spPixelConvertedData->GetWidth(),
		spPixelConvertedData->GetHeight(),
		1,
		32,
		spPixelConvertedData->GetPixels());
	NIASSERT(hBitmap);

	return IntPtr(hBitmap);
}
//-------------------------------------------------------------------------------------------------
void MTextureHelper::ReleaseImageHBMP(IntPtr pkImage)
{
	HBITMAP hBitmap = (HBITMAP)pkImage.ToPointer();
	::DeleteObject(hBitmap);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK