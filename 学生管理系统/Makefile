CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/student.c \
       $(SRC_DIR)/ui.c \
       $(SRC_DIR)/file_io.c \
       $(SRC_DIR)/stats.c

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/grade_system

.PHONY: all clean run dirs

all: dirs $(TARGET)

dirs:
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(DATA_DIR)" mkdir "$(DATA_DIR)"

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(TARGET)

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
