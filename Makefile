CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# Name of the final executable
TARGET := mathbench

# Source files (add more .cpp files here later if you want)
SRCS := main.cpp MathBench.cpp

# Object files (derived from sources)
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generic rule to build any .o from .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
