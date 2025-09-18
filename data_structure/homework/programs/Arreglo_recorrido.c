// P2E - Recorrido de matriz bidimensional
#include <stdio.h>

void leeArr(int n, int m, int array[n][m]){
	int i, j;
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			printf("\nIngresar el elemento [%d][%d]: ", i,j);
			scanf("%d", &array[i][j]);
		}
	}
}

void impArr(int n, int m, int array[n][m]){
	int i, j;
	printf("\nEl arreglo dado es: \n");
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			printf("[%d]",array[i][j]);
		}
		printf("\n");
	}
}

void izqDer(int n, int m, int array[n][m]){
	int i, j;
	printf("\nRecorrido normal: \n");
    for (i = 0; i<n; i++){
        for (j = 0; j<m; j++){
            printf("[%d]", array[i][j]);
        }
    }
    printf("\n");
}

void abajoArriba(int n, int m, int array[n][m]){
	int i, j;
	printf("\nRecorrido de abajo hacia arriba: \n");
	for(j = m-1; j>=0; j--){
	    for(i = n-1; i>=0; i--){
	        printf("[%d]", array[i][j]);
	    }
	}
    printf("\n");
}

void diagonal(int n, int m, int array[n][m]){
	int i;
	printf("\nRecorrido en diagonal: \n");
	if (n!=m || n<m || n>m){
		printf("\nEl arreglo no tiene una diagonal completa :/");
	} else {
		for (i = 0; i < n; i++) {
	        printf("[%d]", array[i][i]);
	    }
	    printf("\n");
	}
    
}

int main(){
	int n, m;
	char siono;
	
	do{
		system("cls");
		printf("\nIntroduzca el tamano del arreglo (max 10)");
		printf("\nTamano de filas N: ");
		scanf("%d", &n);
		printf("\nTamano de columnas M: ");
		scanf("%d", &m);
		
		int array[n][m];
		
		leeArr(n, m, array);
		impArr(n, m, array);
		
		izqDer(n, m, array);
		abajoArriba(n, m, array);
		diagonal(n, m, array);
		
		printf("\n\nDesea llenar otro arreglo? [S -> SI, N -> NO]: ");
		scanf("%s", &siono);
	} while (siono == 'S' || siono == 's');
	
	printf("\nByeeeee :)");
	
	return 0;
}
