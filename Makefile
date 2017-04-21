# Yash Bhutwala
# CS 315 Lab 12
# 12/1/2015

# compiler
CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG
LFLAGS = -lpthread

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

EXECS = gshell

all: $(EXECS)

$(OBJ)/wrappers.o: wrappers.c
	$(CC) -c $(CFLAGS) $(SRC)/wrappers.c -o $(OBJ)/wrappers.o

gshell: $(SRC)/gshell.c $(OBJ)/wrappers.o
	$(CC) $(CFLAGS) $(LFLAGS) $(SRC)/gshell.c $(OBJ)/wrappers.o -o $(BIN)/gshell

.PHONY: clean
clean:
	/bin/rm -rf $(BIN)/* $(OBJ)/* core* *~
