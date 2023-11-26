# project_root/
# |-- src/
# |   |-- module1/
# |   |   |-- file1.c
# |   |   |-- file2.c
# |   |-- module2/
# |   |   |-- file3.c
# |   |   |-- file4.c
# |-- include/
# |   |-- module1/
# |   |   |-- header1.h
# |   |-- module2/
# |   |   |-- header2.h
# |-- bin/
# |   |-- executable
# |-- Makefile

CC = gcc
CFLAGS = -Wall -I ../include

# 使用shell命令获取当前目录的绝对路径
CURRENT_PATH := $(shell pwd)

# 使用shell命令获取上一级目录的路径
MEDIA_PATH := $(shell dirname $(CURRENT_PATH))

SRC_DIR = $(MEDIA_PATH)/src
OBJ_DIR = $(MEDIA_PATH)/src
BIN_DIR = $(MEDIA_PATH)/bin

# 获取所有的.c文件
SRC = $(wildcard $(SRC_DIR)/*.c)
# 根据.c文件生成对应的.o文件
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# 可执行文件
TARGET = $(BIN_DIR)/executable

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking $^ to generate $@"
	$(CC) $(CFLAGS) -o $@ $^

# 创建目标文件夹
#$(OBJ):
#	mkdir -p $(@D)

# 根据.c文件生成对应的.o文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@echo "Compiling $< to generate $@"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*


