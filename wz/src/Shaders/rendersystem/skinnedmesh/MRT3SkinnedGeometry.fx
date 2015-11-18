//*************************************************************************************************
// 内容:	StaticMesh的MRT3GeometryPass着色器(V3版)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#define CHANNELS_FLAG	0x00000000
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000001
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000002
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000003
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000004
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000005
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000006
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000007
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000008
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000009
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000000f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000010
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000011
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000012
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000013
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000014
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000015
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000016
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000017
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000018
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000019
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000001f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000200
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000201
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000202
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000203
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000204
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000205
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000206
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000207
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000208
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000209
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000020f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000210
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000211
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000212
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000213
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000214
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000215
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000216
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000217
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000218
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000219
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000021f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000400
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000401
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000402
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000403
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000404
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000405
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000406
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000407
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000408
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000409
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000040f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000410
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000411
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000412
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000413
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000414
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000415
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000416
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000417
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000418
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000419
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000041f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000600
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000601
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000602
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000603
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000604
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000605
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000606
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000607
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000608
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000609
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000060f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000610
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000611
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000612
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000613
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000614
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000615
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000616
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000617
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000618
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000619
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061a
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061b
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061c
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061d
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061e
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x0000061f
#include "MRT3SkinnedGeometryImplement.h"
#undef 	CHANNELS_FLAG