# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Icrow/include -Idata -Ibusiness

# Linker flags
LDFLAGS = -lmysqlclient -lpthread

# Directories
DATA_DIR = data
BUSINESS_DIR = business
SERVICE_DIR = service
CLIENT_DIR = client

# Source files
DATA_SRC = $(DATA_DIR)/ChatRepository.cpp
BUSINESS_SRC = $(BUSINESS_DIR)/ChatService.cpp
SERVER_SRC = $(SERVICE_DIR)/server.cpp
CLIENT_SRC = $(CLIENT_DIR)/client.cpp

# Object files
DATA_OBJ = $(DATA_SRC:.cpp=.o)
BUSINESS_OBJ = $(BUSINESS_SRC:.cpp=.o)
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

# Targets
SERVER_TARGET = mychat-server
CLIENT_TARGET = mychat-client

# Default target
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Server build
$(SERVER_TARGET): $(DATA_OBJ) $(BUSINESS_OBJ) $(SERVER_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Client build
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $^ -o $@ -lpthread

# Compile rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(DATA_OBJ) $(BUSINESS_OBJ) $(SERVER_OBJ) $(CLIENT_OBJ)
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)

# Run server
run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Run client
run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)
