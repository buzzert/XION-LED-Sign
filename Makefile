# CC := clang
CC := g++
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
CFLAGS := -std=c++11 -g # -Wall
LIB := -lm -lstdc++ -lpthread -lSDL2
INC := -I include -I$(RPI_RGB_INCDIR)

$(TARGET): $(OBJECTS) $(RPI_RGB_LIBRARY)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(RPI_RGB_LIBRARY): FORCE
	$(MAKE) -C $(RPI_RGB_LIBDIR)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

FORCE:
.PHONY: FORCE
