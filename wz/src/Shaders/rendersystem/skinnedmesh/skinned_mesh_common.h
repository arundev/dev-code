//*************************************************************************************************
// ����:	SkinnedMesh����ɫ�������еĹ��������뺯��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-21
// ����޸�:
//*************************************************************************************************
#ifndef _SKINNEDCMESH_COMMON_H__
#define _SKINNEDCMESH_COMMON_H__

//
#include "../common.h"

// �ӳ���ɫ�����������
SD_POINT_CLAMP_SAMPLE(0,	sdLightBufferSampler, 		sdTexLightBuffer, 	false);	
SD_POINT_CLAMP_SAMPLE(1, 	sdGeomBufferSampler, 		sdTexGeomBuffer, 	false);
SD_POINT_CLAMP_SAMPLE(2, 	sdMatBufferSampler, 		sdTexMatBuffer, 	false);
SD_LINEAR_CLAMP_SAMPLE(3, 	sdCurveFogBufferSampler, 	sdTexFogCurveMap, 	false);	

#endif