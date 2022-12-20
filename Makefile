src = src/sumhack.cpp src/events.cpp
bin = sumhack-thelab

include src/parts/Makefile-part
include src/sdlvf/Makefile-part

obj = $(src:.cpp=.o) $(csrc:.c=.o)

opt = -O1
opt = -g
incdir = -Isrc/3dengfx/src -Ilibs -Ilibs/png -Ilibs/jpeglib -Ilibs/ogg -Ilibs/vorbis
CXXFLAGS = -ansi -pedantic -Wall $(opt) $(incdir) -MMD `sdl-config --cflags`
CFLAGS = -std=c89 -pedantic -Wall $(opt) $(incdir) -MMD `sdl-config --cflags`
libs = $(libdir) $(libsys) src/3dengfx/lib3dengfx.a `sdl-config --libs` $(libgl) \
	   -lvorbis -logg -ljpeg -lpng -lz -l3ds $(libc)

sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = $(src:.cpp=.w32.o) $(csrc:.c=.w32.o)
	bin = sumhack-thelab.exe
	libdir = -Llibs/w32
	libgl = -lopengl32
	libsys = -lmingw32 -lgdi32 -mconsole
	libc = -lm

else
	libdir = -Llibs
	libgl = -lGL
	libc = -lm
endif


$(bin): $(obj) 3dengfx libs
	$(CXX) -o $@ $(obj) $(libs)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

%.w32.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) -c $<

.PHONY: 3dengfx
3dengfx:
	$(MAKE) -C src/3dengfx

.PHONY: 3dengfx-clean
3dengfx-clean:
	$(MAKE) -C src/3dengfx clean

-include $(obj:.o=.d)

.PHONY: clean
clean:
	$(RM) $(obj) $(bin)

.PHONY: clean-all
clean-all: clean 3dengfx-clean cleandep clean-libs

.PHONY: cleandep
cleandep:
	$(RM) $(obj:.o=.d)

.PHONY: libs
libs:
	$(MAKE) -C libs

.PHONY: clean-libs
clean-libs:
	$(MAKE) -C libs clean

.PHONY: cross
cross:
	$(MAKE) CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ sys=mingw

.PHONY: cross-libs
cross-libs:
	$(MAKE) CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ sys=mingw -C libs

.PHONY: cross-clean
cross-clean:
	$(MAKE) CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ sys=mingw clean

.PHONY: cross-clean-libs
cross-clean-libs:
	$(MAKE) CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ sys=mingw -C libs clean
