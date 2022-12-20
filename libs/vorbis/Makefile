sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = analysis.w32.o bitrate.w32.o block.w32.o codebook.w32.o envelope.w32.o floor0.w32.o floor1.w32.o \
		  info.w32.o lookup.w32.o lpc.w32.o lsp.w32.o mapping0.w32.o mdct.w32.o psy.w32.o registry.w32.o res0.w32.o \
		  sharedbook.w32.o smallft.w32.o synthesis.w32.o vorbisenc.w32.o vorbisfile.w32.o window.w32.o
	liba = ../w32/libvorbis.a
else
	obj = analysis.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o \
		  info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o \
		  sharedbook.o smallft.o synthesis.o vorbisenc.o vorbisfile.o window.o
	liba = ../libvorbis.a
endif

CFLAGS = -O3 -I. -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
