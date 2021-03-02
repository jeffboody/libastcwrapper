export CC_USE_MATH = 1

TARGET   = jpg2astc
CLASSES  =
SOURCE   = $(TARGET).c $(CLASSES:%=%.c)
OBJECTS  = $(TARGET).o $(CLASSES:%=%.o)
HFILES   = $(CLASSES:%=%.h)
OPT      = -O2 -Wall
CFLAGS   = $(OPT) -I.
LDFLAGS  = -Llibcc -lcc -Ltexgz -ltexgz -Llibastcwrapper -lastcwrapper -Llibastcenc -lastcenc-avx2-static -lstdc++ -ljpeg -lz -lm
CCC      = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS) libcc texgz libastcwrapper
	$(CCC) $(OPT) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: libcc texgz libastcwrapper

libcc:
	$(MAKE) -C libcc

texgz:
	$(MAKE) -C texgz

libastcwrapper:
	$(MAKE) -C libastcwrapper

clean:
	$(MAKE) -C libcc clean
	$(MAKE) -C texgz clean
	$(MAKE) -C libastcwrapper clean
	rm -f $(OBJECTS) *~ \#*\# $(TARGET)

$(OBJECTS): $(HFILES)
