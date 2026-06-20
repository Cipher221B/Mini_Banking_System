CXX = g++

CXXFLAGS = -std=gnu++20 \
           -Wall \
           -Wextra \
           -Isrc \
           -I.. \
           -I/mingw64/include

LIBS = -L/mingw64/lib -lodbc32 \
      -lssl \
      -lcrypto \

TARGET = application.exe

APPLICATION_SRC = $(wildcard src/*.cpp) \
                  $(wildcard src/entity/*.cpp) \
                  $(wildcard src/logging/*.cpp) \
                  $(wildcard src/repository/*.cpp) \
                  $(wildcard src/security/*.cpp) \
                  $(wildcard src/service/*.cpp) \
                  $(wildcard src/utils/*.cpp)



SHARED_SRC = $(wildcard ../Shared/database/*.cpp) \
             $(wildcard ../Shared/utils/*.cpp) \
             $(wildcard ../Shared/logging/*.cpp)

SRC = $(APPLICATION_SRC) $(SHARED_SRC)

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