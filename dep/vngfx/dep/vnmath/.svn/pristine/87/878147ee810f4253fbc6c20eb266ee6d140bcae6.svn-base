//
//  vnVector4.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnVector4_h
#define vnmath_vnVector4_h

#include "vnVector3.h"

_vn_begin

struct vector4 {
	real x;
	real y;
	real z;
	real w;
	
	vector4() : x(0), y(0), z(0), w(1) {}
	vector4(real _x, real _y, real _z, real _w) : x(_x), y(_y), z(_z), w(_w) {}
	vector4(const vector3 &v, real _w) : x(v.x), y(v.y), z(v.z), w(_w) {}
	vector4(const vector4 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
	
	// unaray operators
	vector4 operator +() const {
		return *this;
	}
	
	vector4 operator -() const {
		return vector4( -x, -y, -z, -w );
	}
	
	// binary operators
	vector4 operator +(const vector4 &rhs) const {
		return vector4( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
	}
	
	vector4 operator -(const vector4 &rhs) const {
		return vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}
	
	vector4 operator *( const vector4 &rhs ) const {
		return vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}
	
	vector4 operator /(const vector4 &rhs) const {
		return vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}
	
	vector4 operator *(real s) const {
		return vector4(x * s, y * s, z * s, w * s);
	}
	
	vector4 operator /(real d) const {
		return vector4(x / d, y / d, z / d, w / d);
	}
	
	bool operator ==(const vector4 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}
	
	bool operator !=(const vector4 &rhs) const {
		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	}
	
	vector4 & operator +=(const vector4 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	
	vector4 & operator -=(const vector4 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	
	vector4 & operator *=(const vector4 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	
	vector4 & operator /=(const vector4 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}
	
	vector4 & operator *=(real s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	
	vector4 & operator /=(real d) {
		x /= d;
		y /= d;
		z /= d;
		w /= d;
		return *this;
	}
	
	// functions
	bool equals(const vector4 &other, const real tolerance = math::constant<real>::rounding_error) const {
		return math::equals(x, other.x, tolerance) && math::equals(y, other.y, tolerance) && math::equals(z, other.z, tolerance) && math::equals(w, other.w, tolerance);
	}
	
	real length() const {
		return math::sqrt(x * x + y * y + z * z + w * w);
	}
	
	real length_square() const {
		return x * x + y * y + z * z + w * w;
	}
	
	real distance(const vector4 &other) const {
		return vector4( x - other.x, y - other.y, z - other.z, w - other.w).length();
	}
	
	real distance_square(const vector4 &other) const {
		return vector4( x - other.x, y - other.y, z - other.z, w - other.w ).length_square();
	}
	
	real dot_product(const vector4 &rhs) const {
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}
	
	real dot_product_vector(const vector3 &v) const {
		return x * v.x + y * v.y + z * v.z + w;
	}
	
	real dot_product_normal(const vector3 &v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	
	vector3 cast_vector3(const real tolerance = math::constant<real>::rounding_error) const {
		if (math::is_zero(w, tolerance)) {
			return vector3();
		}
		return vector3(x / w, y / w, z / w);
	}
	
	vector3 cast_normal3() const {
		return vector3(x, y, z);
	}
	
	vector4 & set(real _x, real _y, real _z, real _w) {
		x = _x; y = _y; z = _z; w = _w;
		return *this;
	}
	
	vector4 & invert() {
		x = -x; y = -y; z = -z; w = -w;
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
	
	vector4 & interpolate( const vector4 &a, const vector4 &b, real f ) {
		x = a.x + (b.x - a.x) * f;
		y = a.y + (b.y - a.y) * f;
		z = a.z + (b.z - a.z) * f;
		w = a.w + (b.w - a.w) * f;
		return *this;
	}
	
};

inline vector4 operator *(real s, const vector4 &rhs) {
	return vector4( s * rhs.x, s * rhs.y, s * rhs.z, s * rhs.w);
}

inline vector4 operator /(real d, const vector4 &rhs) {
	return vector4( d / rhs.x, d / rhs.y, d / rhs.z, d / rhs.w);
}

_vn_end


#endif
