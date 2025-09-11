#include <stdio.h>

#define m 3

typedef struct information{
	char name[25];
	int age;
	int cal;
}info;

int main(){
	int agesAd = 0, calAd = 0, n;
	info stud[n];
	
	printf("\nNúmero de alumnos: ");
	scanf("%d", &n);
	
	for (int i=0; i<n; i++){
		printf("\nAlumno %d", i+1);
		
		printf("\nNombre: ");
		fflush(stdin);
		gets(stud[i].name);
		
		printf("\nEdad: ");
		scanf("%d", &stud[i].age);
		
		printf("\nCalificación: ");
		scanf("%d", &stud[i].cal);
	}
	
    for(int i=0; i<n; i++) {
        agesAd += stud[i].age;
        calAd  += stud[i].cal;
    }
    
    printf("\nPromedio de edad: %.2f", (float)agesAd/n);
	printf("\nPromedio de calificaciones: %.2f", (float)calAd/n);
	
	return 1;
}
