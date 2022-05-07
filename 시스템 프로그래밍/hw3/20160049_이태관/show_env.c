#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
	char *env = getenv("USER");

	printf("USER = %s\n" , env);

}

