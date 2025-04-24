# Compiler settings
CXX = g++
CXXFLAGS = -g -Wall -Werror -Wpedantic -I/usr/include/eigen3 -Ilazycsv
LDFLAGS = -lgtest -lgtest_main -lpthread  # Add gtest and pthread flags

# Source files
SRCS = test.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = eigen_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

format:
	astyle -A1 -s4 *.cpp *.hpp
