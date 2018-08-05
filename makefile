CC = gcc
OBJS = Game.o Parser.o Solver.o MainAux.o GUI.o main.o
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors -g

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c Game.h Parser.h Solver.h MainAux.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h 
	$(CC) $(COMP_FLAG) -c $*.c
MainAux.o: MainAux.c MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAG) -c $*.c 
GUI.o: GUI.c GUI.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c Solver.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
