//
//  vnQuaternion.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnQuaternion_h
#define vnmath_vnQuaternion_h

#include "vnVector3.h"

_vn_begin

struct quaternion {
	real w;
	real x;
	real y;
	real z;
	
	quaternion() : w(1), x(0), y(0), z(0) {}
	quaternion(real _w, real _x, real _y, real _z) : w(_w), x(_x), y(_y), z(_z) {}
	
	// unaray operators
	quaternion operator +() const {
		return *this;
	}
	
	quaternion operator -() const {
		return quaternion(-w, -x, -y, -z);
	}
	
	// binary operators
	bool operator ==(const quaternion &rhs) const {
		return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
	}
	
	bool operator !=(const quaternion &rhs) const {
		return w != rhs.w || x != rhs.x || y != rhs.y || z != rhs.z;
	}
	
	quaternion operator +(const quaternion &rhs) const {
		return quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	quaternion operator -(const quaternion &rhs) const {
		return quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	quaternion operator *(const quaternion &rhs) const {
		return quaternion(rhs.w * w - rhs.x * x - rhs.y * y - rhs.z * z, rhs.w * x + rhs.x * w + rhs.y * z - rhs.z * y, rhs.w * y + rhs.y * w + rhs.z * x - rhs.x * z, rhs.w * z + rhs.z * w + rhs.x * y - rhs.y * x);
	}
	
	quaternion operator *(real f) const {
		return quaternion(w * f, x * f, y * f, z * f);
	}
	
	vector3 operator *(const vector3 &v) const {
		vector3 uv, uuv;
		vector3 qvec( x, y, z );
		uv = qvec.cross_product( v );
		uuv = qvec.cross_product( uv );
		uv *= w * (real)2;
		uuv *= (real)2;
		return v + uv + uuv;
	}
	
	
	// functions
	bool equals(const quaternion &rhs, const real tolerance = math::constant<real>::rounding_error) const {
		return math::equals(w, rhs.w, tolerance) && math::equals(x, rhs.x, tolerance) && math::equals(y, rhs.y, tolerance) && math::equals(z, rhs.z, tolerance);
	}
	
	void to_angle_axis(real &angle, vector3 &axis) const {
		real t = x * x + y * y + z * z;
		if (t > 0) {
			angle = math::acos(w) * 2;
			t = math::reciprocal_sqrt( t );
			axis.x = x * t;
			axis.y = y * t;
			axis.z = z * t;
		} else {
			angle = 0;
			axis.x = 0;
			axis.y = 1;
			axis.z = 0;
		}
	}
	
	void to_euler_angles(vector3 &euler) const {
		const real sqw = w*w;
		const real sqx = x*x;
		const real sqy = y*y;
		const real sqz = z*z;
		
		// heading = rotation about z-axis
		euler.z = math::atan2((real)2 * (x*y + z*w), sqx - sqy - sqz + sqw);
		
		// bank = rotation about x-axis
		euler.x = math::atan2((real)2 * (y*z + x*w), -sqx - sqy + sqz + sqw);
		
		// attitude = rotation about y-axis
		euler.y = math::asin_safe((real)-2 * (x*y - y*w));
	}
	
	real dot_product(const quaternion &rhs) const {
		return w * rhs.w + x * rhs.x + y * rhs.y + z * rhs.z;
	}
	
	quaternion & operator =(const quaternion &rhs) {
		w = rhs.w; x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}
	
	quaternion & make_identity() {
		w = 1; x = 0; y = 0; z = 0;
		return *this;
	}
	
	quaternion & invert() {
		// must be unit(normalized).
		x = -x; y = -y; z = -z;
		return *this;
	}
	
	quaternion & normalize() {
		real n = w * w + x * x + y * y + z * z;
		if ( n != 1 ) {
			n = math::reciprocal_sqrt( n );
			w *= n;
			x *= n;
			y *= n;
			z *= n;
		}
		return *this;
	}
	
	quaternion & set( real _w, real _x, real _y, real _z ) {
		w = _w; x = _x; y = _y; z = _z;
		return *this;
	}
	
	
	// set rotation from angle-axis.
	// ! axis must be unit length.
	quaternion & set( real angle, const vector3 &axis ) {
		const real half_angle = angle * (real)0.5;
		real s;
		math::sincos(half_angle, s, w);
		x = s * axis.x;
		y = s * axis.y;
		z = s * axis.z;
		return *this;
	}
	
	// set rotation from 'from' to 'to'.
	quaternion & set( const vector3 &from, const vector3 &to ) {
		vector3 v0 = from;
		vector3 v1 = to;
		v0.normalize();
		v1.normalize();
		
		const real d = v0.dot_product( v1 );
		if (math::equals<real>(d, 1) ) {
			return make_identity();
		}
		if (math::equals<real>(d, -1)) {
			vector3 axis(1, 0, 0);
			axis = axis.cross_product( v0 );
			if ( math::is_zero( axis.length_square() ) ) {
				axis.set( 0, 1, 0 );
				axis = axis.cross_product( v0 );
			}
			axis.normalize();
			return set(math::constant<real>::pi, axis);
		}
		
		const real s = math::sqrt((1 + d) * 2);
		const real invs = math::reciprocal(s);
		const vector3 c = v0.cross_product(v1) * invs;
		w = s * 0.5f;
		x = c.x;
		y = c.y;
		z = c.z;
		return *this;
	}
	
	// set rotation from euler angles.
	quaternion & set( const vector3 &euler ) {
		real angle;
		
		angle = euler.x * 0.5f;
		real sr, cr;
		math::sincos(angle, sr, cr);
		
		angle = euler.y * 0.5f;
		real sp, cp;
		math::sincos(angle, sp, cp);
		
		angle = euler.z * 0.5f;
		real sy, cy;
		math::sincos(angle, sy, cy);
		
		const real cpcy = cp * cy;
		const real spcy = sp * cy;
		const real cpsy = cp * sy;
		const real spsy = sp * sy;
		
		x = sr * cpcy - cr * spsy;
		y = cr * spcy + sr * cpsy;
		z = cr * cpsy - sr * spcy;
		w = cr * cpcy + sr * spsy;
		return *this;
	}
	
	quaternion & slerp( const quaternion &src, const quaternion &dst, real f, bool shortest = true ) {
		const real epsilon = (real)1e-03;
		real cos_value = src.dot_product( dst );
		quaternion tmp = dst;
		
		if (cos_value < 0 && shortest) {
			cos_value = -cos_value;
			tmp.invert();
		}
		
		if (math::abs(cos_value ) < 1 - epsilon) {
			real sin_value = math::sqrt( 1 - cos_value * cos_value );
			real angle = math::atan2( sin_value, cos_value );
			real inv_sin_value = math::reciprocal(sin_value);
			real f0 = math::sin((1 - f) * angle ) * inv_sin_value;
			real f1 = math::sin(f * angle) * inv_sin_value;
			*this = src * f0 + tmp * f1;
		} else {
			// here are two situations:
			// 1. "src" and "dst" are very close ( cosValue ~= 1 ).
			// 2. "src" and "dst" are almost inverse of each other ( cosValue ~= -1 ).
			// i wanna ignore the 2nd situation. :-(
			// so, just make linear interpolation.
			*this = src * ((real)1 - f) + tmp * f;
			normalize();
		}
		return *this;
	}
	
};

inline quaternion operator *(real f, const quaternion &q) {
	return q * f;
}

inline vector3 operator *(const vector3 &lhs, const quaternion &rhs) {
	return rhs * lhs;
}

_vn_end

#endif
