obj = adler32.o compress.o crc32.o deflate.o gzio.o infback.o inffast.o \
	  inflate.o inftrees.o trees.o uncompr.o zutil.o

liba = ../libz.a

CFLAGS = -O3

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
