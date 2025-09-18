// P2E - Arreglos bidimensionales
#include <stdio.h>
#include <stdlib.h>

#define m 3

typedef struct information{
	char name[25];
	int age;
	int cal;
}info;

int main(){
	int agesAd =0, calAd =0, n, i, total;
	char siono;
	info *stud;

	printf("\nNúmero de alumnos a registrar: ");
	scanf("%d", &n);
	
	stud = (info *) malloc(n * sizeof(info));
       
	for (i=0; i<n; i++){
		printf("\nAlumno %d", i+1);
		
		printf("\nNombre: ");
		fflush(stdin);
		gets(stud[i].name);
		
		printf("Edad: ");
		scanf("%d", &stud[i].age);
		
		printf("Calificación: ");
		scanf("%d", &stud[i].cal);
	}
	
	total = n;
	
	printf("\nDesea agregar más alumnos? [S -> SI, N -> NO]: ");
    fflush(stdin);
    scanf("%s", &siono);

    while (siono == 's' || siono == 'S'){
        printf("\nCuantos más?: ");
        scanf("%d", &n);

        stud = (info *) realloc(stud, (total+n) * sizeof(info));

        for (i = total; i < total+n; i++) {
            printf("\nAlumno %d\n", i+1);
            fflush(stdin);
            printf("Nombre: ");
            gets(stud[i].name);

            printf("Edad: ");
            scanf("%d", &stud[i].age);

            printf("Calificación: ");
            scanf("%d", &stud[i].cal);
        }

        total += n;
        
        printf("\nDesea agregar más alumnos? [S -> SI, N -> NO]: ");
        fflush(stdin);
        scanf("%s", &siono);
    }
		
    for(i=0; i<total; i++) {
        agesAd += stud[i].age;
        calAd  += stud[i].cal;
    }
    
    printf("\nPromedio edades: %.2f", (float)agesAd/total);
	printf("\nPromedio calificaciones: %.2f", (float)calAd/total);
	
	printf("\nNombres de los alumnos en orden inverso: \n");
    for(i = total-1; i>=0; i--) {
        printf("%s\n", stud[i].name);
    }
	
	return 0;
}
