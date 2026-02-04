CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS = -lmariadb

SRC_DIR = src
OBJ_DIR = build
TARGET = chat_frontend

SRC = $(SRC_DIR)/main.cpp
OBJ = $(OBJ_DIR)/main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

