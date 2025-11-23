// MERGE SORT :)
#include <stdio.h>
#include <stdlib.h>

void imprimir(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int izq[n1], der[n2];

    int i, j, k;

    // Copiar elementos
    for (i = 0; i < n1; i++)
        izq[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        der[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    // Mezclar
    while (i < n1 && j < n2) {
        if (izq[i] <= der[j]) {
            arr[k] = izq[i];
            i++;
        } else {
            arr[k] = der[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = izq[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = der[j];
        j++;
        k++;
    }
}

int main() {
    int n;
    char siono;

    do{
        system("cls");
        printf("Ingresa el numero de elementos: ");
        scanf("%d", &n);

        int arr[n];
        int i, tam, izq;

        printf("Ingresa los %d elementos:\n", n);
        for (i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        printf("\nPROCESO DE MERGE SORT\n");

        for (tam = 1; tam < n; tam *= 2) {
            printf("\nCombinando subarreglos de tamano %d\n", tam);

            for (izq = 0; izq < n - 1; izq += 2 * tam) {
                int mid = izq + tam - 1;
                int der = izq + 2 * tam - 1;
                int x;

                if (mid >= n) continue;
                if (der >= n) der = n - 1;

                printf("Uniendo: [");
                for (x = izq; x <= mid; x++) printf("%d ", arr[x]);
                printf("] con [");
                for (x = mid + 1; x <= der; x++) printf("%d ", arr[x]);
                printf("]\n");

                merge(arr, izq, mid, der);

                printf("Resultado parcial: ");
                imprimir(arr, n);
            }
        }

        printf("\nARREGLO FINAL ORDENADO\n");
        imprimir(arr, n);

        printf("\nDeseas ingresar otra lista de elementos? [S -> SI, cualquier otra tecla para salir]: ");
        fflush(stdin);
		scanf("%s", &siono);
    } while (siono == 'S' || siono == 's');

    return 0;
}

