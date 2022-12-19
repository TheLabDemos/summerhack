obj = atmosphere.o background.o camera.o chunk.o ease.o file.o float.o io.o \
	  light.o material.o matrix.o mesh.o node.o quat.o shadow.o tcb.o tracks.o \
	  vector.o viewport.o

liba = ../lib3ds.a

CFLAGS = -O3 -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
