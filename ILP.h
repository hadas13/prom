/*
 * a header file that represents the methods of the Gurobi ILP optimizer
 */

#ifndef ILP_H_
#define ILP_H_

#include "defines.h"

/*
 * receives a board, command indicator, row num and col num
 * runs Gurobi ILP optimizor on the board and according to the command indicator returns:
 * hint of cell <row,col> - if succeeded the number to place in the cell and 0 otherwise
 * validate - 1 if succeeded and 0 otherwise
 * generate - 1 if succeeded and fills the given board to have a full and correct board, 0 otherwise
 */
int run_ILP(Board *board, int command_index, int row, int col);


#endif /* ILP_H_ */
