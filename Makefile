
MIPS_DNAME = /home/amv/trash/mips/
BASE_DNAME = $(MIPS_DNAME)/CodeSourcery/Sourcery_CodeBench_Lite_for_MIPS_GNU_Linux/
BIN_BASE = $(BASE_DNAME)/bin
SDK_BASE = $(MIPS_DNAME)/work/
BROOT = $(SDK_BASE)/build_root

CC = $(BIN_BASE)/mips-linux-gnu-gcc
CXXC = $(BIN_BASE)/mips-linux-gnu-g++
LD = $(BIN_BASE)/mips-linux-gnu-gcc
AR = $(BIN_BASE)/mips-linux-gnu-ar

LIBS = -lm
CFLAGS = -Wall -march=xlp -O3 -mplt -std=c++11 -shared -fPIC
LDFLAGS = -L$(BROOT)/usr/lib/ -lamv -liconv
INCLUDE_DIRS = \
	-Wno-poison-system-directories\
	-I. -I$(BROOT)/usr/include/ -I$(BROOT)/usr/include/libdrm/
SRC = \
	lib/sxf.cpp lib/sxf_map.cpp lib/sxf_height_map.cpp lib/file.cpp lib/codepage.cpp lib/map.cpp lib/point.cpp

OUTPUT_DIR = $(BROOT)/usr/lib/
OUTPUT_FNAME = ${OUTPUT_DIR}/libsxf.so

release: clean

	$(CXXC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC) $(LIBS) $(LDFLAGS) -o $(OUTPUT_FNAME)

clean:

	rm -f $(OUTPUT_FNAME)

