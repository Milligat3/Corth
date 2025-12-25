TARGET := CortCOMP.exe

CC := gcc
CFLAGS := -ggdb -Wall -Werror -Wextra -Wpedantic -std=c89

all: $(TARGET)

$(TARGET): CortCOMP.o labels.o optimize.o synt.o codgen.o token.o asmgen.o prep.o lexi.o utils.o ast.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ -c -o $@ $(CFLAGS)

.PHONY: clear

clear:
	rm -rf *.o $(TARGET)
	
