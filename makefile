TARGET := CorthCOMP.exe

CC := gcc
CFLAGS := -ggdb -Wall -Werror -Wextra -Wpedantic

all: $(TARGET)

$(TARGET): CorthCOMP.o labels.o optimize.o syntax_checker.o codgen.o tokenizing.o asmgen.o preprocessor.o lexical_analysys.o utils.o ast_builder.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ -c -o $@ $(CFLAGS)

.PHONY: clear

clear:
	rm -rf *.o $(TARGET)
	
