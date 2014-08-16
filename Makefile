
MIPS_DNAME = /home/amv/trash/mips/
BASE_DNAME = $(MIPS_DNAME)/CodeSourcery/Sourcery_CodeBench_Lite_for_MIPS_GNU_Linux/
BIN_BASE = $(BASE_DNAME)/bin
SDK_BASE = $(MIPS_DNAME)/work/
BROOT = $(SDK_BASE)/build_root

CC = $(BIN_BASE)/mips-linux-gnu-gcc
CXXC = $(BIN_BASE)/mips-linux-gnu-g++
LD = $(BIN_BASE)/mips-linux-gnu-gcc
AR = $(BIN_BASE)/mips-linux-gnu-ar

CFLAGS = -Wall -march=xlp -O3 -mplt -std=c++11 -shared -fPIC
LDFLAGS = -L$(BROOT)/usr/lib/ -lm -lamv -liconv
INCLUDE_DIRS = \
	-Wno-poison-system-directories\
	-I. -I$(BROOT)/usr/include/ -I$(BROOT)/usr/include/libdrm/
SRC = \
	lib/sxf.cpp lib/sxf_map.cpp lib/sxf_height_map.cpp lib/file.cpp lib/codepage.cpp lib/map.cpp lib/point.cpp

PREFIX = $(BROOT)/usr/
BUILD_DIR = build/
OUTPUT_FNAME = libsxf.so
INCLUDES_TO_INSTALL = file.hpp map.hpp point.hpp sxf.hpp

release: clean

	mkdir -p $(BUILD_DIR)

	$(CXXC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC) $(LDFLAGS) -o $(BUILD_DIR)/$(OUTPUT_FNAME)

install: release

	cp $(BUILD_DIR)/$(OUTPUT_FNAME) $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/sxf
	cp $(INCLUDES_TO_INSTALL) $(PREFIX)/include/sxf

clean:

	rm -f $(OUTPUT_FNAME)

