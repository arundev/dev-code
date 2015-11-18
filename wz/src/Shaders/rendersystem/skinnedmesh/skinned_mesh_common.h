//*************************************************************************************************
// 内容:	SkinnedMesh的着色器的特有的公共变量与函数
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#ifndef _SKINNEDCMESH_COMMON_H__
#define _SKINNEDCMESH_COMMON_H__

//
#include "../common.h"

// 延迟着色的纹理采样器
SD_POINT_CLAMP_SAMPLE(0,	sdLightBufferSampler, 		sdTexLightBuffer, 	false);	
SD_POINT_CLAMP_SAMPLE(1, 	sdGeomBufferSampler, 		sdTexGeomBuffer, 	false);
SD_POINT_CLAMP_SAMPLE(2, 	sdMatBufferSampler, 		sdTexMatBuffer, 	false);
SD_LINEAR_CLAMP_SAMPLE(3, 	sdCurveFogBufferSampler, 	sdTexFogCurveMap, 	false);	

#endif