/*
This file is part of XRay, a photorealistic 3D rendering library.
Copyright (C) 2005 John Tsiombikas

XRay is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

XRay is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XRay; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * @file quaternion.h
 * @author John Tsiombikas
 * @date 28 June 2005
 *
 * Quaternion math.
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <iostream>
#include "vmath_types.h"
#include "vector.h"

/** Quaternion */
class Quaternion {
public:
	scalar_t s;
	Vector3 v;

	Quaternion();
	Quaternion(scalar_t s, const Vector3 &v);
	Quaternion(scalar_t s, scalar_t x, scalar_t y, scalar_t z);
	Quaternion(const Vector3 &axis, scalar_t angle);

	Quaternion operator +(const Quaternion &quat) const;
	Quaternion operator -(const Quaternion &quat) const;
	Quaternion operator -() const;
	Quaternion operator *(const Quaternion &quat) const;
	
	void operator +=(const Quaternion &quat);
	void operator -=(const Quaternion &quat);
	void operator *=(const Quaternion &quat);

	void reset_identity();

	Quaternion conjugate() const;

	scalar_t length() const;
	scalar_t length_sq() const;
	
	void normalize();
	Quaternion normalized() const;

	Quaternion inverse() const;

	void set_rotation(const Vector3 &axis, scalar_t angle);
	void rotate(const Vector3 &axis, scalar_t angle);
	/* note: this is a totally different operation from the above
	 * this treats the quaternion as signifying direction and rotates
	 * it by a rotation quaternion by rot * q * rot'
	 */
	void rotate(const Quaternion &q);

	Matrix3x3 get_rotation_matrix() const;
	
	friend Quaternion slerp(const Quaternion &q1, const Quaternion &q2, scalar_t t);
	
	friend std::ostream &operator <<(std::ostream &out, const Quaternion &q);
};


#endif	// QUATERNION_H_
