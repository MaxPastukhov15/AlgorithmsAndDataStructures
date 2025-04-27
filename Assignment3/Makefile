# Compiler settings
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Werror -Wpedantic 
LDFLAGS = -lgtest -lgtest_main -lpthread

# Targets
TARGET = cd_shannon
TEST_TARGET = shannon_test

# Source files
MAIN_SRCS = Shannon.cpp main.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

TEST_SRCS = Shannon.cpp test.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) input.csv output.csv

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(MAIN_OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

format:
	astyle -A1 -s4 *.cpp *.hpp

.PHONY: all run test clean format
