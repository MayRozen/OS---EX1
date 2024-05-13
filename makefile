SOURCES=exe3.cpp  # Remove exe2.cpp from SOURCES
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libpoisson.so  # dynamic library

CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g -fPIC  # Add -fPIC flag here

.PHONY: all run tidy clean

all: exe3

run: exe3
	./exe3

exe3: $(OBJECTS) $(LIBRARY)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(LIBRARY): $(OBJECTS)
	$(CXX) -shared $^ -o $@

tidy:
	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o exe3 $(LIBRARY)
