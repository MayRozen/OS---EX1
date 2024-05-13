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

# ------------------------------exe4------------------------------
CXX=clang++
CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g # lm = debugging flags
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

SOURCES=exe4.cpp 
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all run tidy clean

all: exe4

run: exe4
	./exe4

exe4: exe4.o $(OBJECTS) $(LIBRARY)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(LIBRARY): $(OBJECTS)
	$(CXX) -shared -o $@ $^

tidy:
	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o exe4 $(LIBRARY)
