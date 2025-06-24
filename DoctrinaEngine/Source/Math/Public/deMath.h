#ifndef _DEMATH_H_
#define _DEMATH_H_

#include <cmath>

#include "Math/Private/deMathAliases.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

namespace de::math
{
    template <typename T>
    constexpr T Clamp(T x, T a, T b)
    {
    	return x < a ? a : (x > b ? b : x);
    }                                                                               
    
}

#endif // ! _DEMATH_H_