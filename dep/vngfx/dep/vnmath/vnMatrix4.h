//
//  vnMatrix4.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnMatrix4_h
#define vnmath_vnMatrix4_h

#include "vnMatrix3.h"
#include "vnPlane3.h"
#include "vnVector4.h"
#include "vnAssert.h"

_vn_begin

struct matrix4 {
	union {
		struct {
			real m11, m12, m13, m14;
			real m21, m22, m23, m24;
			real m31, m32, m33, m34;
			real m41, m42, m43, m44;
		};
		real m[16];
	};
	
	enum contructor {
		c_nothing = 0,
		c_identity,
		c_copy,
		c_transposed,
		c_inverse,
		c_inverse_transposed
	};
	
	matrix4(contructor c = c_identity) {
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
	
	matrix4(const matrix4 &rhs, contructor c = c_copy) {
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
				rhs.transposed(*this);
				break;
			case c_inverse:
				if (!rhs.inverse(*this)) {
					memset(m, 0, sizeof(m));
				}
				break;
			case c_inverse_transposed: {
				matrix4 temp(c_nothing);
				if (!rhs.inverse(temp)) {
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
		return m[row * 4 + col];
	}
	
	real operator ()(const int row, const int col) const {
		return m[row * 4 + col];
	}
	
	
	matrix4 & operator =(const matrix4 &rhs) {
		memcpy(m, rhs.m, sizeof(m));
		return *this;
	}
	
	matrix4 operator +(const matrix4 &rhs) const {
		matrix4 ret(c_nothing);
		ret.m11 = m11 + rhs.m11;
		ret.m12 = m12 + rhs.m12;
		ret.m13 = m13 + rhs.m13;
		ret.m14 = m14 + rhs.m14;
		ret.m21 = m21 + rhs.m21;
		ret.m22 = m22 + rhs.m22;
		ret.m23 = m23 + rhs.m23;
		ret.m24 = m24 + rhs.m24;
		ret.m31 = m31 + rhs.m31;
		ret.m32 = m32 + rhs.m32;
		ret.m33 = m33 + rhs.m33;
		ret.m34 = m34 + rhs.m34;
		ret.m41 = m41 + rhs.m41;
		ret.m42 = m42 + rhs.m42;
		ret.m43 = m43 + rhs.m43;
		ret.m44 = m44 + rhs.m44;
		return ret;
	}
	
	matrix4 operator -(const matrix4 &rhs) const {
		matrix4 ret(c_nothing);
		ret.m11 = m11 - rhs.m11;
		ret.m12 = m12 - rhs.m12;
		ret.m13 = m13 - rhs.m13;
		ret.m14 = m14 - rhs.m14;
		ret.m21 = m21 - rhs.m21;
		ret.m22 = m22 - rhs.m22;
		ret.m23 = m23 - rhs.m23;
		ret.m24 = m24 - rhs.m24;
		ret.m31 = m31 - rhs.m31;
		ret.m32 = m32 - rhs.m32;
		ret.m33 = m33 - rhs.m33;
		ret.m34 = m34 - rhs.m34;
		ret.m41 = m41 - rhs.m41;
		ret.m42 = m42 - rhs.m42;
		ret.m43 = m43 - rhs.m43;
		ret.m44 = m44 - rhs.m44;
		return ret;
	}
	
	matrix4 operator *(const real s) const {
		matrix4 ret(c_nothing);
		ret.m11 = m11 * s;
		ret.m12 = m12 * s;
		ret.m13 = m13 * s;
		ret.m14 = m14 * s;
		ret.m21 = m21 * s;
		ret.m22 = m22 * s;
		ret.m23 = m23 * s;
		ret.m24 = m24 * s;
		ret.m31 = m31 * s;
		ret.m32 = m32 * s;
		ret.m33 = m33 * s;
		ret.m34 = m34 * s;
		ret.m41 = m41 * s;
		ret.m42 = m42 * s;
		ret.m43 = m43 * s;
		ret.m44 = m44 * s;
		return ret;
	}
	
	vector4 operator *(const vector4 &rhs) const {
		return vector4(
					   m11 * rhs.x + m12 * rhs.y + m13 * rhs.z + m14 * rhs.w,
					   m21 * rhs.x + m22 * rhs.y + m23 * rhs.z + m24 * rhs.w,
					   m31 * rhs.x + m32 * rhs.y + m33 * rhs.z + m34 * rhs.w,
					   m41 * rhs.x + m42 * rhs.y + m43 * rhs.z + m44 * rhs.w
					   );
	}
	
	plane3 operator *(const plane3 &rhs) const {
		return plane3(
					  m11 * rhs.normal.x + m12 * rhs.normal.y + m13 * rhs.normal.z + m14 * rhs.d,
					  m21 * rhs.normal.x + m22 * rhs.normal.y + m23 * rhs.normal.z + m24 * rhs.d,
					  m31 * rhs.normal.x + m32 * rhs.normal.y + m33 * rhs.normal.z + m34 * rhs.d,
					  m41 * rhs.normal.x + m42 * rhs.normal.y + m43 * rhs.normal.z + m44 * rhs.d
					  );
	}
	
	matrix4 operator *(const matrix4 &rhs) const {
		return matrix4(c_nothing).set_by_product( *this, rhs );
	}
	
	bool operator ==(const matrix4 &rhs) const {
		for (int i = 0; i < 16; ++i) {
			if (m[i] != rhs.m[i]) {
				return false;
			}
		}
		return true;
	}
	
	bool operator !=(const matrix4 &rhs) const {
		return !operator ==(rhs);
	}
	
	// functions
	
	vector4 & row(int index) {
		return *(vector4 *)(m + index * 4);
	}
	
	const vector4 & row(int index) const {
		return *(vector4 *)(m + index * 4);
	}
	
	vector4 column(int index) const {
		const real *p = m + index;
		return vector4(p[0], p[4], p[8], p[12]);
	}
	
	real column_dot_product(int index, const vector4 &v) const {
		const real *p = m + index;
		return p[0] * v.x + p[4] * v.y + p[8] * v.z + p[12] * v.w;
	}
	
	real column_dot_product_vector(int index, const vector3 &v) const {
		const real *p = m + index;
		return p[0] * v.x + p[4] * v.y + p[8] * v.z + p[12];
	}
	
	real column_dot_product_normal(int index, const vector3 &v) const {
		const real *p = m + index;
		return p[0] * v.x + p[4] * v.y + p[8] * v.z;
	}
	
	matrix4 & make_identity() {
		memset( m, 0, sizeof(m) );
		m11 = m22 = m33 = m44 = (real)1;
		return *this;
	}
	
	matrix4 & set(real *_m) {
		memcpy(m, _m, sizeof(m));
		return *this;
	}
	
	matrix4 & set(const matrix3 &rotation) {
		m11 = rotation.m11;
		m12 = rotation.m12;
		m13 = rotation.m13;
		m21 = rotation.m21;
		m22 = rotation.m22;
		m23 = rotation.m23;
		m31 = rotation.m31;
		m32 = rotation.m32;
		m33 = rotation.m33;
		m41 = m42 = m43 = 0;
		m44 = 1;
		return *this;
	}
	
	matrix4 & set(const matrix3 &rotation, const vector3 &translation) {
		m11 = rotation.m11;
		m12 = rotation.m12;
		m13 = rotation.m13;
		m21 = rotation.m21;
		m22 = rotation.m22;
		m23 = rotation.m23;
		m31 = rotation.m31;
		m32 = rotation.m32;
		m33 = rotation.m33;
		m41 = translation.x;
		m42 = translation.y;
		m43 = translation.z;
		m44 = 1;
		return *this;
	}
	
	matrix4 & set(const vector3 &scale, const matrix3 &rotation, const vector3 &translation) {
		m11 = rotation.m11 * scale.x;
		m12 = rotation.m12 * scale.x;
		m13 = rotation.m13 * scale.x;
		m21 = rotation.m21 * scale.y;
		m22 = rotation.m22 * scale.y;
		m23 = rotation.m23 * scale.y;
		m31 = rotation.m31 * scale.z;
		m32 = rotation.m32 * scale.z;
		m33 = rotation.m33 * scale.z;
		m41 = translation.x;
		m42 = translation.y;
		m43 = translation.z;
		m44 = 1;
		return *this;
	}
	
	matrix4 & set(const quaternion &rotation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if (math::is_zero(rx2 + ry2 + rz2)) {
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
		m14 = 0;
		
		m21 = 2 * (rxy - rzw);
		m22 = 1 - 2 * (rx2 + rz2);
		m23 = 2 * (ryz + rxw);
		m24 = 0;
		
		m31 = 2 * (rxz + ryw);
		m32 = 2 * (ryz - rxw);
		m33 = 1 - 2 * (rx2 + ry2);
		m34 = 0;
		
		m41 = m42 = m43 = 0;
		m44 = 1;
		return *this;
	}
	
	matrix4 & set(const quaternion &rotation, const vector3 &translation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if ( math::is_zero( rx2 + ry2 + rz2 ) ) {
			memset( m, 0, sizeof(m) );
			m11 = 1; m22 = 1; m33 = 1;
		} else {
			const real rxy = rotation.x * rotation.y;
			const real rzw = rotation.z * rotation.w;
			const real rxw = rotation.x * rotation.w;
			const real ryz = rotation.y * rotation.z;
			const real rxz = rotation.x * rotation.z;
			const real ryw = rotation.y * rotation.w;
			m11 = 1 - 2 * (ry2 + rz2);
			m12 = 2 * (rxy + rzw);
			m13 = 2 * (rxz - ryw);
			m14 = 0;
			
			m21 = 2 * (rxy - rzw);
			m22 = 1 - 2 * (rx2 + rz2);
			m23 = 2 * (ryz + rxw);
			m24 = 0;
			
			m31 = 2 * (rxz + ryw);
			m32 = 2 * (ryz - rxw);
			m33 = 1 - 2 * (rx2 + ry2);
			m34 = 0;
			
		}
		m41 = translation.x; m42 = translation.y; m43 = translation.z; m44 = 1;
		return *this;
	}
	
	matrix4 & set(const vector3 &scale, const quaternion &rotation, const vector3 &translation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if (math::is_zero(rx2 + ry2 + rz2)) {
			memset( m, 0, sizeof(m) );
			m11 = scale.x; m22 = scale.y; m33 = scale.z;
		} else {
			const real rxy = rotation.x * rotation.y;
			const real rzw = rotation.z * rotation.w;
			const real rxw = rotation.x * rotation.w;
			const real ryz = rotation.y * rotation.z;
			const real rxz = rotation.x * rotation.z;
			const real ryw = rotation.y * rotation.w;
			m11 = (1 - 2 * (ry2 + rz2)) * scale.x;
			m12 = 2 * (rxy + rzw) * scale.x;
			m13 = 2 * (rxz - ryw) * scale.x;
			m14 = 0;
			
			m21 = 2 * (rxy - rzw) * scale.y;
			m22 = (1 - 2 * (rx2 + rz2)) * scale.y;
			m23 = 2 * (ryz + rxw) * scale.y;
			m24 = 0;
			
			m31 = 2 * (rxz + ryw) * scale.z;
			m32 = 2 * (ryz - rxw) * scale.z;
			m33 = (1 - 2 * (rx2 + ry2)) * scale.z;
			m34 = 0;
		}
		m41 = translation.x; m42 = translation.y; m43 = translation.z; m44 = 1;
		return *this;
	}
	
	matrix4 & set(const real scale, const quaternion &rotation, const vector3 &translation) {
		const real rx2 = rotation.x * rotation.x;
		const real ry2 = rotation.y * rotation.y;
		const real rz2 = rotation.z * rotation.z;
		if ( math::is_zero( rx2 + ry2 + rz2 ) ) {
			memset( m, 0, sizeof(m) );
			m11 = scale; m22 = scale; m33 = scale;
		} else {
			const real rxy = rotation.x * rotation.y;
			const real rzw = rotation.z * rotation.w;
			const real rxw = rotation.x * rotation.w;
			const real ryz = rotation.y * rotation.z;
			const real rxz = rotation.x * rotation.z;
			const real ryw = rotation.y * rotation.w;
			m11 = (1 - 2 * (ry2 + rz2)) * scale;
			m12 = 2 * (rxy + rzw) * scale;
			m13 = 2 * (rxz - ryw) * scale;
			m14 = 0;
			
			m21 = 2 * (rxy - rzw) * scale;
			m22 = (1 - 2 * (rx2 + rz2)) * scale;
			m23 = 2 * (ryz + rxw) * scale;
			m24 = 0;
			
			m31 = 2 * (rxz + ryw) * scale;
			m32 = 2 * (ryz - rxw) * scale;
			m33 = (1 - 2 * (rx2 + ry2)) * scale;
			m34 = 0;
		}
		m41 = translation.x; m42 = translation.y; m43 = translation.z; m44 = 1;
		return *this;
	}
	
	matrix4 & set_by_product(const matrix4 &lhs, const matrix4 &rhs) {
		m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 + lhs.m14 * rhs.m41;
		m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 + lhs.m14 * rhs.m42;
		m13 = lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 + lhs.m14 * rhs.m43;
		m14 = lhs.m11 * rhs.m14 + lhs.m12 * rhs.m24 + lhs.m13 * rhs.m34 + lhs.m14 * rhs.m44;
		
		m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 + lhs.m24 * rhs.m41;
		m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 + lhs.m24 * rhs.m42;
		m23 = lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 + lhs.m24 * rhs.m43;
		m24 = lhs.m21 * rhs.m14 + lhs.m22 * rhs.m24 + lhs.m23 * rhs.m34 + lhs.m24 * rhs.m44;
		
		m31 = lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 + lhs.m34 * rhs.m41;
		m32 = lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 + lhs.m34 * rhs.m42;
		m33 = lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 + lhs.m34 * rhs.m43;
		m34 = lhs.m31 * rhs.m14 + lhs.m32 * rhs.m24 + lhs.m33 * rhs.m34 + lhs.m34 * rhs.m44;
		
		m41 = lhs.m41 * rhs.m11 + lhs.m42 * rhs.m21 + lhs.m43 * rhs.m31 + lhs.m44 * rhs.m41;
		m42 = lhs.m41 * rhs.m12 + lhs.m42 * rhs.m22 + lhs.m43 * rhs.m32 + lhs.m44 * rhs.m42;
		m43 = lhs.m41 * rhs.m13 + lhs.m42 * rhs.m23 + lhs.m43 * rhs.m33 + lhs.m44 * rhs.m43;
		m44 = lhs.m41 * rhs.m14 + lhs.m42 * rhs.m24 + lhs.m43 * rhs.m34 + lhs.m44 * rhs.m44;
		return *this;
	}
	
	matrix4 & set_by_inverse(const vector3 &scale, const quaternion &rotation, const vector3 &translation) {
		quaternion invrot(rotation.w, -rotation.x, -rotation.y, -rotation.z);
		vector3 invscale = 1 / scale;
		const real rx2 = invrot.x * invrot.x;
		const real ry2 = invrot.y * invrot.y;
		const real rz2 = invrot.z * invrot.z;
		if (math::is_zero( rx2 + ry2 + rz2)) {
			memset( m, 0, sizeof(m) );
			m11 = invscale.x;
			m22 = invscale.y;
			m33 = invscale.z;
			m41 = -translation.x * invscale.x;
			m42 = -translation.y * invscale.y;
			m43 = -translation.z * invscale.z;
		} else {
			const real rxy = invrot.x * invrot.y;
			const real rzw = invrot.z * invrot.w;
			const real rxw = invrot.x * invrot.w;
			const real ryz = invrot.y * invrot.z;
			const real rxz = invrot.x * invrot.z;
			const real ryw = invrot.y * invrot.w;
			m11 = (1 - 2 * (ry2 + rz2)) * invscale.x;
			m12 = 2 * (rxy + rzw) * invscale.y;
			m13 = 2 * (rxz - ryw) * invscale.z;
			m14 = 0;
			
			m21 = 2 * (rxy - rzw) * invscale.x;
			m22 = (1 - 2 * (rx2 + rz2)) * invscale.y;
			m23 = 2 * (ryz + rxw) * invscale.z;
			m24 = 0;
			
			m31 = 2 * (rxz + ryw) * invscale.x;
			m32 = 2 * (ryz - rxw) * invscale.y;
			m33 = (1 - 2 * (rx2 + ry2)) * invscale.z;
			m34 = 0;
			
			m41 = -translation.x * m11 - translation.y * m21 - translation.z * m31;
			m42 = -translation.x * m12 - translation.y * m22 - translation.z * m32;
			m43 = -translation.x * m13 - translation.y * m23 - translation.z * m33;
		}
		m44 = 1;
		return *this;
	}
	
	matrix4 & set_by_inverse(const vector3 &scale, const matrix3 &rotation, const vector3 &translation) {
		vector3 invs(math::reciprocal(scale.x), math::reciprocal(scale.y), math::reciprocal(scale.z));
		m11 = rotation.m11 * invs.x;
		m12 = rotation.m21 * invs.y;
		m13 = rotation.m31 * invs.z;
		m14 = 0;
		
		m21 = rotation.m12 * invs.x;
		m22 = rotation.m22 * invs.y;
		m23 = rotation.m32 * invs.z;
		m24 = 0;
		
		m31 = rotation.m13 * invs.x;
		m32 = rotation.m23 * invs.y;
		m33 = rotation.m33 * invs.z;
		m34 = 0;
		
		m41 = -translation.x * m11 - translation.y * m21 - translation.z * m31;
		m42 = -translation.x * m12 - translation.y * m22 - translation.z * m32;
		m43 = -translation.x * m13 - translation.y * m23 - translation.z * m33;
		return *this;
	}
	
	void transposed(matrix4 &result) const {
		result.m11 = m11;
		result.m12 = m21;
		result.m13 = m31;
		result.m14 = m41;
		
		result.m21 = m12;
		result.m22 = m22;
		result.m23 = m32;
		result.m24 = m42;
		
		result.m31 = m13;
		result.m32 = m23;
		result.m33 = m33;
		result.m34 = m43;
		
		result.m41 = m14;
		result.m42 = m24;
		result.m43 = m34;
		result.m44 = m44;
	}
	
	matrix4 transposed() const {
		matrix4 ret( c_nothing );
		transposed( ret );
		return ret;
	}
	
	bool inverse(matrix4 &result) const {
		real v1 = m31 * m42 - m32 * m41;
		real v2 = m31 * m43 - m33 * m41;
		real v3 = m31 * m44 - m34 * m41;
		real v4 = m32 * m43 - m33 * m42;
		real v5 = m32 * m44 - m34 * m42;
		real v6 = m33 * m44 - m34 * m43;
		
		real t11 = + (v6 * m22 - v5 * m23 + v4 * m24);
		real t21 = - (v6 * m21 - v3 * m23 + v2 * m24);
		real t31 = + (v5 * m21 - v3 * m22 + v1 * m24);
		real t41 = - (v4 * m21 - v2 * m22 + v1 * m23);
		
		real d = t11 * m11 + t21 * m12 + t31 * m13 + t41 * m14;
		
		if (math::is_zero(d)) {
			return false;
		}
		
		d = math::reciprocal(d);
		
		result.m11 = t11 * d;
		result.m21 = t21 * d;
		result.m31 = t31 * d;
		result.m41 = t41 * d;
		
		result.m12 = - (v6 * m12 - v5 * m13 + v4 * m14) * d;
		result.m22 = + (v6 * m11 - v3 * m13 + v2 * m14) * d;
		result.m32 = - (v5 * m11 - v3 * m12 + v1 * m14) * d;
		result.m42 = + (v4 * m11 - v2 * m12 + v1 * m13) * d;
		
		v1 = m21 * m42 - m22 * m41;
		v2 = m21 * m43 - m23 * m41;
		v3 = m21 * m44 - m24 * m41;
		v4 = m22 * m43 - m23 * m42;
		v5 = m22 * m44 - m24 * m42;
		v6 = m23 * m44 - m24 * m43;
		
		result.m13 = + (v6 * m12 - v5 * m13 + v4 * m14) * d;
		result.m23 = - (v6 * m11 - v3 * m13 + v2 * m14) * d;
		result.m33 = + (v5 * m11 - v3 * m12 + v1 * m14) * d;
		result.m43 = - (v4 * m11 - v2 * m12 + v1 * m13) * d;
		
		v1 = m32 * m21 - m31 * m22;
		v2 = m33 * m21 - m31 * m23;
		v3 = m34 * m21 - m31 * m24;
		v4 = m33 * m22 - m32 * m23;
		v5 = m34 * m22 - m32 * m24;
		v6 = m34 * m23 - m33 * m24;
		
		result.m14 = - (v6 * m12 - v5 * m13 + v4 * m14) * d;
		result.m24 = + (v6 * m11 - v3 * m13 + v2 * m14) * d;
		result.m34 = - (v5 * m11 - v3 * m12 + v1 * m14) * d;
		result.m44 = + (v4 * m11 - v2 * m12 + v1 * m13) * d;
		
		return true;
	}
	
	bool check_identity(const real tolerance = math::constant<real>::rounding_error) const {
		if (!math::equals(m11, (real)1, tolerance) || !math::equals(m22, (real)1, tolerance) || !math::equals(m33, (real)1, tolerance) || !math::equals(m44, (real)1, tolerance)) {
			return false;
		}

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (i != j && !math::is_zero(m[i * 4 + j], tolerance)) {
					return false;
				}
			}
		}
		return true;
	}
	
	bool check_affine() const {
		return m14 == 0 && m24 == 0 && m34 == 0 && m44 == 1;
	}
	
	bool check_orthogonal(const real tolerance = math::constant<real>::rounding_error) const {
		real dp = m11 * m21 + m12 * m22 + m13 * m23 + m14 * m24;
		if (!math::is_zero(dp, tolerance)) {
			return false;
		}
		
		dp = m11 * m31 + m12 * m32 + m13 * m33 + m14 * m34;
		if (!math::is_zero(dp, tolerance)) {
			return false;
		}
		
		dp = m11 * m41 + m12 * m42 + m13 * m43 + m14 * m44;
		if (!math::is_zero(dp, tolerance)) {
			return false;
		}
		
		dp = m21 * m31 + m22 * m32 + m23 * m33 + m24 * m34;
		if ( !math::is_zero(dp, tolerance) ) {
			return false;
		}
		
		dp = m21 * m41 + m22 * m42 + m23 * m43 + m24 * m44;
		if ( !math::is_zero(dp, tolerance) ) {
			return false;
		}
		
		dp = m31 * m41 + m32 * m42 + m33 * m43 + m34 * m44;
		return math::is_zero(dp, tolerance);
	}
	
	void decomposition(vector3 &scale, matrix3 &rotation, vector3 &translation) const {
		vnassert(check_affine());
		translation.set( m41, m42, m43 );
		rotation.m11 = m11;
		rotation.m12 = m12;
		rotation.m13 = m13;
		rotation.m21 = m21;
		rotation.m22 = m22;
		rotation.m23 = m23;
		rotation.m31 = m31;
		rotation.m32 = m32;
		rotation.m33 = m33;
		scale.x = rotation.row(0).normalize();
		scale.y = rotation.row(1).normalize();
		scale.z = rotation.row(2).normalize();
	}
	
	void decomposition(vector3 &scale, vector3 &translation, quaternion *rotation = 0) const {
		matrix3 m;
		decomposition(scale, m, translation);
		if (rotation) {
			m.to_quaternion(*rotation);
		}
	}
	
	vector3 affine_transform_vector(const vector3 &v) const {
		vnassert(check_affine());
		return vector3(
					   v.x * m11 + v.y * m21 + v.z * m31 + m41,
					   v.x * m12 + v.y * m22 + v.z * m32 + m42,
					   v.x * m13 + v.y * m23 + v.z * m33 + m43
					   );
	}
	
	vector3 affine_transform_normal(const vector3 &v) const {
		vnassert(check_affine());
		return vector3(
					   v.x * m11 + v.y * m21 + v.z * m31,
					   v.x * m12 + v.y * m22 + v.z * m32,
					   v.x * m13 + v.y * m23 + v.z * m33
					   );
	}
};

inline vector4 operator *(const vector4 &v, const matrix4 &m) {
	return vector4(
				   v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
				   v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
				   v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
				   v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44
				   );
}

inline plane3 operator *(const plane3 &pn, const matrix4 &m) {
	return plane3(
				  pn.normal.x * m.m11 + pn.normal.y * m.m21 + pn.normal.z * m.m31 + pn.d * m.m41,
				  pn.normal.x * m.m12 + pn.normal.y * m.m22 + pn.normal.z * m.m32 + pn.d * m.m42,
				  pn.normal.x * m.m13 + pn.normal.y * m.m23 + pn.normal.z * m.m33 + pn.d * m.m43,
				  pn.normal.x * m.m14 + pn.normal.y * m.m24 + pn.normal.z * m.m34 + pn.d * m.m44
				  );
}

_vn_end

#endif
