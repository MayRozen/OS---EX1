# ------------------------------exe1------------------------------
# CXX=clang++
# CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g  # Added -g flag for debugging symbols
# VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

# SOURCES=exe1.cpp
# OBJECTS=$(SOURCES:.cpp=.o)

# .PHONY: all run tidy clean

# all: exe1

# run: exe1
# 	./exe1

# exe1: exe1.o $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# tidy:
# 	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o exe1

# # Target to open core file in gdb
# core: exe1
# 	gdb ./exe1 core


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
# # Define the source files
# SOURCES_EXE3 = exe3.cpp
# SOURCES_LIB = exe2.hpp

# # Define the object files
# OBJECTS_EXE3 = $(SOURCES_EXE3:.cpp=.o)
# OBJECTS_LIB = $(SOURCES_LIB:.hpp=.o)

# # Define the shared library name
# LIBRARY = libpoisson.so

# # Compiler and flags
# CXX = clang++
# CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g -fPIC
# LDFLAGS = -ldl

# .PHONY: all run tidy clean

# # Default target
# all: exe3

# # Run target
# run: exe3
# 	./exe3

# # Executable target
# exe3: $(OBJECTS_EXE3) $(LIBRARY)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) -Wl,-rpath,.

# # Shared library target
# $(LIBRARY): $(OBJECTS_LIB)
# 	$(CXX) -shared -o $@ $^

# # Clang-tidy target
# tidy:
# 	clang-tidy $(SOURCES_EXE3) $(SOURCES_LIB) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-*

# # Object file compilation
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean target
# clean:
# 	rm -f *.o exe3 $(LIBRARY)

# ------------------------------exe4------------------------------
# CXX=clang++
# CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -g # lm = debugging flags
# VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

# SOURCES=exe4.cpp 
# OBJECTS=$(SOURCES:.cpp=.o)

# .PHONY: all run tidy clean

# all: exe4

# run: exe4
# 	./exe4

# exe4: exe4.o $(OBJECTS) $(LIBRARY)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# $(LIBRARY): $(OBJECTS)
# 	$(CXX) -shared -o $@ $^

# tidy:
# 	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o exe4 $(LIBRARY)
# ------------------------------exe5------------------------------
# CXX=clang++
# CXXFLAGS=-std=c++11 -Werror -Wsign-conversion -pg  # Added -pg flag for profiling
# LDFLAGS=-pg  # Added -pg flag for linking with profiling
# VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all --error-exitcode=99

# SOURCES=exe5.cpp
# OBJECTS=$(SOURCES:.cpp=.o)

# .PHONY: all run tidy clean profile

# all: exe5

# run: exe5
# 	./exe5

# exe5: $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# tidy:
# 	clang-tidy $(SOURCES) $(HEADERS) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --

# %.o: %.cpp $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# profile: exe5
# 	./exe5
# 	gprof exe5 gmon.out > analysis.txt

# clean:
# 	rm -f *.o exe5 gmon.out analysis.txt


# ------------------------------exe6------------------------------

CXX = clang++
CXXFLAGS = -std=c++11 -Werror -Wsign-conversion -g
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

SOURCES = exe6.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = exe6

.PHONY: all clean valgrind

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

valgrind: $(EXECUTABLE)
	valgrind $(VALGRIND_FLAGS) ./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
