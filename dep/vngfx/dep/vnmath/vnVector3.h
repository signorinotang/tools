//
//  vnVector3.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnVector3_h
#define vnmath_vnVector3_h

#include "vnVector2.h"

_vn_begin

struct vector3 {
	real x;
	real y;
	real z;
	
	vector3() : x(0), y(0), z(0) {}
	vector3(real _x, real _y, real _z) : x(_x), y(_y), z(_z) {}
	
	template <typename T>
	vector3(const vector2<T> &v, real _z) : x((real)v.x), y((real)v.y), z(_z) {}
	
	// cast operators
	operator real *() {
		return (real *)this;
	}
	
	operator const real *() const {
		return (real *)this;
	}
	
	// unaray operators
	vector3 operator +() const {
		return *this;
	}
	
	vector3 operator -() const {
		return vector3(-x, -y, -z);
	}
	
	// binary operators
	vector3 operator +(const vector3 &rhs) const {
		return vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	vector3 operator -(const vector3 &rhs) const {
		return vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	vector3 operator *(const vector3 &rhs) const {
		return vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}
	
	vector3 operator /(const vector3 &rhs) const {
		return vector3(x / rhs.x, y / rhs.y, z / rhs.z);
	}
	
	vector3 operator *(real s) const {
		return vector3(x * s, y * s, z * s);
	}
	
	vector3 operator /(real s) const {
		return vector3(x / s, y / s, z / s);
	}
	
	bool operator ==(const vector3 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	
	bool operator !=(const vector3 &rhs) const {
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}
	
	bool operator >(const vector3 &rhs) const {
		return x > rhs.x && y > rhs.y && z > rhs.z;
	}
	
	bool operator >=(const vector3 &rhs) const {
		return x >= rhs.x && y >= rhs.y && z > rhs.z;
	}
	
	bool operator <(const vector3 &rhs) const {
		return x < rhs.x && y < rhs.y && z < rhs.z;
	}
	
	bool operator <=(const vector3 &rhs) const {
		return x <= rhs.x && y <= rhs.y && z <= rhs.z;
	}
	
	vector3 & operator +=(const vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	vector3 & operator -=(const vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	
	vector3 & operator *=(const vector3 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	
	vector3 & operator /=(const vector3 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	
	vector3 & operator *=(real s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	
	vector3 & operator /=(real d) {
		x /= d;
		y /= d;
		z /= d;
		return *this;
	}
	
	// functions
	bool equals(const vector3 &other, real tolerance = math::constant<real>::rounding_error) const {
		return math::equals(x, other.x, tolerance) && math::equals(y, other.y, tolerance) && math::equals(z, other.z, tolerance);
	}
	
	real volume() const {
		return x * y * z;
	}
	
	real length() const {
		return math::sqrt( x * x + y * y + z * z );
	}
	
	real length_square() const {
		return x * x + y * y + z * z;
	}
	
	real distance(const vector3 &other) const {
		return vector3(x - other.x, y - other.y, z - other.z).length();
	}
	
	real distance_square( const vector3 &other ) const {
		return vector3(x - other.x, y - other.y, z - other.z).length_square();
	}
	
	real dot_product( const vector3 &rhs ) const {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	
	vector3 cross_product(const vector3 &rhs) const {
		// right-handed coordinate system.
		return vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}
	
	vector3 & set(real _x, real _y, real _z) {
		x = _x; y = _y; z = _z;
		return *this;
	}
	
	vector3 & invert() {
		x = -x; y = -y; z = -z;
		return *this;
	}
	
	bool normalize(real &len, const real tolerance = math::constant<real>::rounding_error) {
		len = length();
		if (!math::is_zero(len, tolerance)) {
			*this /= len;
			return true;
		}
		return false;
	}
	
	real normalize(const real tolerance = math::constant<real>::rounding_error) {
		real len = length();
		if (!math::is_zero(len, tolerance)) {
			*this /= len;
		}
		return len;
	}
	
	vector3 & interpolate(const vector3 &a, const vector3 &b, real f) {
		x = a.x + (b.x - a.x) * f;
		y = a.y + (b.y - a.y) * f;
		z = a.z + (b.z - a.z) * f;
		return *this;
	}
	
	vector3 & interpolate(const vector3 &a, const vector3 &b, const vector3 &c, real f) {
		real tx = a.x + (b.x - a.x) * f;
		real ty = a.y + (b.y - a.y) * f;
		real tz = a.z + (b.z - a.z) * f;
		x = tx + (c.x - tx) * f;
		y = ty + (c.y - ty) * f;
		z = tz + (c.z - tz) * f;
		return *this;
	}
	
};

inline vector3 operator *( real s, const vector3 &rhs ) {
	return vector3( s * rhs.x, s * rhs.y, s * rhs.z );
}

inline vector3 operator /( real d, const vector3 &rhs ) {
	return vector3( d / rhs.x, d / rhs.y, d / rhs.z );
}

_vn_end

#endif
