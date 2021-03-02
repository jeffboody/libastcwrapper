TARGET   = libastcwrapper.a
CLASSES  = astc_wrapper astc_header
SOURCE   = $(CLASSES:%=%.cpp)
OBJECTS  = $(SOURCE:.cpp=.o)
HFILES   = $(CLASSES:%=%.h)
OPT      = -O2 -Wall
CPPFLAGS = $(OPT) -I.
LDFLAGS  =
AR       = ar

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) *~ \#*\# $(TARGET)

$(OBJECTS): $(HFILES)
