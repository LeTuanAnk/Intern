# CC = /home/alt/Downloads/buildroot-2015.05/output/host/usr/bin/mips-buildroot-linux-uclibc-gcc
# CFLAGS = -g -std=c99 -D_POSIX_C_SOURCE=200809L -I./lib

CC := gcc -D_POSIX_C_SOURCE=200809L
CFLAGS := -g -std=c99 -Wall


PLUS_DIR := .

BIN_DIR := $(PLUS_DIR)/bin
OBJ_DIR := $(PLUS_DIR)/obj
LIB_DIR := $(PLUS_DIR)/lib
SRC_DIR := $(PLUS_DIR)/src
DATA_DIR := $(PLUS_DIR)/database

# SRC_FILES_APP := $(SRC_DIR)/main.c $(SRC_DIR)/parser_option.c $(SRC_DIR)/create_mail_from_name.c $(SRC_DIR)/file.c $(SRC_DIR)/process_file.c $(SRC_DIR)/log.c
# SRC_FILES_GET_INFO := $(SRC_DIR)/get_interface.c $(SRC_DIR)/main.c $(SRC_DIR)/log.c $(SRC_DIR)/parser_option.c


SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
# OBJ_FILES_APP := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES_APP))
# OBJ_FILES_GET_INFO := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES_GET_INFO))

all: $(OBJ_DIR) $(BIN_DIR) $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(BIN_DIR)/get_info -lm
# all: $(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/app $(BIN_DIR)/get_info

# $(BIN_DIR)/app: $(OBJ_FILES_APP)
# 	$(CC) $(CFLAGS) $^ -o $@

# $(BIN_DIR)/get_info: $(OBJ_FILES_GET_INFO)
# 	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LIB_DIR)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*
	