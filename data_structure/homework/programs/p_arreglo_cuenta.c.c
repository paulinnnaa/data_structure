// Descomposición del número de cuenta - P2E
#include <stdio.h>
#define n 10

void list (int array[n]){
	int i;
	for (i = 0; i < n; i++){
		array[i] = (i+1) * 2;
		printf("|%d|", array[i]);
	}
	printf("\n");
}

void listR (int array[n]){
	int i;
	for (i = n-1; i >= 0; i--){
		printf("|%d|", array[i]);
	}
	printf("\n");
}

void position(int array[n]){ // Aquí hago un "contadorsito" para ubicar la posición 8
	int i, pos = 1;
	for (i = n-1; i>=0; i--, pos++){
		if(pos == 8){
			printf("|-1|");
		} else {
			printf("|%d|", array[i]);
		}
	}
	printf("\n");
}

int main(){
	int pNums[n];
	
	printf("\nPrimeros 10 numeros pares: ");
	list(pNums);
	
	printf("\nEn orden inverso: ");
	listR(pNums);
	
	printf("\nMi numero de cuenta: 322106778");
	
	printf("\n\nCambio en la posicion 8: ");
	position(pNums);
	
	return 1;
}
