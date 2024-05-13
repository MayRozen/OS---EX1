# ------------------------------exe1------------------------------
CXX=clang++
CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g  # Added -g flag for debugging symbols
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

SOURCES=exe1.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all run tidy clean

all: exe1

run: exe1
	./exe1

exe1: exe1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

tidy:
	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o exe1

# Target to open core file in gdb
core: exe1
	gdb ./exe1 core


# ------------------------------exe2------------------------------
# CXX=clang++
# CXXFLAGS=-std=c++11 -Werror -Wsign-conversion  # Removed -lm flag
# VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

# SOURCES=exe2.cpp
# OBJECTS=$(SOURCES:.cpp=.o)

# .PHONY: all run tidy clean

# all: exe2

# run: exe2
# 	./exe2

# exe2: exe2.o $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# tidy:
# 	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o exe2


# ------------------------------exe3------------------------------
# CXX=clang++
# CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g # lm = debugging flags
# VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

# SOURCES=exe3.cpp exe2.cpp
# OBJECTS=$(SOURCES:.cpp=.o)
# LIBRARY=libpoisson.so  # dynamic library

# .PHONY: all run tidy clean

# all: exe3

# run: exe3
# 	./exe3

# exe3: exe3.o $(OBJECTS) $(LIBRARY)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# $(LIBRARY): $(OBJECTS)
# 	$(CXX) -shared -o $@ $^

# tidy:
# 	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o exe3 $(LIBRARY)