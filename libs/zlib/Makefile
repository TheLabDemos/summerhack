sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = adler32.w32.o compress.w32.o crc32.w32.o deflate.w32.o gzio.w32.o infback.w32.o inffast.w32.o \
		  inflate.w32.o inftrees.w32.o trees.w32.o uncompr.w32.o zutil.w32.o
	liba = ../w32/libz.a
else
	obj = adler32.o compress.o crc32.o deflate.o gzio.o infback.o inffast.o \
		  inflate.o inftrees.o trees.o uncompr.o zutil.o
	liba = ../libz.a
endif

CFLAGS = -O3

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
