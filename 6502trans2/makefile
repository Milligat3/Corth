TARGET := CorthCOMP.exe

CC := gcc
CFLAGS := -ggdb -Wall -Werror -Wextra -Wpedantic

all: $(TARGET)

$(TARGET): CorthCOMP.o labels.o optimize.o syntax_checker.o codgen.o tokenizing.o asmgen.o preprocessor.o lexical_analysys.o
	$(CC) $^ -o $@

CorthCOMP.o: CorthCOMP.c optimize.h syntax_checker.h codgen.h tokenizing.h asmgen.h
	$(CC) $< -c -o $@ $(CFLAGS)

optimize.o: optimize.c optimize.h tokenizing.h
	$(CC) $< -c -o $@ $(CFLAGS)

syntax_checker.o: syntax_checker.c syntax_checker.h tokenizing.h asmgen.h 
	$(CC) $< -c -o $@ $(CFLAGS)

codgen.o: codgen.c codgen.h tokenizing.h asmgen.h labels.h
	$(CC) $< -c -o $@ $(CFLAGS)

tokenizing.o: tokenizing.c asmgen.h
	$(CC) $< -c -o $@ $(CFLAGS)

asmgen.o: asmgen.c asmgen.h
	$(CC) $< -c -o $@ $(CFLAGS)

labels.o: labels.c tokenizing.h labels.h
	$(CC) $< -c -o $@ $(CFLAGS)

preprocessor.o: preprocessor.c preprocessor.h tokenizing.h
	$(CC) $< -c -o $@ $(CFLAGS)

lexical_analysys.o: lexical_analysys.c lexical_analysys.h tokenizing.h
	$(CC) $< -c -o $@ $(CFLAGS)
clean:
	rm -rf *.o $(TARGET)
	
