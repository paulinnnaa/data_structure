
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
#include "dlist.h"

#define INPUT_SZ 512
#define LINE_SZ 512

static void show_help(void) {
    printf("\nAYUDA - Calculadora simple\n");
    printf("Bienvenid@ a la calculadora de operaciones.\n");
    printf("INSTRUCCIONES\n");
    printf("Ingrese cualquier expresion algebraica en notacion infija,\n");
    printf("procurando colocar correctamente cada parentesis/operacion para evitar errores de salida,\n");
    printf("click enter y de forma automatica se mostraran los tokens (out),\n");
    printf("a forma prefija y la forma postfija de la expresion dada.\n");
    printf("\nNota: todo el historial de operaciones se guardara en un archivo \nllamado proceso.txt en la ruta /Calculadora/proceso.txt.\n");
    printf("\nEjemplo al ingresar la expresion: (3 + 4 * ( 2 - 1 ) ).\n\n");
}

int main() {
    char line[LINE_SZ];
    char input[INPUT_SZ];
    int opcion = -1;

    while (1) {
        printf("\n=== CALCULADORA ===\n");
        printf("1. Tokenizar expresion\n");
        printf("2. Convertir a Postfija (paso a paso)\n");
        printf("3. Evaluar expresion (postfija paso a paso)\n");
        printf("4. Convertir a Prefija (paso a paso)\n");
        printf("0. Salir\n");
        printf("/h para ayuda\n");
        printf("Seleccione: ");

        if (!fgets(line, sizeof(line), stdin)) {
            /* EOF o error */
            break;
        }
        /* quitar newline */
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '/') {
            if (strcmp(line, "/h") == 0) {
                show_help();
                continue;
            } else {
                printf("Comando desconocido: %s\n", line);
                continue;
            }
        }

        /* intentar parsear entero */
        if (sscanf(line, "%d", &opcion) != 1) {
            printf("Opción inválida. Escribe /h para ayuda.\n");
            continue;
        }

        if (opcion == 0) break;

        printf("Ingrese expresion: ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Entrada inválida.\n");
            continue;
        }
        input[strcspn(input, "\r\n")] = '\0';

        /* TOKENIZAR */
        DList tokens;
        FILE *log = fopen("proceso.txt", "a");
        if (!log) log = stdout; 

        dlist_init(&tokens, free);

        if (!tokenize(input, &tokens)) {
            printf("Error tokenizando.\n");
            dlist_destroy(&tokens);
            if (log != stdout) fclose(log);
            continue;
        }

        print_tokens(&tokens);

        if (opcion == 1) {
            if (log != stdout) fclose(log);
            dlist_destroy(&tokens);
            continue;
        }

        /* POSTFIJO */
        if (opcion == 2) {
            List postfix;
            list_init(&postfix, free);

            if (!infix_to_postfix(&tokens, &postfix, log)) {
                printf("Error convirtiendo a postfija.\n");
            } else {
                printf("\n--- POSTFIJA FINAL ---\n");
                print_list(&postfix);
            }

            list_destroy(&postfix);
            if (log != stdout) fclose(log);
            dlist_destroy(&tokens);
            continue;
        }

        /* EVALUAR */
        if (opcion == 3) {
            List postfix2;
            list_init(&postfix2, free);

            if (!infix_to_postfix(&tokens, &postfix2, log)) {
                printf("Error convirtiendo a postfija.\n");
                list_destroy(&postfix2);
                if (log != stdout) fclose(log);
                dlist_destroy(&tokens);
                continue;
            }

            printf("\n--- POSTFIJA FINAL ---\n");
            print_list(&postfix2);

            printf("\n--- RESULTADO ---\n");
            {
                double resultado;
                if (eval_postfix(&postfix2, &resultado, log))
                    printf("%.10g\n", resultado);
                else
                    printf("Error evaluando.\n");
            }

            list_destroy(&postfix2);
            if (log != stdout) fclose(log);
            dlist_destroy(&tokens);
            continue;
        }

        /* PREFIJA */
        if (opcion == 4) {
            List prefix;
            list_init(&prefix, free);

            if (!infix_to_prefix(&tokens, &prefix, log)) {
                printf("Error convirtiendo a prefija.\n");
            } else {
                printf("\n--- PREFIJA FINAL ---\n");
                print_list(&prefix);
            }

            list_destroy(&prefix);
            if (log != stdout) fclose(log);
            dlist_destroy(&tokens);
            continue;
        }

        printf("Opcion no reconocida.\n");
    }

    return 0;
}

