CC = gcc

CFLAGS = -Wall -Wextra -Isrc/interpreter

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/storm.exe

SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard src/interpreter/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ -mconsole

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir $(subst /,\,$(dir $@)) 2>nul || rem 
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir $(subst /,\,$@) 2>nul || rem

.PHONY: clean run

clean:
	del /Q /F $(OBJ_DIR)\*.o 2>nul
	del /Q /F $(TARGET) 2>nul
	rmdir /S /Q  $(OBJ_DIR) 2>nul

run: $(TARGET)
	$(TARGET)