//
//  vnMatrix3.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnMatrix3_h
#define vnmath_vnMatrix3_h

#include "vnVector3.h"
#include "vnQuaternion.h"

_vn_begin

struct matrix3 {
	union {
		struct {
			real m11, m12, m13;
			real m21, m22, m23;
			real m31, m32, m33;
		};
		real m[9];
	};
	
	enum contructor {
		c_nothing = 0,
		c_identity,
		c_copy,
		c_transposed,
		c_inverse,
		c_inverse_transposed
	};
	
	matrix3(contructor c = c_identity) {
		switch ( c ) {
			case c_nothing:
			case c_copy:
				break;
			case c_identity:
			default:
				make_identity();
				break;
				
		}
	}
	
	matrix3(const matrix3 &rhs, contructor c = c_copy) {
		switch (c) {
			case c_nothing:
				break;
			case c_identity:
				make_identity();
				break;
			case c_copy:
				*this = rhs;
				break;
			case c_transposed:
				rhs.transposed( *this );
				break;
			case c_inverse:
				if (!rhs.inverse( *this )) {
					memset( m, 0, sizeof(m) );
				}
				break;
			case c_inverse_transposed: {
				matrix3 temp(c_nothing);
				if ( !rhs.inverse( temp ) ) {
					memset(m, 0, sizeof(m));
				} else {
					temp.transposed(*this);
				}
				break;
			}
		} // end swith.
	}
	
	// operators
	
	real & operator ()(const int row, const int col) {
		return m[row * 3 + col];
	}
	
	real operator ()(const int row, const int col) const {
		return m[row * 3 + col];
	}
	
	
	matrix3 & operator =(const matrix3 &rhs) {
		memcpy(m, rhs.m, sizeof(m));
		return *this;
	}
	
	matrix3 operator +(const matrix3 &rhs) const {
		matrix3 ret(c_nothing);
		ret.m11 = m11 + rhs.m11;
		ret.m12 = m12 + rhs.m12;
		ret.m13 = m13 + rhs.m13;
		ret.m21 = m21 + rhs.m21;
		ret.m22 = m22 + rhs.m22;
		ret.m23 = m23 + rhs.m23;
		ret.m31 = m31 + rhs.m31;
		ret.m32 = m32 + rhs.m32;
		ret.m33 = m33 + rhs.m33;
		return ret;
	}
	
	matrix3 operator -(const matrix3 &rhs) const {
		matrix3 ret(c_nothing);
		ret.m11 = m11 - rhs.m11;
		ret.m12 = m12 - rhs.m12;
		ret.m13 = m13 - rhs.m13;
		ret.m21 = m21 - rhs.m21;
		ret.m22 = m22 - rhs.m22;
		ret.m23 = m23 - rhs.m23;
		ret.m31 = m31 - rhs.m31;
		ret.m32 = m32 - rhs.m32;
		ret.m33 = m33 - rhs.m33;
		return ret;
	}
	
	matrix3 operator *(const real s) const {
		matrix3 ret(c_nothing);
		ret.m11 = m11 * s;
		ret.m12 = m12 * s;
		ret.m13 = m13 * s;
		ret.m21 = m21 * s;
		ret.m22 = m22 * s;
		ret.m23 = m23 * s;
		ret.m31 = m31 * s;
		ret.m32 = m32 * s;
		ret.m33 = m33 * s;
		return ret;
	}
	
	vector3 operator *(const vector3 &rhs) const {
		return vector3(m11 * rhs.x + m12 * rhs.y + m13 * rhs.z, m21 * rhs.x + m22 * rhs.y + m23 * rhs.z, m31 * rhs.x + m32 * rhs.y + m33 * rhs.z);
	}
	
	matrix3 operator *(const matrix3 &rhs) const {
		return matrix3(c_nothing).set_by_product(*this, rhs);
	}
	
	bool operator ==(const matrix3 &rhs) const {
		for (int i = 0; i < 9; ++i) {
			if (m[i] != rhs.m[i]) {
				return false;
			}
		}
		return true;
	}
	
