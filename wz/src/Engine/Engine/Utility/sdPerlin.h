//*************************************************************************************************
// 内容:	Perlin噪声
//		- Perlin Noise
//		- Simplex Noise
//
// 生成步骤:
//		1.Integer-Noise函数
//		double IntegerNoise (int n)
//		{
//			n = (n >> 13) ^ n;
//			int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
//			return 1.0 - ((double)nn / 1073741824.0);
//		}
//		2.在Interger-Noise间进行插值(linear/bilinear/trilinear interpolation)
//		double CoherentNoise (double x)
//		{
//			int intX = (int)(floor(x));
//			double n0 = IntegerNoise (intX);
//			double n1 = IntegerNoise (intX + 1);
//			double weight = x - floor (x);
//			double noise = lerp(n0, n1, weight);
//			return noise;
//		}
//		3.在线性插值的基础上进一步改进(Hermit/Cubic interpolation)
//		double CoherentNoise (double x)
//		{
//			int intX = (int)(floor (x));
//			double n0 = IntegerNoise (intX);
//			double n1 = IntegerNoise (intX + 1);
//			double weight = x - floor (x);
//			//double noise = lerp (n0, n1, weight);
//			double noise = lerp (n0, n1, SCurve (weight));
//			return noise;
//		}
//
// 参考网址:
//		http://en.wikipedia.org/wiki/Perlin_noise
//		http://www.cnblogs.com/ArenAK/archive/2008/02/21/1076847.html
//		http://libnoise.sourceforge.net/index.html
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_PERLIN_H__
#define _SD_ENGINE_UTIL_PERLIN_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL

template<int prime0, int prime1, int prime2>
class sdPerlinNoise
{
public:
	sdPerlinNoise();
	~sdPerlinNoise();

	float Noise1D(int x)
	{
		x = (x << 13)^x;
		x = x * (x * x * prime0 + prime1) + prime2;
		return ((x & 0x7fffffff) / 1073741823.5f) - 1.f;
	}

	float Noise1D(int x, int offset)
	{
		return Noise1D(x + offset);
	}

	float Noise2D(int x, int y)
	{
		return Noise1D(x + y * 57);
	}

	float Noise2D(int x, int y, int offset)
	{
		return Noise1D(x + y * 57 + offset);
	}

	float Noise3D(int x, int y, int z)
	{
		return Noise1D(x + y * 57 + z * 149);
	}

	float Noise3D(int x, int y, int z, int offset)
	{
		return Noise1D(x + y * 57 + z * 149 + offset);
	}

	float SoftNoise1D(int x)
	{
		float a = Noise1D(x >> 16);
		float b = Noise1D((x >> 16) + 1);
		float t = (x & 0xffff) / 65536.f;

		return Base::Math::sdMath::Cos_Interpolate(a, b, t);
	}
};

// 缺省的噪声对象
typedef sdPerlinNoise<19379, 819233, 1266122899> sdDefaultPerlinNoise;

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif