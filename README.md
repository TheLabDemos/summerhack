Summerhack / The Lab
====================
![GNU/Linux build status](https://github.com/TheLabDemos/summerhack/actions/workflows/build_gnulinux.yml/badge.svg)
![FreeBSD build status](https://github.com/TheLabDemos/summerhack/actions/workflows/build_freebsd.yml/badge.svg)

Summerhack was a short and sweet demo I've written during the summer of 2005
for a random on-line competition. Mainly it was a showcase of the 3D engine I
was writing back then with the help of Michael Georgoulopoulos: 3dengfx, and which was
hosted at: http://engfx3d.berlios.de (which has since gone off-line, see
archived: https://web.archive.org/web/20060210131739/http://engfx3d.berlios.de ).
You can find the last version of 3dengfx at: https://github.com/jtsiomb/3dengfx

![screenshot](http://nuclear.mutantstargoat.com/sw/demos/shots/summerhack-thumb.jpg)

Pouet page: http://www.pouet.net/prod.php?which=18823

Release archive: http://nuclear.mutantstargoat.com/sw/demos/summerhack-thelab-repack.tar.gz

Video capture: https://www.youtube.com/watch?v=B9FuevPkDN8


Credits
-------
 - Demo code: John Tsiombikas (Nuclear)
 - 3D engine code: John Tsiombikas (Nuclear) and Michael Georgoulopoulos (Samurai)
 - Sound code: Vasilis Vasaitis (Savvy)
 - Music: Red Herring by Union Jack


Original readme file
--------------------
```
Summer Hack / The Lab Demos
---------------------------
Demo code: Nuclear
3d engine: Nuclear, Samurai
Sound sys: Savvy
    Music: Tourmpadakis

Greets to everyone who watches this small demoish thingy.

About this thing...
Needs a video card that doesn't suck for fillrate. No shaders or other
high-end gimmicks are used this time.
Released under the GPL, do whatever you wish with it.

Contact info...
http://thelab.demoscene.gr - nuclear@siggraph.org
```

Build instructions
------------------
If you acquired the source code from the git repo, it does not include the
binary data files (images, 3D models, and music). Grab the data file archive
from: http://nuclear.mutantstargoat.com/sw/demos/summerhack-data.tar.gz and
extract it in the project root directory, so that you now have the `img`, `geom`
and `music` subdirectories in the `data` directory.

If you got the source code from a full release archive of the demo, the data
files are already there. Of course the release archive also contains
pre-compiled binaries for multiple platforms.

### UNIX

The only dependencies are SDL 1.2 and OpenGL. After installing them, just type
`make` to build the demo. For example under Debian GNU/Linux:

    sudo apt-get install libgl-dev libsdl1.2-dev
    make

While on FreeBSD:

    sudo pkg install mesa-libs sdl
    gmake

### Windows (MSVC)

The sticking point with compiling on windows, especially with MSVC, is how to
handle dependencies. OpenGL is not an issue, but SDL must be installed and the
compiler/linker needs to be informed of its location.

For older versions of MSVC, download the pre-compiled SDL 1.2 package:
https://libsdl.org/release/SDL-devel-1.2.15-VC.zip
and unzip it in the libs directory of summerhack (so that the directory
`libs\SDL-1.2.15` exists).

For new versions of MS Visual C++ (2015 or later), use `vcpkg install sdl1` to
install SDL 1.2. vcpkg does not seem to put the `include/SDL` directory in the
include path, so you might need to change all `#include <SDL.h>` lines to
`#include <SDL/SDL.h>`. I can't be bothered to figure out a solution that works
both with and without `vcpkg` at the moment.

Once SDL is taken care of, simply open the `summerhack.sln` project and hit F7
(Build->Build Solution).


### Windows (msys2/mingw32)

Use the mingw32 environment to build summerhack. Simply install SDL with the
pacman package manager: `pacman -S mingw-w64-i686-SDL`, and type `make` to
build.

### MacOS X

TODO
