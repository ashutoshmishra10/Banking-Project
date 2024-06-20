CC = gcc
CFLAGS = -Wall -Icommon
LDFLAGS = -pthread

# Directories
COMMON_DIR = common
SERVER_DIR = server
CLIENT_DIR = client
BUILD_DIR = build

# Source files
COMMON_SRC = $(COMMON_DIR)/bank.h $(COMMON_DIR)/customer.c
SERVER_SRC = $(SERVER_DIR)/main.c $(SERVER_DIR)/customer.c
CLIENT_SRC = $(CLIENT_DIR)/socket_client.c $(CLIENT_DIR)/customer.c

# Object files
COMMON_OBJ = $(BUILD_DIR)/customer.o
SERVER_OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/customer_server.o
CLIENT_OBJ = $(BUILD_DIR)/socket_client.o $(BUILD_DIR)/customer_client.o

# Targets
SERVER_TARGET = server
CLIENT_TARGET = client

# Create build directory if not exists
$(shell mkdir -p $(BUILD_DIR))

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(COMMON_OBJ) $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(CLIENT_TARGET): $(COMMON_OBJ) $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/customer.o: $(COMMON_DIR)/customer.c $(COMMON_DIR)/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(SERVER_DIR)/main.c $(COMMON_DIR)/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/customer_server.o: $(SERVER_DIR)/customer.c $(COMMON_DIR)/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/socket_client.o: $(CLIENT_DIR)/socket_client.c $(COMMON_DIR)/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/customer_client.o: $(CLIENT_DIR)/customer.c $(COMMON_DIR)/bank.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(SERVER_TARGET) $(CLIENT_TARGET)

.PHONY: all clean
