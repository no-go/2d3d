MAKE_OPTS = $(shell expr 1 + $(shell grep -c "^processor" </proc/cpuinfo))
CFLAGS=-march=native -pipe -O2 -Wall
LDFLAGS=-I/usr/include/opencv -lm -lopencv_core -lopencv_imgproc -lopencv_highgui -lGL -lGLU -lglut -std=gnu++11
BIN=main

all:
	$(MAKE) -s -j$(MAKE_OPTS) real_all

real_all: $(BIN).cpp
	$(LINK.cc) $(BIN).cpp $(CFLAGS) -o $(BIN)

clean:
	$(RM) $(BIN)
