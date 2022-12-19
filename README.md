Summerhack / The Lab
====================
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

### Windows

TODO

### MacOS X

TODO
