#include "sdBasePCH.h"
#include "sdVector4.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
const sdVector4 sdVector4::ZERO(0, 0, 0, 0);
const sdVector4 sdVector4::UNIT_X(1, 0, 0, 0);
const sdVector4 sdVector4::UNIT_Y(0, 1, 0, 0);
const sdVector4 sdVector4::UNIT_Z(0, 0, 1, 0);
const sdVector4 sdVector4::UNIT_W(0, 0, 0, 1);
const sdVector4 sdVector4::NEGATIVE_UNIT_X(-1,  0,  0,  0);
const sdVector4 sdVector4::NEGATIVE_UNIT_Y( 0, -1,  0,  0);
const sdVector4 sdVector4::NEGATIVE_UNIT_Z( 0,  0, -1,  0);
const sdVector4 sdVector4::NEGATIVE_UNIT_W( 0,  0,  0, -1);;
const sdVector4 sdVector4::UNIT_SCALE(1, 1, 1, 1);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE