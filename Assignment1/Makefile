CXX = g++
CXXFLAGS = -g -Wall -Werror -Wpedantic
LDFLAGS = -pthread  # Required for Google Test

# Google Test installation paths
GTEST_LIB = /usr/local/lib
GTEST_INCLUDE = /usr/local/include

# Source files
SRCS = test.cpp 
OBJS = $(SRCS:.cpp=.o)
TARGET = test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE) $^ -o $@ -L$(GTEST_LIB) -lgtest -lgtest_main $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)


format:
	astyle -A1 -s4 *.cpp *.hpp
