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
 * @file vmath.h
 * @author John Tsiombikas
 * @date 28 June 2005
 *
 * vmath provides the mathematical foundations for 3D graphics.
 * This specific file contains various constants and basic operations
 * as well as the Basis class, which represents an orthogonal basis in
 * 3 dimensional space.
 */

#ifndef VMATH_H_
#define VMATH_H_

#include "vmath_types.h"

extern const scalar_t e;

extern const scalar_t pi;
extern const scalar_t half_pi;
extern const scalar_t quarter_pi;
extern const scalar_t two_pi;
extern const scalar_t three_half_pi;

extern const scalar_t xsmall_number;
extern const scalar_t small_number;

extern const scalar_t error_margin;


// angle conversion
#define RAD_TO_DEG(a) ((((scalar_t)a) * 360.0) / two_pi)
#define DEG_TO_RAD(a) (((scalar_t)a) * (pi / 180.0))

#define SQ(x) ((x) * (x))

#ifndef __GNUC__
#define round(x)	((x) >= 0 ? (x) + 0.5 : (x) - 0.5)
#endif	// __GNUC__

// -- mathematical & helper functions --
scalar_t frand(scalar_t range);
scalar_t integral(scalar_t (*f)(scalar_t), scalar_t low, scalar_t high, int samples);
scalar_t gaussian(scalar_t x, scalar_t mean, scalar_t sdev);

// -- interpolation and approximation --
inline scalar_t lerp(scalar_t a, scalar_t b, scalar_t t);

scalar_t bspline(const Vector4 &cpvec, scalar_t t);
inline scalar_t bspline(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t);

scalar_t catmull_rom_spline(const Vector4 &cpvec, scalar_t t);
inline scalar_t catmull_rom_spline(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t);

scalar_t bezier(const Vector4 &cp, scalar_t t);
Vector3 bezier(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, scalar_t t);
Vector3 bezier_tangent(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, scalar_t t);

// -- actual class definitions --
#include "vector.h"
#include "matrix.h"
#include "quaternion.h"
#include "sphvec.h"

/** Orthogonal basis in 3-space */
class Basis {
public:
	Vector3 i, j, k;

	Basis();
	Basis(const Vector3 &i, const Vector3 &j, const Vector3 &k);
	Basis(const Vector3 &dir, bool left_handed = true);

	void rotate(scalar_t x, scalar_t y, scalar_t z);
	void rotate(const Vector3 &axis, scalar_t angle);
	void rotate(const Matrix4x4 &mat);
	void rotate(const Quaternion &quat);

	Matrix3x3 create_rotation_matrix() const;
};

#include "vmath.inl"

#endif	// VMATH_H_
