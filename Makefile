src = src/sumhack.cpp src/events.cpp
bin = sumhack-thelab

include src/parts/Makefile-part
include src/sdlvf/Makefile-part


obj = $(src:.cpp=.o) $(csrc:.c=.o)

opt = -O1
opt = -g
CXXFLAGS = -ansi -pedantic -Wall $(opt) -Isrc/3dengfx/src -MMD `sdl-config --cflags`
CFLAGS = -std=c89 -pedantic -Wall $(opt) -MMD `sdl-config --cflags`
libs = src/3dengfx/lib3dengfx.a `sdl-config --libs` -lGL -lvorbisfile -ljpeg -lpng -lz

$(bin): $(obj) 3dengfx
	$(CXX) -o $@ $(obj) $(libs)

.PHONY: 3dengfx
3dengfx:
	$(MAKE) -C src/3dengfx

-include $(obj:.o=.d)

.PHONY: clean
clean:
	$(RM) $(obj) $(bin)

.PHONY: cleandep
cleandep:
	$(RM) $(obj:.o=.d)
