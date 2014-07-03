//
//  vnMath.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnMath_h
#define vnmath_vnMath_h

#include "vnMathDef.h"
#include <cmath>
#include <cstring>

_vn_begin

namespace math {

	template <typename T>
	struct constant;
	
	template <>
	struct constant<int> {
		static const int rounding_error = 0;
	};
	
#if VN_COMPILER == VN_COMPILER_MSVC
    template <>
    struct constant<float> {
        static const float rounding_error;
		static const float pi;
        static const float half_pi;
        static const float two_pi;
    };
    
    template <>
	struct constant<double> {
		static const double rounding_error;
        static const double pi;
        static const double half_pi;
        static const double two_pi;
	};
    
#else
	template <>
	struct constant<float> {
		constexpr static const float rounding_error = 0.000001f;
		constexpr static const float pi =       3.1415926535897932f;
        constexpr static const float half_pi =  1.5707963267948965f;
        constexpr static const float two_pi =   6.2831853071795862f;
	};
	
	template <>
	struct constant<double> {
		constexpr static const double rounding_error = 0.000000000000001;
        constexpr static const double pi =       3.1415926535897932;
        constexpr static const double half_pi =  1.5707963267948965;
        constexpr static const double two_pi =   6.2831853071795862;
	};
#endif
	
	inline float reciprocal(float x) {
		return 1.f / x;
	}
	
	inline double reciprocal(double x) {
		return 1.0 / x;
	}
	
	inline float sqrt(float x) {
		return ::sqrtf(x);
	}
	
	inline double sqrt(double x) {
		return ::sqrt(x);
	}
	
	inline float reciprocal_sqrt(float x) {
		return 1.f / ::sqrtf(x);
	}
	
	inline double reciprocal_sqrt(double x) {
		return 1.0 / ::sqrt(x);
	}
	
	inline int abs(int x) {
		return (x >= 0 ? x : -x);
	}
	
	inline float abs(float x) {
		return ::fabsf(x);
	}
	
	inline double abs(double x) {
		return ::fabs(x);
	}
	
	template <typename T>
	bool equals(const T a, const T b, const T tolerance = constant<T>::rounding_error) {
		return abs(a - b) <= tolerance;
	}
	
	template <typename T>
	bool is_zero(const T a, const T tolerance = constant<T>::rounding_error) {
		return abs(a) <= tolerance;
	}
	
	template <typename T>
	T min_(T a, T b) {
		return a < b ? a : b;
	}
	
	template <typename T>
    T max_(T a, T b) {
		return a > b ? a : b;
	}
    
	template <typename T>
    T clamp(T value, T _min, T _max) {
		return max_(_min, min_(_max, value));
	}
    
    template <typename T>
	T deg2rad(T x) {
		return x * constant<T>::pi / 180;
	}
	
	template <typename T>
	T rad2deg(T x) {
		return x * 180 / constant<T>::pi;
	}
    
    template <typename T>
    void swap(T &a, T &b) {
        T t = a; a = b; b = t;
    }
	
	inline void sincos(float x, float &s, float &c) {
		s = ::sinf(x);
		c = ::cosf(x);
	}
	
	inline void sincos(double x, double &s, double &c) {
		s = ::sin(x);
		c = ::cos(x);
	}
	
	inline float sin(float x) {
		return ::sinf(x);
	}
	
	inline double sin(double x) {
		return ::sin(x);
	}
	
	inline float asin_safe(float x) {
		return ::asinf(x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
	}
	
	inline double asin_safe(double x) {
		return ::asin(x < -1.0 ? -1.0 : (x > 1.0 ? 1.0 : x));
	}
	
	inline float acos(float x) {
		return ::acosf(x);
	}
	
	inline double acos(double x) {
		return ::acos(x);
	}
	
	inline float atan2(float y, float x) {
		return ::atan2f(y, x);
	}
	
	inline double atan2(double y, double x) {
		return ::atan2(y, x);
	}
};

_vn_end

#endif
