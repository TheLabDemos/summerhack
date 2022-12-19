obj = pngerror.o pngget.o pngmem.o png.o pngpread.o pngread.o pngrio.o \
	  pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o \
	  pngwutil.o

liba = ../libpng.a

CFLAGS = -O3 -I../zlib

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
