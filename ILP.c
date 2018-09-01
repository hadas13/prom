#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "defines.h"

int run_ILP(Board *board, int command_index, int row, int col){
	/* game parameters */
	int sub_dim_rows = board->m_rows; /* number of rows in each block */
	int sub_dim_cols = board->m_cols; /* number of rows in each block */
	int dim = board->n; /* board dimensions */
	int dimpow = dim * dim * dim; /* dim power 3 */

	/* gurobi parameters */
	GRBenv *env   = NULL;
	GRBmodel *model = NULL;
	int *ind;
	double *val;
	double *low; /* lower bounds for variables */
	char *vtype;
	double *sol;
	int optimstatus;
	double objval;
	int error = 0;

	int x, y, z, x_sub, y_sub, count; /* indexes for row, col, val etc. */
	int result = 1; /* return value of the function */

	ind = (int *) malloc(dim * sizeof(int));
	if (ind == NULL){
		printf("malloc has failed\n");
		exit(0);
	}
	val = (double *) malloc (dim * sizeof(double));
	if (val == NULL){
		free(ind);
		printf("malloc has failed\n");
		exit(0);
	}
	low = (double *) malloc (dimpow * sizeof(double));
	if (low == NULL){
		free(ind);
		free(val);
		printf("malloc has failed\n");
		exit(0);
	}
	vtype = (char *) malloc (dimpow * sizeof(char));
	if (vtype == NULL){
		free(ind);
		free(val);
		free(low);
		printf("malloc has failed\n");
		exit(0);

	}
	sol = (double *) malloc (dimpow * sizeof(double));
	if (sol == NULL){
		free(ind);
		free(val);
		free(low);
		free(vtype);
		printf("malloc has failed\n");
		exit(0);
	}

	/* read the current cells from the board and update ILP variables */
	for (x = 0; x < dim; x++){
		for (y = 0; y < dim; y++){
			for (z = 1; z < dim + 1; z++){ /* values in our board start from 1 and not 0 */
				if (board->game_table[x][y].val == z){
					low[x * dim * dim + y * dim + (z - 1)] = 1; /* value is already set */
				}
				else{
					low[x * dim * dim + y * dim + (z - 1)] = 0; /* value isn't set */
				}
				vtype[x * dim * dim + y * dim + (z - 1)] = GRB_BINARY;
			}
		}
	}

	/* creating the environment */
	error = GRBloadenv(&env, "mip1.log");
	if (error){
		printf("error in loading Gurobi's environment'n");
		goto ERROR;
	}

	/* create new model */
	/* TODO check about names parameter */
	error = GRBnewmodel(env, &model, "mip1", dimpow, NULL, low, NULL, vtype, NULL);
	if (error){
		printf("error in creating Gurobi's model\n");
		goto ERROR;
	}

	/* constraint - a value for each cell */
	for (x = 0; x < dim; x++){
		for (y = 0; y < dim; y++){
			for (z = 0; z < dim; z++){
				ind[z] = x * dim * dim + y * dim + z;
				val[z] = 1;
			}
			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error){
				printf("error in adding Gurobi's value constraint\n");
				goto ERROR;
			}
		}
	}

	/* constraint - each row has all the values */
	for (z = 0; z < dim; z++){
		for (y = 0; y < dim; y++){
			for (x = 0; x < dim; x++){
				ind[x] = x * dim * dim + y * dim + z;
				val[x] = 1.0;
			}
			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error){
				printf("error in adding Gurobi's row constraint\n");
				goto ERROR;
			}
		}
	}

	/* constraint - each column has all the values */
	for (z = 0; z < dim; z++){
		for (x = 0; x < dim; x++){
			for (y = 0; y < dim; y++){
				ind[y] = x * dim * dim + y * dim + z;
				val[y] = 1.0;
			}
			error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error){
				printf("error in adding Gurobi's column constraint\n");
				goto ERROR;
			}
		}
	}

	/* constraint - each sub block has all the values */
	for (z = 0; z < dim; z++){
		for (x_sub = 0; x_sub < sub_dim_cols; x_sub++){
			for (y_sub = 0; y_sub < sub_dim_rows; y_sub++){
				count = 0;
				for (x = x_sub * sub_dim_rows; x < (x_sub + 1) * sub_dim_rows; x++){
					for (y = y_sub * sub_dim_cols; y < (y_sub + 1) * sub_dim_cols; y++){
						ind[count] = x * dim * dim + y * dim + z;
						val[count] = 1.0;
						count++;
					}
				}
				error = GRBaddconstr(model, dim, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error){
					printf("error in adding Gurobi's sub block constraint\n");
					goto ERROR;
				}
			}
		}
	}

	/* optimize model */
	error = GRBoptimize(model);
	if (error){
		printf("error in Gurobi's optimizing function\n");
		goto ERROR;
	}

	/* write model to mip1.lp */
	error = GRBwrite(model, "mip1.lp");
	if (error){
		printf("error in Gurobi's writing function\n");
		goto ERROR;
	}

	/* solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error){
		printf("error in Gurobi's getting solution function\n");
		goto ERROR;
	}

	/* get optimal result */
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error){
		printf("error in Gurobi's getting optimal result function\n");
		goto ERROR;
	}

	/* assign value to variables */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, dimpow, sol);
	if (error) {
		printf("error in Gurobi's get variables values function\n");
		goto ERROR;
	}

	if (optimstatus == GRB_OPTIMAL){ /* board has solution */
		if (command_index == 1){ /* hint command */
			/* go over possible value of the given cell to find ILP assigned value */
			for (z = 0; z < dim; z++){
				if (sol[(row - 1) * dim * dim + (col - 1) * dim + z] == 1){
					result = z + 1;
				}
			}
		}
		else if(command_index == 2){ /* validate command */
		}
		else { /* generate command */
			/* filling the board game to receive a complete legal board */
			for (x = 0; x < dim; x++){
				for (y = 0; y < dim; y++){
					for (z = 0; z < dim; z++){
						if (sol[x * dim * dim + y * dim + z] == 1){
							board->game_table[x][y].val = z + 1;
						}
					}
				}
			}
		}
		free(ind);
		free(val);
		free(low);
		free(vtype);
		free(sol);
		GRBfreemodel(model);
		GRBfreeenv(env);
		return result;
	}
	return 0;

	ERROR:
	free(ind);
	free(val);
	free(low);
	free(vtype);
	free(sol);
	GRBfreemodel(model);
	GRBfreeenv(env);
	result = 0;
	return result;
}
