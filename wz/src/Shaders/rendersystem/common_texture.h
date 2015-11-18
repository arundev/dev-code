//*************************************************************************************************
// ����:	������ɫ���Ĺ�������/�궨��/����
//			�������������������
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:
//*************************************************************************************************
#ifndef _COMMON_TEXTURE_H__
#define _COMMON_TEXTURE_H__

//*****************************************************************************
#define SRGB_DEFINE		true
#define SRGB_ENABLE		SRGBTexture = SRGB_DEFINE
#define SRGB_DISABLE	SRGBTexture = false

//*****************************************************************************
// ���������Ԥ����
// ����GB��base����ͨ���
//*****************************************************************************
texture sdBaseMap <string NTM = "base";>;
sampler sdBaseSampler = sampler_state
{
	Texture		= (sdBaseMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_ENABLE;
};

texture sdNormalMap <string NTM = "normal";>;
sampler sdNormalSampler = sampler_state
{
	Texture		= (sdNormalMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

texture sdGlossMap <string NTM = "gloss";>;
sampler sdGlossSampler = sampler_state
{
	Texture		= (sdGlossMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

texture sdDetailMap <string NTM = "detail";>;
sampler sdDetailSampler = sampler_state
{
	Texture		= (sdDetailMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

texture sdDarkMap <string NTM = "dark";>;
sampler sdDarkSampler = sampler_state
{
	Texture		= (sdDarkMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

texture sdGlowMap <string NTM = "glow";>;
sampler sdGlowSampler = sampler_state
{
	Texture		= (sdGlowMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

// ���ò�Ʊ��ĳɳ���DetailNormalMap�����
texture sdDetailNormalMap <string NTM = "parallax";>;
sampler sdDetailNormalSampler = sampler_state
{
	Texture		= (sdDetailNormalMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= WRAP;
	ADDRESSV	= WRAP;
	SRGB_DISABLE;
};

// ò�ư������۸ĳ�EnvironmentMap��
texture sdEnvMap <string NTM = "bump";>;
sampler sdEnvSampler = sampler_state
{
	Texture		= (sdEnvMap);
	MINFILTER	= LINEAR;
	MIPFILTER	= LINEAR;
	MAGFILTER	= LINEAR;
	ADDRESSU	= CLAMP;
	ADDRESSV	= CLAMP;
	SRGB_ENABLE;
};

//*****************************************************************************
// �����������Ԥ�����(���ڼ򻯴���)
// ����GB��shader���
//*****************************************************************************
#define SD_SAMPLE(_idx, _sampler, _tex, _address, _filter, _srgb)	\
	texture _tex													\
	<																\
		string	NTM 		= "shader";								\
		int 	NTMIndex	= _idx;									\
	>;																\
																	\
	sampler _sampler = sampler_state								\
	{																\
		Texture		= (_tex);										\
		ADDRESSU	= _address;										\
		ADDRESSV	= _address;										\
		ADDRESSW	= _address;										\
		MAGFILTER	= _filter;										\
		MINFILTER	= _filter;										\
		MIPFILTER	= _filter;										\
		SRGBTEXTURE	= _srgb;										\
	};				
//-----------------------------------------------------------------------------
#define SD_CUBESAMPLE(_idx, _sampler, _tex, _address, _filter)		\
	texture _tex													\
	<																\
		string	NTM 		= "shader";								\
		int 	NTMIndex	= _idx;									\
	>;																\
																	\
	samplerCUBE _sampler = sampler_state							\
	{																\
		Texture		= (_tex);										\
		ADDRESSU	= _address;										\
		ADDRESSV	= _address;										\
		ADDRESSW	= _address;										\
		MAGFILTER	= _filter;										\
		MINFILTER	= _filter;										\
		MIPFILTER	= _filter;										\
	};			
//-----------------------------------------------------------------------------
#define SD_SAMPLE_EX(_idx, _sampler, _tex, _address, _magfilter, _minfilter, _mipfilter)	\
	texture _tex													\
	<																\
		string	NTM 		= "shader";								\
		int 	NTMIndex	= _idx;									\
	>;																\
																	\
	sampler _sampler = sampler_state								\
	{																\
		Texture		= (_tex);										\
		ADDRESSU	= _address;										\
		ADDRESSV	= _address;										\
		ADDRESSW	= _address;										\
		MAGFILTER	= _magfilter;									\
		MINFILTER	= _minfilter;									\
		MIPFILTER	= _mipfilter;									\
		SRGB_ENABLE;												\
	};	
//-----------------------------------------------------------------------------
#define SD_SAMPLE_EX2(_idx, _ntm, _sampler, _tex, _address, _filter, _srgb)	\
	texture _tex													\
	<																\
		string	NTM 		= _ntm;									\
		int 	NTMIndex	= _idx;									\
	>;																\
																	\
	sampler _sampler = sampler_state								\
	{																\
		Texture		= (_tex);										\
		ADDRESSU	= _address;										\
		ADDRESSV	= _address;										\
		ADDRESSW	= _address;										\
		MAGFILTER	= _filter;										\
		MINFILTER	= _filter;										\
		MIPFILTER	= _filter;										\
		SRGBTEXTURE	= _srgb;										\
	};	
	
//-----------------------------------------------------------------------------
#define SD_POINT_CLAMP_SAMPLE(_idx, _sampler, _tex, _srgb)		SD_SAMPLE(_idx, _sampler, _tex, CLAMP, POINT, _srgb)
#define SD_LINEAR_CLAMP_SAMPLE(_idx, _sampler, _tex, _srgb)		SD_SAMPLE(_idx, _sampler, _tex, CLAMP, LINEAR, _srgb)
#define SD_LINEAR_WRAP_SAMPLE(_idx, _sampler, _tex, _srgb)		SD_SAMPLE(_idx, _sampler, _tex, WRAP, LINEAR, _srgb)
#define SD_LINEAR_MIRROR_SAMPLE(_idx, _sampler, _tex, _srgb)	SD_SAMPLE(_idx, _sampler, _tex, MIRROR, LINEAR, _srgb)

#define SD_POINT_CLAMP_CUBESAMPLE(_idx, _sampler, _tex)			SD_CUBESAMPLE(_idx, _sampler, _tex, CLAMP, POINT)
#define SD_LINEAR_CLAMP_CUBESAMPLE(_idx, _sampler, _tex)		SD_CUBESAMPLE(_idx, _sampler, _tex, CLAMP, LINEAR)
//*****************************************************************************
#endif	