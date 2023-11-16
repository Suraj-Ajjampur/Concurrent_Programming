# Define the compiler and compiler flags, added -fopenmp for OpenMP
CC = g++
CFLAGS = -O0 -std=c++2a -g -fopenmp

# Define the target executable name
TARGET = mysort

# Define the source files for your program
SOURCES = main.cpp mergesort.cpp quicksort.cpp

# Define the object files to be generated
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule to build the executable
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Rule to compile individual source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJECTS) $(TARGET)
