APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

all: libed

libed: 
	gcc -Wall -Werror -Wpedantic -O3 -march=native $(APPS)/main.c -o $(BIN)/main

run:
	$(BIN)/main