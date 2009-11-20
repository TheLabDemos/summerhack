src = src/sumhack.cpp src/events.cpp
bin = sumhack-thelab

include src/parts/Makefile-part
include src/sdlvf/Makefile-part

obj = $(src:.cpp=.o) $(csrc:.c=.o)

opt = -O1
opt = -g
CXXFLAGS = -ansi -pedantic -Wall $(opt) -Isrc/3dengfx/src `src/3dengfx/3dengfx-config --cflags`
CFLAGS = -std=c89 -pedantic -Wall $(opt) `src/3dengfx/3dengfx-config --cflags`
libs = src/3dengfx/lib3dengfx.a `src/3dengfx/3dengfx-config --libs-no-3dengfx` -lGL -lvorbisfile

$(bin): $(obj) src/3dengfx/lib3dengfx.a data/tex_list
	$(CXX) -o $@ $(obj) $(libs)

src/3dengfx/lib3dengfx.a:
	cd src/3dengfx;\
	./configure --with-gfxlib=sdl --disable-ft --enable-opt --disable-debug && make

data/tex_list: $(src)
	tools/find_textures >$@

-include $(obj:.o=.d)

%.d: %.cpp
	@set -e; rm -f $@; $(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

%.d: %.c
	@set -e; rm -f $@; $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

.PHONY: clean
clean:
	$(RM) $(obj) $(bin)

.PHONY: cleandep
cleandep:
	find src \( -name '*.d' -o -name '*.d.*' \) -exec $(RM) '{}' ';'
