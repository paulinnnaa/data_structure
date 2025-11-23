// QUICK SORT :)
#include <stdio.h>
#include <stdlib.h>

void intercambio(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void imprimir(int arr[], int n) {
	int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Partición menor a mayor
int partAsc(int arr[], int low, int high, int n) {
    int pivot = arr[high];
    int i = low - 1, j;

    printf("\nPivote = %d\n", pivot);

    for (j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            intercambio(&arr[i], &arr[j]);
        }
    }

    intercambio(&arr[i + 1], &arr[high]);

    printf("Arreglo despues de la particion: ");
    imprimir(arr, n);

    return i + 1;
}

// Partición mayor a menor
int partDesc(int arr[], int low, int high, int n) {
    int pivot = arr[high];
    int i = low - 1, j;

    printf("\nPivote: %d\n", pivot);

    for (j = low; j < high; j++) {
        if (arr[j] > pivot) {
            i++;
            intercambio(&arr[i], &arr[j]);
        }
    }

    intercambio(&arr[i + 1], &arr[high]);

    printf("Arreglo despues de la particion: ");
    imprimir(arr, n);

    return i + 1;
}

// Menor a mayor
void quickSortAsc(int arr[], int low, int high, int n) {
    if (low < high) {
        int pi = partAsc(arr, low, high, n);
        quickSortAsc(arr, low, pi - 1, n);
        quickSortAsc(arr, pi + 1, high, n);
    }
}

// Mayor a menor
void quickSortDesc(int arr[], int low, int high, int n) {
    if (low < high) {
        int pi = partDesc(arr, low, high, n);
        quickSortDesc(arr, low, pi - 1, n);
        quickSortDesc(arr, pi + 1, high, n);
    }
}

int main() {
    int n, opc, i;
    char siono;
	
	do{
		system("cls");
		printf("Ingresa el numero de elementos: ");
	    scanf("%d", &n);
	
	    int arr[n];
	
	    printf("Ingresa los %d elementos:\n", n);
	    for (i = 0; i < n; i++) {
	        scanf("%d", &arr[i]);
	    }
	
	    printf("\nMENU\n");
	    printf("1. Quick Sort MENOR A MAYOR\n");
	    printf("2. Quick Sort MAYOR A MENOR\n");
	    printf("Elige una opcion: ");
	    scanf("%d", &opc);
	
	    printf("\nORDENAMIENTO\n");
	
	    if (opc == 1)
	        quickSortAsc(arr, 0, n - 1, n);
	    else if (opc == 2)
	        quickSortDesc(arr, 0, n - 1, n);
	    else {
	        printf("Verifica el numero de elementos.\n");
	        return 0;
	    }
	    
	    printf("\nArreglo final ordenado:\n");
    	imprimir(arr, n);
	    
	    printf("\nDeseas ingresar otra lista de elementos? [S -> SI, cualquier otra tecla para salir]: ");
	    fflush(stdin);
		scanf("%s", &siono);
	} while (siono == 'S' || siono == 's');

    return 0;
}

