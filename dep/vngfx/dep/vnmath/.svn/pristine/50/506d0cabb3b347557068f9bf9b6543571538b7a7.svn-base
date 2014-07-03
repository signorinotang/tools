//
//  vnPlane3.h
//  vnmath
//
//  Created by Wayne on 13-9-21.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnmath_vnPlane3_h
#define vnmath_vnPlane3_h

#include "vnVector3.h"

_vn_begin

struct plane3 {
	vector3 normal;
	real d;
	
	plane3() : normal(0, 0, 1), d(0) {}
	plane3(const vector3 &n, real d) : normal(n), d(d) {}
	plane3(real nx, real ny, real nz, real d) : normal(nx, ny, nz), d(d) {}
	plane3(const vector3 &n, const vector3 &p) : normal(n), d(-p.dot_product(n)) {}
	
	plane3 operator +(const vector3 &rhs) const {
		return plane3(normal, d - normal.dot_product(rhs));
	}
	
	plane3 operator -(const vector3 &rhs) const {
		return plane3(normal, d + normal.dot_product(rhs));
	}
	
	bool operator ==(const plane3 &rhs) const {
		return normal == rhs.normal && d == rhs.d;
	}
	
	bool operator !=(const plane3 &rhs) const {
		return normal != rhs.normal || d != rhs.d;
	}
	
	plane3 & operator +=(const vector3 &rhs) {
		d -= normal.dot_product( rhs );
		return *this;
	}
	
	plane3 & operator -=(const vector3 &rhs) {
		d += normal.dot_product( rhs);
		return *this;
	}
	
	bool equals(const plane3 &other, const real tolerance = math::constant<real>::rounding_error) const {
		return normal.equals(other.normal, tolerance) && math::equals(d, other.d, tolerance);
	}
	
	vector3 member_point() const {
		return normal * -d;
	}
	
	real distance(const vector3& point) const {
		return point.dot_product(normal) + d;
	}
	
	real distance(const vector3 &center, const vector3 &extent) const {
		return distance(center) - math::abs(normal.dot_product(extent) * 0.5f);
	}
	
	plane3 & set(const vector3 &n, real d) {
		normal = n;
		this->d = d;
		return *this;
	}
	
	plane3 & set(real nx, real ny, real nz, real d) {
		normal.set(nx, ny, nz);
		this->d = d;
		return *this;
	}
	
	plane3 & set(const vector3 &n, const vector3 &p) {
		normal = n;
		d = -p.dot_product(n);
		return *this;
	}
	
	plane3 & normalize() {
		real t = normal.length_square();
		if ( !math::is_zero( t ) ) {
			t = math::sqrt( t );
			normal /= t;
			d *= t;
		}
		return *this;
	}
	
	bool intersects(const vector3 &start, const vector3 &direction, real *result = 0) const {
		real k = normal.dot_product( direction );
		if (math::is_zero(k)) {
			return false;
		}
		real t = (start.dot_product(normal) + d) * math::reciprocal(k);
		if (t < 0) {
			return false;
		}
		if (result) {
			*result = t;
		}
		return true;
	}
	
};


_vn_end


#endif
