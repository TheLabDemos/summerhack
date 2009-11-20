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
 * @file vmath.inl
 * @author John Tsiombikas
 * @date 28 June 2005
 *
 * vmath inline functions.
 */

/** linear interpolation */
inline scalar_t lerp(scalar_t a, scalar_t b, scalar_t t) {
	return a + (b - a) * t;
}

/** b-spline approximation */
inline scalar_t bspline(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t) {
	return bspline(Vector4(a, b, c, d), t);
}

/** catmull-rom spline interpolation */
inline scalar_t catmull_rom_spline(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t) {
	return catmull_rom_spline(Vector4(a, b, c, d), t);
}
