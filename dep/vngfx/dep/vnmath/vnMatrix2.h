//
//  vnMatrix2.h
//  vnmath
//
//  Created by Wayne on 13-12-28.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnMatrix2_h
#define vnmath_vnMatrix2_h

#include "vnVector2.h"

_vn_begin

struct matrix2 {
	union {
		struct {
			real m11, m12;
			real m21, m22;
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
	
	matrix2(contructor c = c_identity) {
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
	
	matrix2(const matrix2 &rhs, contructor c = c_copy) {
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
				matrix2 temp(c_nothing);
				if (!rhs.inverse(temp) ) {
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
		return m[row * 2 + col];
	}
	
	real operator ()(const int row, const int col) const {
		return m[row * 2 + col];
	}
	
	
	matrix2 & operator =(const matrix2 &rhs) {
		memcpy(m, rhs.m, sizeof(m));
		return *this;
	}
	
	matrix2 operator +(const matrix2 &rhs) const {
		matrix2 ret(c_nothing);
		ret.m11 = m11 + rhs.m11;
		ret.m12 = m12 + rhs.m12;
		ret.m21 = m21 + rhs.m21;
		ret.m22 = m22 + rhs.m22;
		return ret;
	}
	
	matrix2 operator -(const matrix2 &rhs) const {
		matrix2 ret(c_nothing);
		ret.m11 = m11 - rhs.m11;
		ret.m12 = m12 - rhs.m12;
		ret.m21 = m21 - rhs.m21;
		ret.m22 = m22 - rhs.m22;
		return ret;
	}
	
	matrix2 operator *(const real s) const {
		matrix2 ret(c_nothing);
		ret.m11 = m11 * s;
		ret.m12 = m12 * s;
		ret.m21 = m21 * s;
		ret.m22 = m22 * s;
		return ret;
	}
	
	vector2f operator *(const vector2f &rhs) const {
		return vector2f(m11 * rhs.x + m12 * rhs.y, m21 * rhs.x + m22 * rhs.y);
	}
	
	matrix2 operator *(const matrix2 &rhs) const {
		return matrix2(c_nothing).set_by_product(*this, rhs);
	}
	
	bool operator ==(const matrix2 &rhs) const {
		for (int i = 0; i < 4; ++i) {
			if (m[i] != rhs.m[i]) {
				return false;
			}
		}
		return true;
	}
	
	bool operator !=(const matrix2 &rhs) const {
		return !operator ==(rhs);
	}
	
	matrix2 & operator *=(const real s) {
		m11 *= s;
		m12 *= s;
		m21 *= s;
		m22 *= s;
		return *this;
	}
	
	matrix2 & operator *=(const matrix2 &rhs) {
		matrix2 t(*this);
		set_by_product(t, rhs);
		return *this;
	}
	
	// functions
	
	vector2f & row(const int idx) {
		return *(vector2f *)(m + idx * 2);
	}
	
	const vector2f & row(const int idx) const {
		return *(const vector2f *)(m + idx * 2);
	}
	
	matrix2 & make_identity() {
		m11 = m22 = (real)1;
		m12 = m21 = 0;
		return *this;
	}
	
	matrix2 & make_rotation(real angle) {
		real s, c;
		math::sincos(angle, s, c);
		m11 = c;
		m12 = s;
		m21 = -s;
		m22 = c;
		return *this;
	}
	
	matrix2 & make_rotation(const vector2f &v) {
		m11 = m22 = v.x;
		m12 = v.y;
		m21 = -v.y;
		return *this;
	}
	
	matrix2 & make_scale(const vector2f &v) {
		m11 = v.x;
		m22 = v.y;
		m21 = m12 = 0;
		return *this;
	}
	
	matrix2 & make_scale(real scale) {
		m11 = m22 = scale;
		m21 = m12 = 0;
		return *this;
	}
	
	matrix2 & set(real *_m) {
		m11 = _m[0];
		m12 = _m[1];
		m21 = _m[2];
		m22 = _m[3];
		return *this;
	}
	
	matrix2 & set_by_product(const matrix2 &lhs, const matrix2 &rhs) {
		m11 = lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		m12 = lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;
		
		m21 = lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		m22 = lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;
		return *this;
	}
	
	matrix2 & transposed(matrix2 &result) const {
		result.m11 = m11;
		result.m12 = m21;
		
		result.m21 = m12;
		result.m22 = m22;
		return result;
	}
	
	matrix2 transposed() const {
		matrix2 ret(c_nothing);
		transposed(ret);
		return ret;
	}
	
	bool inverse(matrix2 &result) const {
		real det = m11 * m22 - m21 * m12;
		if (math::is_zero(det)) {
			return false;
		}
		result.m11 = m22;
		result.m12 = -m12;
		result.m21 = -m21;
		result.m22 = m11;
		result *= math::reciprocal(det);
		return true;
	}
	
	
	bool check_orthogonal(const real tolerance = math::constant<real>::rounding_error) const {
		real dp = m11 * m21 + m12 * m22;
		if (!math::is_zero(dp, tolerance) ) {
			return false;
		}
		return true;
	}
	
};

inline vector2f operator *(const vector2f &v, const matrix2 &m) {
	return vector2f(v.x * m.m11 + v.y * m.m21, v.x * m.m12 + v.y * m.m22);
}

inline vector2f & operator *=(vector2f &v, const matrix2 &m) {
	return v.set(v.x * m.m11 + v.y * m.m21, v.x * m.m12 + v.y * m.m22);
}

_vn_end


#endif
