# Enable this to use the virtual LED sign
DEFINES+=-D USE_VIRTUAL_CANVAS

CC := clang++
SRCDIR := src
BUILDDIR := build
TARGET := bin/ledsign

# RPI RGB Library
RPI_RGB_DIR=lib/rpi-rgb-led-matrix
RPI_RGB_INCDIR=$(RPI_RGB_DIR)/include
RPI_RGB_LIBDIR=$(RPI_RGB_DIR)/lib
RPI_RGB_LIBRARY_NAME=rgbmatrix
RPI_RGB_LIBRARY=$(RPI_RGB_LIBDIR)/lib$(RPI_RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RPI_RGB_LIBDIR) -l$(RPI_RGB_LIBRARY_NAME) -lrt -lm -lpthread

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -g $(DEFINES) # -Wall
LIB := -lm -lstdc++ -lpthread -lSDL2
INC := -I include -I$(RPI_RGB_INCDIR)

# ImageMagick
MAGICK_CXXFLAGS=`GraphicsMagick++-config --cppflags --cxxflags`
MAGICK_LDFLAGS=`GraphicsMagick++-config --ldflags --libs`

$(TARGET): $(OBJECTS) $(RPI_RGB_LIBRARY)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB) $(MAGICK_LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) $(MAGICK_CXXFLAGS) -c -o $@ $<

$(RPI_RGB_LIBRARY): FORCE
	$(MAKE) -C $(RPI_RGB_LIBDIR)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

FORCE:
.PHONY: FORCE
