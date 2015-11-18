#include "sdBasePCH.h"
#include "sdMath.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
const float sdMath::POS_INFINITY	= std::numeric_limits<float>::infinity();
const float sdMath::NEG_INFINITY	= -std::numeric_limits<float>::infinity();
const float sdMath::MATH_EPS		= 10e-6f;
const float sdMath::MATH_BIG_EPS	= 10e-2f;
const float sdMath::MATH_SMALL_EPS	= 10e-6f;
const float sdMath::PI				= float(4.0f * atan(1.0f));
const float sdMath::TWO_PI			= float(2.0f * PI);
const float sdMath::HALF_PI			= float(0.5f * PI);
const float sdMath::DEG2RAD			= PI / float(180.0f);
const float sdMath::RAD2DEG			= float(180.0f) /PI;
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE