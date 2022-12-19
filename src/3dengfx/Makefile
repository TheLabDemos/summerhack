PREFIX = /usr/local
dbg = -g
opt = -O1 -ffast-math -fomit-frame-pointer

inc_flags = -Isrc -I../../libs -I../../libs/zlib -I../../libs/libpng -I../../libs/jpeglib
warn_flags = -Wall -Wno-sign-compare -Wno-strict-aliasing -Wno-char-subscripts

CXXFLAGS = $(opt) $(dbg) -ansi -pedantic $(warn_flags) -MMD $(inc_flags) \
		   `sdl-config --cflags`
CFLAGS = $(opt) $(dbg) -std=c89 -pedantic $(warn_flags) -MMD $(inc_flags) \
		 `sdl-config --cflags`

include src/3dengfx/makefile.part
include src/gfx/makefile.part
include src/n3dmath2/makefile.part
include src/dsys/makefile.part
include src/common/makefile.part
include src/fxwt/makefile.part
include src/sim/makefile.part

# get all the object files in one variable $(obj)
obj = $(3dengfx_obj) \
	  $(gfx_obj) \
	  $(n3dmath2_obj) \
	  $(dsys_obj) \
	  $(common_obj) \
	  $(fxwt_obj) \
	  $(sim_obj)

lib3dengfx.a: $(obj)
	$(AR) rcs $@ $(obj)

-include $(obj:.o=.d)

.PHONY: clean
clean:
	$(RM) $(obj) lib3dengfx.a

.PHONY: cleandep
cleandep:
	$(RM) $(obj:.o=.d)
