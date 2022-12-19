obj = bitwise.o framing.o

liba = ../libogg.a

CFLAGS = -O3 -I..

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
