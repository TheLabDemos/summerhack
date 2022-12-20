sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = atmosphere.w32.o background.w32.o camera.w32.o chunk.w32.o ease.w32.o file.w32.o float.w32.o io.w32.o \
		  light.w32.o material.w32.o matrix.w32.o mesh.w32.o node.w32.o quat.w32.o shadow.w32.o tcb.w32.o tracks.w32.o \
		  vector.w32.o viewport.w32.o
	liba = ../w32/lib3ds.a
else 
	obj = atmosphere.o background.o camera.o chunk.o ease.o file.o float.o io.o \
		  light.o material.o matrix.o mesh.o node.o quat.o shadow.o tcb.o tracks.o \
		  vector.o viewport.o
	liba = ../lib3ds.a
endif

CFLAGS = -O3 -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
