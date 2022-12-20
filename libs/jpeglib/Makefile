sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/')
ifeq ($(sys), mingw)
	obj = jcapimin.w32.o jcapistd.w32.o jccoefct.w32.o jccolor.w32.o jcdctmgr.w32.o jchuff.w32.o jcinit.w32.o \
		  jcmainct.w32.o jcmarker.w32.o jcmaster.w32.o jcomapi.w32.o jcparam.w32.o jcphuff.w32.o jcprepct.w32.o \
		  jcsample.w32.o jctrans.w32.o jdapimin.w32.o jdapistd.w32.o jdatadst.w32.o jdatasrc.w32.o \
		  jdcoefct.w32.o jdcolor.w32.o jddctmgr.w32.o jdhuff.w32.o jdinput.w32.o jdmainct.w32.o jdmarker.w32.o \
		  jdmaster.w32.o jdmerge.w32.o jdphuff.w32.o jdpostct.w32.o jdsample.w32.o jdtrans.w32.o jerror.w32.o \
		  jfdctflt.w32.o jfdctfst.w32.o jfdctint.w32.o jidctflt.w32.o jidctfst.w32.o jidctint.w32.o \
		  jidctred.w32.o jmemmgr.w32.o jmemnobs.w32.o jquant1.w32.o jquant2.w32.o jutils.w32.o
	liba = ../w32/libjpeg.a
else
	obj = jcapimin.o jcapistd.o jccoefct.o jccolor.o jcdctmgr.o jchuff.o jcinit.o \
		  jcmainct.o jcmarker.o jcmaster.o jcomapi.o jcparam.o jcphuff.o jcprepct.o \
		  jcsample.o jctrans.o jdapimin.o jdapistd.o jdatadst.o jdatasrc.o \
		  jdcoefct.o jdcolor.o jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o \
		  jdmaster.o jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o \
		  jfdctflt.o jfdctfst.o jfdctint.o jidctflt.o jidctfst.o jidctint.o \
		  jidctred.o jmemmgr.o jmemnobs.o jquant1.o jquant2.o jutils.o
	liba = ../libjpeg.a
endif

CFLAGS = -O3

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(obj) $(liba)
