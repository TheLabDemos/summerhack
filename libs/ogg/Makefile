sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = bitwise.w32.o framing.w32.o
	liba = ../w32/libogg.a
else
	obj = bitwise.o framing.o
	liba = ../libogg.a
endif

CFLAGS = -O3 -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
