# --------------------------------------------------------
# Makefile for building iProcessor
# --------------------------------------------------------

# Compiler, tools and options
CC        = clang
CXX       = clang++
CFLAGS    = -pipe -Wall -W 
CXXFLAGS  = -pipe -Wall -W -std=c++17
INCPATH   = -I.
LINK      = clang++
LDFLAGS   = 
LIBPATH   = 
DEL_FILE  = rm -f
THISFILE  = Makefile

# Output directory
OBJECTS_DIR = ./

# Files
HNAMES = \
	gaussian \
	graph \
	image \
	item \
	main \
	pnm \
	point \
	script \
	shape \

SNAMES = \
	ccd \
	coalesce \
	derivative \
	drawpolygon \
	drawShape \
	dumproi \
	fillShape \
	filter \
	flow \
	graph \
	image \
	item \
	loadimage \
	main \
	math \
	morphology \
	overlay \
	pnm \
	point \
	prune \
	readDB \
	reconstruct \
	saveimage \
	script \
	scriptImage \
	scriptInit \
	scriptLang \
	shape \
	utility \
	uvdec \
	writeDB \

SOURCES = $(addsuffix .cpp, $(SNAMES))
HEADERS = $(addsuffix .h, $(HNAMES))
OBJECTS = $(addsuffix .o, $(SNAMES))
TARGET  = iProcessor

# Implicit rules

.SUFFIXES: .c .cpp

.cpp.o: $(THISFILE) 
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CXXFLAGS) $(OTHER_CFLAGS) $(INCPATH) -o $$@ $<

# Build rules

all: $(TARGET) 

debug: OPTIMIZATION_CFLAGS = -O0
debug: OTHER_CFLAGS = -g
debug: $(TARGET) 

release: OPTIMIZATION_CFLAGS = -O3
release: OTHER_CFLAGS =
release: $(TARGET) 

$(TARGET):  $(OBJECTS) 
	$(LINK) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBPATH) $(LIBS) 

%.o: %.cpp $(THISFILE) $(HEADERS)
	$(CXX) -c $(OPTIMIZATION_CFLAGS) $(CXXFLAGS) $(OTHER_CFLAGS) $(INCPATH) $< 

.PHONY: clean
clean:
	-$(DEL_FILE) $(OBJECTS) $(TARGET)
	-$(DEL_FILE) *~ core *.core
