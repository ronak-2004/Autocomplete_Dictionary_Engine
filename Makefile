CXX = g++
CXXFLAGS = -std=c++17 -O2

SRC = src/autocomplete.cpp
TARGET = autocomplete

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o  $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)