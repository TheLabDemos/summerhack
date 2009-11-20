obj = curve_draw.o curves.o cursors.o gfx.o
bin = curve_draw

CXXFLAGS = -g -ansi -pedantic -Wall `sdl-config --cflags`

include vmath/Makefile-part

$(bin): $(obj)
	$(CXX) -o $@ $(obj) `sdl-config --libs`

.PHONY: clean
clean:
	$(RM) $(obj) $(bin)
