src = src/sumhack.cpp src/events.cpp
bin = sumhack-thelab

include src/parts/Makefile-part
include src/sdlvf/Makefile-part


obj = $(src:.cpp=.o) $(csrc:.c=.o)

opt = -O1
opt = -g
CXXFLAGS = -ansi -pedantic -Wall $(opt) -Isrc/3dengfx/src -MMD `sdl-config --cflags`
CFLAGS = -std=c89 -pedantic -Wall $(opt) -MMD `sdl-config --cflags`
libs = -Llibs src/3dengfx/lib3dengfx.a `sdl-config --libs` -lGL -lvorbis -logg \
	   -ljpeg -lpng -lz -l3ds

$(bin): $(obj) 3dengfx libs
	$(CXX) -o $@ $(obj) $(libs)

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
