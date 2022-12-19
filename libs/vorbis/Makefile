obj = analysis.o bitrate.o block.o codebook.o envelope.o floor0.o floor1.o \
	  info.o lookup.o lpc.o lsp.o mapping0.o mdct.o psy.o registry.o res0.o \
	  sharedbook.o smallft.o synthesis.o vorbisenc.o vorbisfile.o window.o

liba = ../libvorbis.a

CFLAGS = -O3 -I. -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
