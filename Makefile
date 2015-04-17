CXX = $(shell wx-config --cxx)
CXX_FLAGS = $(shell wx-config --cxxflags) -I3rd_party/ -I/usr/include/freetype2
WX_LIBS = $(shell wx-config --libs) $(shell wx-config --gl-libs)
PROGRAM = pygmy
LIBS = -lGL -lglut -lGLU -lboost_signals -lpng -lboost_system -lboost_filesystem -lftgl
 
SRC := $(wildcard 3rd_party/Image/*cpp src/*/*cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SRC))
 
 # implementation
 
 .SUFFIXES:      .o .cpp
 
%.o : %.cpp
	$(CXX) -c $(CXX_FLAGS) -o $@ $<
 
all: $(PROGRAM)

3rd_party/lib/libticppd.a:
	premake --target gnu 3rd_party/ticpp/premake.lua
	$(MAKE) -C 3rd_party/ticpp/
 
$(PROGRAM): $(OBJECTS) 3rd_party/lib/libticppd.a
	$(CXX) -o $(PROGRAM) $(OBJECTS) 3rd_party/lib/libticppd.a $(WX_LIBS) $(LIBS)
 
clean:
	rm -f $(OBJECTS) $(PROGRAM)
