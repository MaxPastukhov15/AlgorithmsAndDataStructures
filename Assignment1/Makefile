CXX = g++
CXXFLAGS = -g -Wall -Werror -Wpedantic -I.
LDFLAGS = -pthread -lgtest -lgtest_main

# Source files
SRCS = test.cpp 
OBJS = $(SRCS:.cpp=.o)
TARGET = test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

format:
	astyle -A1 -s4 *.cpp *.hpp
