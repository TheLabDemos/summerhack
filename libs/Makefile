.PHONY: all
all: zlib libpng jpeglib lib3ds ogg vorbis

.PHONY: clean
clean: clean-zlib clean-libpng clean-jpeglib clean-lib3ds clean-ogg clean-vorbis

.PHONY: zlib
zlib:
	$(MAKE) -C zlib

.PHONY: clean-zlib
clean-zlib:
	$(MAKE) -C zlib clean


.PHONY: libpng
libpng:
	$(MAKE) -C libpng

.PHONY: clean-libpng
clean-libpng:
	$(MAKE) -C libpng clean


.PHONY: jpeglib
jpeglib:
	$(MAKE) -C jpeglib

.PHONY: clean-jpeglib
clean-jpeglib:
	$(MAKE) -C jpeglib clean


.PHONY: lib3ds
lib3ds:
	$(MAKE) -C lib3ds

.PHONY: clean-lib3ds
clean-lib3ds:
	$(MAKE) -C lib3ds clean


.PHONY: ogg
ogg:
	$(MAKE) -C ogg

.PHONY: clean-ogg
clean-ogg:
	$(MAKE) -C ogg clean


.PHONY: vorbis
vorbis:
	$(MAKE) -C vorbis

.PHONY: clean-vorbis
clean-vorbis:
	$(MAKE) -C vorbis clean
