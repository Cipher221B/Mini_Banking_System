CXX = g++

CXXFLAGS = -std=gnu++17 \
           -Wall \
           -Wextra \
           -I. \
           -I.. \
           -I/mingw64/include

LIBS = -L/mingw64/lib \
       -lodbc32

TARGET = collector.exe

COLLECTOR_SRC = $(wildcard *.cpp) \
                $(wildcard repository/*.cpp) \
                $(wildcard service/*.cpp) \
                $(wildcard logging/*.cpp) \
                $(wildcard normalize/*.cpp) \
                $(wildcard signal/*.cpp) 

SHARED_SRC = $(wildcard ../Shared/database/*.cpp) \
             $(wildcard ../Shared/utils/*.cpp) \
             $(wildcard ../Shared/logging/*.cpp)

SRC = $(COLLECTOR_SRC) $(SHARED_SRC)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean