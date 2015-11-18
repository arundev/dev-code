//*************************************************************************************************
// 内容:	StaticMesh的MRT3ShadingPass着色器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-19
// 最后修改:	2013-05-27
//*************************************************************************************************
#define CHANNELS_FLAG	0x00000000
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000001
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000002
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000003
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000004
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000005
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000006
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000007
#include "MRT3ShadingImplement.h"
#undef 	CHANNELS_FLAG