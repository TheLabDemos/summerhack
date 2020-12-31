/* this header is created by the 3dengfx configuration script */
#ifndef _3DENGFX_CONFIG_H_
#define _3DENGFX_CONFIG_H_

#define USING_3DENGFX
#define VER_STR	"0.5"
#define PREFIX	"/usr/local"

#define SDL					1
#define GLUT				2
#define GTK					3
#define GTKMM				4
#define NATIVE				5
#define NATIVE_X11			10
#define NATIVE_WIN32		11

#define GFX_LIBRARY SDL

#define SINGLE_PRECISION_MATH

#define COORD_LHS

#define FXWT_NO_FREETYPE

#if GFX_LIBRARY == NATIVE
#if defined(unix) || defined(__unix__)
#define NATIVE_LIB		NATIVE_X11
#elif defined(WIN32) || defined(__WIN32__)
#define NATIVE_LIB		NATIVE_WIN32
#endif	/* unix/win32 */
#endif	/* GFX_LIBRARY == NATIVE */

#endif	/* _3DENGFX_CONFIG_H_ */
