CXX=clang++
CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -pg  # Added -pg flag for profiling
LDFLAGS=-pg  # Added -pg flag for linking with profiling
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES=exe5.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all run tidy clean profile

all: exe5

run: exe5
	./exe5

exe5: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tidy:
	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

profile: exe5
	./exe5
	gprof exe5 gmon.out > analysis.txt

clean:
	rm -f *.o exe5 gmon.out analysis.txt
