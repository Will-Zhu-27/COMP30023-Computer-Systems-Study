#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* toUppercase(char *string){
	int i;
	int length = strlen(string);
	char *ret = (char *)calloc(length, sizeof(char));
	if (ret == NULL){
		exit(EXIT_FAILURE);
	}
	printf("%d\n", length);
	for (i = 0; i < length; i++){
		ret[i] = toupper(string[i]);
	}
	return ret;
}

int main(int argc, char **argv){
	char a[] = "Test it!";
	char *uppercase = NULL;
	printf("%s\n",a);
	uppercase = toUppercase(a);
	printf("%s\n",uppercase);
	free(uppercase);
	return 0;
}
