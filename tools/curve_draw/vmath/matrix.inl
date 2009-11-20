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
 * @file matrix.inl
 * @author John Tsiombikas
 * @date 28 June 2005
 *
 * Matrix inline functions.
 */

inline scalar_t *Matrix3x3::operator [](int index) {
	return m[index];
}

inline const scalar_t *Matrix3x3::operator [](int index) const {
	return m[index];
}

inline void Matrix3x3::reset_identity() {
	memcpy(this->m, identity_matrix.m, 9 * sizeof(scalar_t));
}

inline scalar_t *Matrix4x4::operator [](int index) {
	return m[index];
}

inline const scalar_t *Matrix4x4::operator [](int index) const {
	return m[index];
}

inline void Matrix4x4::reset_identity() {
	memcpy(this->m, identity_matrix.m, 16 * sizeof(scalar_t));
}
