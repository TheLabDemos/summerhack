sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = pngerror.w32.o pngget.w32.o pngmem.w32.o png.w32.o pngpread.w32.o pngread.w32.o pngrio.w32.o \
		  pngrtran.w32.o pngrutil.w32.o pngset.w32.o pngtrans.w32.o pngwio.w32.o pngwrite.w32.o pngwtran.w32.o \
		  pngwutil.w32.o
	liba = ../w32/libpng.a
else
	obj = pngerror.o pngget.o pngmem.o png.o pngpread.o pngread.o pngrio.o \
		  pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o \
		  pngwutil.o
	liba = ../libpng.a
endif

CFLAGS = -O3 -I../zlib

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