	bool operator !=(const matrix3 &rhs) const {
		return !operator ==(rhs);
	}
	
	matrix3 & operator *=(const real s) {
		m11 *= s;
		m12 *= s;
		m13 *= s;
		m21 *= s;
		m22 *= s;
		m23 *= s;
		m31 *= s;
		m32 *= s;
		m33 *= s;
		return *this;
	}
	
	matrix3 & operator *=(const matrix3 &rhs) {
		matrix3 t(*this);
		set_by_product(t, rhs);
		return *this;
	}
	
	// functions
	
	vector3 & row(const int idx) {
		return *(vector3 *)(m + idx * 3);
	}
	
	const vector3 & row(const int idx) const {
		return *(const vector3 *)(m + idx * 3);
	}
	
	matrix3 & make_identity() {
		::memset(m, 0, sizeof(m));
		m11 = m22 = m33 = (real)1;
		return *this;
	}
	
	matrix3 & make_rotation_x(real angle) {
		real s, c;
		math::sincos(angle, s, c);
		m11 = 1;
		m12 = 0;
		m13 = 0;
		m21 = 0;
		m22 = c;
		m23 = s;
		m31 = 0;
		m32 = -s;
		m33 = c;
		return *this;
	}
	
	matrix3 & make_rotation_y(real angle) {
		real s, c;
		math::sincos(angle, s, c);
		m11 = c;
		m12 = 0;
		m13 = -s;
		m21 = 0;
		m22 = 1;
		m23 = 0;
		m31 = s;
		m32 = 0;
		m33 = c;
		return *this;
	}
	
	matrix3 & make_rotation_z(real angle) {
		real s, c;
		math::sincos(angle, s, c);
		m11 = c;
		m12 = s;
		m13 = 0;
		m21 = -s;
		m22 = c;
		m23 = 0;
		m31 = 0;
		m32 = 0;
		m33 = 1;
		return *this;
	}
	
	matrix3 & make_rotation_xy(real x, real y) {
		real sinx, cosx, siny, cosy;
		math::sincos(x, sinx, cosx);
		math::sincos(y, siny, cosy);
		m11 = cosy;
		m12 = 0;
		m13 = -siny;
		m21 = sinx * siny;
		m22 = cosx;
		m23 = sinx * cosy;
		m31 = cosx * siny;
		m32 = -sinx;
		m33 = cosx * cosy;
		return *this;
	}
	
	matrix3 & make_rotation_xz(real x, real z) {
		real sinx, cosx, sinz, cosz;
		math::sincos(x, sinx, cosx);
		math::sincos(z, sinz, cosz);
		m11 = cosz;
		m12 = sinz;
		m13 = 0;
		m21 = -cosx * sinz;
		m22 = cosx * cosz;
		m23 = sinx;
		m31 = sinx * sinz;
		m32 = -sinx * cosz;
		m33 = cosx;
		return *this;
	}
	
	matrix3 & make_rotation_yz(real y, real z) {
		real siny, cosy, sinz, cosz;
		math::sincos(y, siny, cosy);
		math::sincos(z, sinz, cosz);
		m11 = cosy * cosz;
		m12 = cosy * sinz;
		m13 = -siny;
		m21 = -sinz;
		m22 = cosz;
		m23 = 0;
		m31 = siny * cosz;
		m32 = siny * sinz;
		m33 = cosy;
		return *this;
	}
	
	matrix3 & make_rotation_xyz(real x, real y, real z) {
		real sinx, cosx, siny, cosy, sinz, cosz;
		math::sincos(x, sinx, cosx);
		math::sincos(y, siny, cosy);
		math::sincos(z, sinz, cosz);
		m11 = cosy * cosz;
		m12 = cosy * sinz;
		m13 = -siny;
		m21 = sinx * siny * cosz - cosx * sinz;
		m22 = cosx * cosz + sinx * siny * sinz;
		m23 = sinx * cosy;
		m31 = sinx * sinz + cosx * siny * cosz;
		m32 = cosx * siny * sinz - sinx * cosz;
		m33 = cosx * cosy;
		return *this;
	}
	
