#include <stdlib.h>
#include <ctype.h> /*** NEW NEW NEW NEW NEW ***/


int string_to_int(char *string){
	char *end;
	int num = strtol(string, &end, 10);
	return (num);
}

int is_numeric_string(char *string){
	int i = 0;
	int numeric = 1;
	while (string[i] != '\0'){
		if (!isdigit(string[i])){
			numeric = 0;
			break;
		}
		i += 1;
	}
	return numeric;
}
