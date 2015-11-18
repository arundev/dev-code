#include "sdBasePCH.h"
#include "sdVector3.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
const sdVector3 sdVector3::ZERO(0, 0, 0);
const sdVector3 sdVector3::UNIT_X(1, 0, 0);
const sdVector3 sdVector3::UNIT_Y(0, 1, 0);
const sdVector3 sdVector3::UNIT_Z(0, 0, 1);
const sdVector3 sdVector3::NEGATIVE_UNIT_X(-1,  0,  0);
const sdVector3 sdVector3::NEGATIVE_UNIT_Y( 0, -1,  0);
const sdVector3 sdVector3::NEGATIVE_UNIT_Z( 0,  0, -1);
const sdVector3 sdVector3::UNIT_SCALE(1, 1, 1);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE