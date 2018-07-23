#include <stdlib.h>


int string_to_int(char *string){
	char *end;
	int num = strtol(string, &end, 10);
	return (num);
}
