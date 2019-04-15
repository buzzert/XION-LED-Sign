# Debugging
# DEBUG=1

# Enable this to use the virtual LED sign
USE_VIRTUAL_CANVAS=1

ifeq ($(USE_VIRTUAL_CANVAS), 1)
    DEFINES+=-D USE_VIRTUAL_CANVAS
endif

# NOTE! When compiling on Raspberry-pi, use the g++-4.7 compiler
#       for compatibility with rpi library
CC := clang++
SRCDIR := src
BUILDDIR := build
RESOURCEDIR := resources

TARGET := bin/ledsign
TARGET_RESOURCES := bin/resources

RESOURCES_DIR := $(realpath $(TARGET_RESOURCES))
DEFINES+=-D RESOURCES_DIR=\"$(RESOURCES_DIR)/\"

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
LIB := -lm -lstdc++ -std=c++11 -lpthread

OS := $(shell uname)
ifeq ($(OS),Linux)
    LIB += -lrt
	CC := g++
endif

ifeq ($(USE_VIRTUAL_CANVAS), 1)
LIB+= -lSDL2
endif

INC := -I include -I$(RPI_RGB_INCDIR)

# ImageMagick
CFLAGS+=`pkg-config --cflags Magick++`
LDFLAGS+=`pkg-config --libs Magick++`

$(TARGET): $(OBJECTS) $(RPI_RGB_LIBRARY) $(TARGET_RESOURCES)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LDFLAGS) $(LIB)"; $(CC) $(OBJECTS) $(RPI_RGB_LIBRARY) $(LDFLAGS) -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(RPI_RGB_LIBRARY): FORCE
	$(MAKE) -C $(RPI_RGB_LIBDIR)

$(TARGET_RESOURCES): FORCE
	@echo " Copying resources..."
	@mkdir -p $(TARGET_RESOURCES)
	@cp -f $(RESOURCEDIR)/* $(TARGET_RESOURCES) 2> /dev/null || :
	@echo " Copying fonts..."
	@mkdir -p $(TARGET_RESOURCES)/fonts
	@cp -f $(RPI_RGB_DIR)/fonts/* $(TARGET_RESOURCES)/fonts


clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

FORCE:
.PHONY: FORCE
