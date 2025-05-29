CC = cc
CFLAGS = -Wall -O2 -Iinclude
LDFLAGS =

OBJ_DIR = build
BIN_DIR = bin

OBJECTS = $(OBJ_DIR)/carbon.o $(OBJ_DIR)/measure.o $(OBJ_DIR)/main.o
TARGET = $(BIN_DIR)/carbon

TEST_BIN = $(BIN_DIR)/test_co2

.PHONY: all clean test_co2

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	test -d $(OBJ_DIR) || mkdir $(OBJ_DIR)

$(BIN_DIR):
	test -d $(BIN_DIR) || mkdir $(BIN_DIR)

test_co2: $(TEST_BIN)

$(TEST_BIN): tests/test_co2.c $(OBJ_DIR)/carbon.o $(OBJ_DIR)/measure.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

