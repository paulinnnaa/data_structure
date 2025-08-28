// Factorial cola
#include<stdio.h>

factTail(int n, int a){
	if (n < 0){
		return 0;
	}
	else if(n == 0){
		return 1;
	}
	else if (n == 1){
		return a;
	}
	else{
		return factTail(n-1, n*a);
	}
}

int main(int argc, char *argv[]){
	int i;
	char *num = argv[1];
	int n = atoi(argv[1]);
	
	fprintf(stdout, "arg[1] = %s \n", argv[1]);
	
	for (i=0; i < strlen(argv[1]); i++){
		fprintf(stdout, "num[%d] = %c \n", i, num[i]);
	}
	
	fprintf(stdout, "Factorial de %d = %d\n", n, factTail(n, 1));
	
	return 0;
}
