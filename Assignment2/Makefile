# Compiler settings
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Werror -Wpedantic -I/usr/include/eigen3 -Iexternals/lazycsv/include
LDFLAGS = -lgtest -lgtest_main -lpthread

# Targets
TARGET = eigen_app
TEST_TARGET = eigen_test

# Source files
MAIN_SRCS = main.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

TEST_SRCS = test.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) 

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(MAIN_OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

format:
	astyle -A1 -s4 *.cpp *.hpp

.PHONY: all run test clean format
