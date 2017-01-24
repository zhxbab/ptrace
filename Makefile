CC = gcc
OBJ32 = auxiliary32.o  ex_string32.o
OBJ64 = auxiliary64.o  ex_string64.o
CFLAG = -Wall

ptrace64:${OBJ64}
	${CC} ${CFLAG} -o ptrace64 ptrace.c ${OBJ64}
	
ptrace32:${OBJ32}
	${CC} ${CFLAG} -m32 -o ptrace32 ptrace.c ${OBJ32}
	
auxiliary32.o: auxiliary.h
	${CC} ${CFLAG} -c auxiliary.c -m32 -o auxiliary32.o
	
auxiliary64.o: auxiliary.h
	${CC} ${CFLAG} -c auxiliary.c -o auxiliary64.o
	
ex_string64.o: ex_string.h
	${CC} ${CFLAG} -c ex_string.c -o ex_string64.o

ex_string32.o: ex_string.h
	${CC} ${CFLAG} -c ex_string.c -m32 -o ex_string32.o
	
.PHONY: all
all: ptrace64 ptrace32

.PHONY: clean
clean:
	rm -f ptrace32 ptrace64 *.o