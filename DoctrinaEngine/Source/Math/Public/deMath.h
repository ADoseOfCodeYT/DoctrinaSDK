#ifndef _DEMATH_H_
#define _DEMATH_H_

namespace de::Math
{
    template <typename T>
    constexpr T Clamp(T x, T a, T b)
    {
    	return x < a ? a : (x > b ? b : x);
    }
}

#endif