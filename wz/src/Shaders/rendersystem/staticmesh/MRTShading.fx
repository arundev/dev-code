//*************************************************************************************************
// 内容:	StaticMesh的MRTShadingPass着色器(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-19
// 最后修改:	2013-05-28
//*************************************************************************************************
#define CHANNELS_FLAG	0x00000000
#include "MRTShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001000
#include "MRTShadingImplement.h"
#undef 	CHANNELS_FLAG