	matrix3 & make_rotation_axis(real angle, const vector3 &axis) {
		real s, c;
		math::sincos(angle, s, c);
		real invc = 1 - c;
		real axy = axis.x * axis.y;
		real axz = axis.x * axis.z;
		real ayz = axis.y * axis.z;
		m11 = c + invc * axis.x * axis.x;
		m12 = s * axis.z + invc * axy;
		m13 = -s * axis.y + invc * axz;
		m21 = -s * axis.z + invc * axy;
		m22 = c + invc * axis.y * axis.y;
		m23 = s * axis.x + invc * ayz;
		m31 = s * axis.y + invc * axz;
		m32 = -s * axis.x + invc * ayz;
		m33 = c + invc * axis.z * axis.z;
		return *this;
	}
	
	matrix3 & set(real *_m) {
		memcpy( m, _m, sizeof(m) );
		return *this;
	}
	
	matrix3 & set(const quaternion &rotation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if ( math::is_zero( rx2 + ry2 + rz2 ) ) {
			return make_identity();
		}
		const real rxy = rotation.x * rotation.y;
		const real rzw = rotation.z * rotation.w;
		const real rxw = rotation.x * rotation.w;
		const real ryz = rotation.y * rotation.z;
		const real rxz = rotation.x * rotation.z;
		const real ryw = rotation.y * rotation.w;
		m11 = 1 - 2 * (ry2 + rz2);
		m12 = 2 * (rxy + rzw);
		m13 = 2 * (rxz - ryw);
		
		m21 = 2 * (rxy - rzw);
		m22 = 1 - 2 * (rx2 + rz2);
		m23 = 2 * (ryz + rxw);
		
		m31 = 2 * (rxz + ryw);
		m32 = 2 * (ryz - rxw);
		m33 = 1 - 2 * (rx2 + ry2);
		return *this;
	}
	
	
	matrix3 & set(const vector3 &scale, const quaternion &rotation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if ( math::is_zero( rx2 + ry2 + rz2 ) ) {
			memset( m, 0, sizeof(m) );
			m11 = scale.x; m22 = scale.y; m33 = scale.z;
		} else {
			const real rxy = rotation.x * rotation.y;
			const real rzw = rotation.z * rotation.w;
			const real rxw = rotation.x * rotation.w;
			const real ryz = rotation.y * rotation.z;
			const real rxz = rotation.x * rotation.z;
			const real ryw = rotation.y * rotation.w;
			m11 = (1 - 2 * ry2 - 2 * rz2) * scale.x;
			m12 = (2 * rxy + 2 * rzw) * scale.x;
			m13 = (2 * rxz - 2 * ryw) * scale.x;
			
			m21 = (2 * rxy - 2 * rzw) * scale.y;
			m22 = (1 - 2 * rx2 - 2 * rz2) * scale.y;
			m23 = (2 * ryz + 2 * rxw) * scale.y;
			
			m31 = (2 * rxz + 2 * ryw) * scale.z;
			m32 = (2 * ryz + 2 * rxw) * scale.z;
			m33 = (1 - 2 * rx2 - 2 * ry2) * scale.z;
		}
		return *this;
	}
	
	matrix3 & set_by_product(const matrix3 &lhs, const matrix3 &rhs) {
		m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
		m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
		m13 = lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;
		
		m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
		m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
		m23 = lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
		
		m31 = lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
		m32 = lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
		m33 = lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;
		return *this;
	}
	
