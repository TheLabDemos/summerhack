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
 * @file sphvec.h
 * @author John Tsiombikas
 * @date 7 July 2005
 *
 * Spherical coordinates.
 */

#ifndef SPHVEC_H_
#define SPHVEC_H_

#include "vmath_types.h"

/** Vector in spherical coordinates */
class SphVector {
public:
	scalar_t theta, phi, r;

	SphVector(scalar_t theta = 0.0, scalar_t phi = 0.0, scalar_t r = 1.0);
	SphVector(const Vector3 &cvec);
	SphVector &operator =(const Vector3 &cvec);
};

#endif	// SPHVEC_H_
