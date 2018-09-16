CC = gcc
OBJS = Game.o Moves.o Backtracking.o Parser.o Solver.o MainAux.o GUI.o ILP.o main.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors -g
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Game.h Parser.h Solver.h MainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h 
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
MainAux.o: MainAux.c MainAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
GUI.o: GUI.c GUI.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Solver.o: Solver.c Solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Moves.o: Moves.c Moves.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Backtracking.o: Backtracking.c Backtracking.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
ILP.o: ILP.c ILP.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
all: $(EXEC)