	quaternion & to_quaternion(quaternion &q) const {
		const real diag = m11 + m22 + m33 + 1;
		if(diag > 0) {
			const real scale = math::sqrt(diag) * 2;
			const real invscale = math::reciprocal(scale);
			q.x = (m23 - m32) * invscale;
			q.y = (m31 - m13) * invscale;
			q.z = (m12 - m21) * invscale;
			q.w = (real)0.25 * scale;
		} else {
			if (m11 > m22 && m11 > m33) {
				const real scale = math::sqrt(1 + m11 - m22 - m33) * 2;
				const real invscale = math::reciprocal(scale);
				q.x = (real)0.25 * scale;
				q.y = (m21 + m12) * invscale;
				q.z = (m13 + m31) * invscale;
				q.w = (m23 - m32) * invscale;
			} else if (m22 > m33) {
				const real scale = math::sqrt(1 + m22 - m11 - m33) * 2;
				const real invscale = math::reciprocal(scale);
				q.x = (m21 + m12) * invscale;
				q.y = (real)0.25 * scale;
				q.z = (m32 + m23) * invscale;
				q.w = (m31 - m13) * invscale;
			} else {
				const real scale = math::sqrt(1 + m33 - m11 - m22) * 2;
				const real invscale = math::reciprocal(scale);
				q.x = (m31 + m13) * invscale;
				q.y = (m32 + m23) * invscale;
				q.z = (real)0.25 * scale;
				q.w = (m12 - m21) * invscale;
			}
		}
		
		return q.normalize();
	}
	
	quaternion to_quaternion() const {
		quaternion q;
		return to_quaternion(q);
	}
	
	matrix3 & transposed(matrix3 &result) const {
		result.m11 = m11;
		result.m12 = m21;
		result.m13 = m31;
		
		result.m21 = m12;
		result.m22 = m22;
		result.m23 = m32;
		
		result.m31 = m13;
		result.m32 = m23;
		result.m33 = m33;
		return result;
	}
	
	matrix3 transposed() const {
		matrix3 ret( c_nothing );
		transposed( ret );
		return ret;
	}
	
	bool inverse(matrix3 &result) const {
		real d1 = m22 * m33 - m23 * m32;
		real d2 = m23 * m31 - m21 * m33;
		real d3 = m21 * m32 - m22 * m31;
		real det = m11 * d1 + m12 * d2 + m13 * d3;
		if (math::is_zero(det)) {
			return false;
		}
		result.m11 = d1;
		result.m21 = d2;
		result.m23 = d3;
		
		result.m12 = m13 * m32 - m12 * m33;
		result.m22 = m11 * m33 - m13 * m31;
		result.m23 = m12 * m31 - m11 * m32;
		
		result.m13 = m12 * m23 - m13 * m22;
		result.m23 = m13 * m21 - m11 * m23;
		result.m33 = m11 * m22 - m12 * m21;
		
		result *= math::reciprocal(det);
		
		return true;
	}
	
	bool check_identity(const real tolerance = math::constant<real>::rounding_error) const {
		if (!math::equals(m11, (real)1, tolerance) || !math::equals(m22, (real)1, tolerance) || !math::equals(m33, (real)1, tolerance)) {
			return false;
		}
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (i != j && !math::is_zero(m[i * 3 + j], tolerance)) {
					return false;
				}
			}
		}
		return true;
	}
	
	
	bool check_orthogonal(const real tolerance = math::constant<real>::rounding_error) const {
		real dp = m11 * m21 + m12 * m22 + m13 * m23;
		if (!math::is_zero(dp, tolerance) ) {
			return false;
		}
		
		dp = m11 * m31 + m12 * m32 + m13 * m33;
		if (!math::is_zero(dp, tolerance)) {
			return false;
		}
		
		
		dp = m21 * m31 + m22 * m32 + m23 * m33;
		if (!math::is_zero(dp, tolerance)) {
			return false;
		}
		
		return true;
	}
	
};

inline vector3 operator *(const vector3 &v, const matrix3 &m) {
	return vector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31, v.x * m.m12 + v.y * m.m22 + v.z * m.m32, v.x * m.m13 + v.y * m.m23 + v.z * m.m33);
}

_vn_end


#endif
