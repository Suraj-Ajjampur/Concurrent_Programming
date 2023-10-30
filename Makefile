# Define the compiler and compiler flags
CC = g++
CFLAGS = -pthread -O3 -std=c++2a -g

# Define the target executables
MYSORT_TARGET = mysort
COUNTER_TARGET = counter

# Define the source files for mysort and counter
MYSORT_SOURCES = main_mysort.cpp bucket_sort.cpp my_atomics.cpp
COUNTER_SOURCES = main_counter.cpp my_atomics.cpp

# Define the object files for mysort and counter
MYSORT_OBJECTS = $(MYSORT_SOURCES:.cpp=.o)
COUNTER_OBJECTS = $(COUNTER_SOURCES:.cpp=.o)

# Default rule to build all executables
all: $(MYSORT_TARGET) $(COUNTER_TARGET)

# Rule to build mysort executable
$(MYSORT_TARGET): $(MYSORT_OBJECTS)
	$(CC) $(CFLAGS) $(MYSORT_OBJECTS) -o $(MYSORT_TARGET)

# Rule to build counter executable
$(COUNTER_TARGET): $(COUNTER_OBJECTS)
	$(CC) $(CFLAGS) $(COUNTER_OBJECTS) -o $(COUNTER_TARGET)

# Rule to compile individual source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(MYSORT_OBJECTS) $(COUNTER_OBJECTS) $(MYSORT_TARGET) $(COUNTER_TARGET)
