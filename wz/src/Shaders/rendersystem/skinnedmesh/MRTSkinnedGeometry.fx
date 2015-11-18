//*************************************************************************************************
// 内容:	SkinnedMesh的MRTGeometryPass着色器(V2版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#define CHANNELS_FLAG	0x00000000
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000001
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000002
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000003
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000004
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000005
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000006
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000007
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000008
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000009
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000a
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000b
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000c
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000d
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000e
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000f
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000200
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000201
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000202
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000203
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000204
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000205
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000206
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000207
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000208
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000209
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020a
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020b
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020c
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020d
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020e
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020f
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001000
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001001
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001002
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001003
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001004
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001005
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001006
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001007
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001008
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001009
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100a
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100b
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100c
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100d
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100e
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000100f
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001200
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001201
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001202
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001203
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001204
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001205
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001206
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001207
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001208
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00001209
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120a
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120b
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120c
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120d
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120e
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000120f
#include "MRTSkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG