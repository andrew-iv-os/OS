#include <stdio.h>
#include <stdlib.h>

int main(void) {
	if(puts("!!!Hello World!!!")==EOF)
	{
		fputs("Error in HelloWorld",stderr);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
