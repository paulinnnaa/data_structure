// Factorial recursivo
#include <stdio.h>

int factRecursion(int n){
	if(n < 0){
		return 0;
	}
	else if (n == 0){
		return 1;
	}
	else if (n == 1){
		return 1;
	}
	else{
		return n * factRecursion(n-1);
	}
}

int main(int argc, char *argv[]){
	int i;
	char *num = argv[1];
	int n = atoi (argv[1]);
	
	fprintf(stdout, "arg[1] = %s \n", argv[1]);
	
	for (i=0; i < strlen(argv[1]); i++){
		fprintf(stdout, "num[%d] = %c \n", i, num[i]);
	}
	
	fprintf(stdout, "Factorial de %d = %d\n", n, factRecursion(n));
	
	return 0;
}
