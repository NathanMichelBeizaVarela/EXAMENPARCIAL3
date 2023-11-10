#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNS 3100
#define MAX_ROWS 51
#define BUFFER_SIZE 1024 * 1024

void liberarMemoria(char* palabras[], int numPalabras) {
    for (int i = 0; i < numPalabras; i++) {
        if (palabras[i] != NULL) {
            free(palabras[i]);
        }
    }
}

int main() {
    const char *archivoCsv = "emails.csv";
    FILE *archivoCsvPtr = fopen(archivoCsv, "r");
    if (!archivoCsvPtr) {
        perror("Error al abrir el archivo CSV");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int frecuenciaPalabras[MAX_COLUMNS] = {0};
    char *nombresPalabras[MAX_COLUMNS];
    int empezarConteo;
    int saltoPrimerColumna = -1;

    printf("Ingrese un número de cuenta: ");
    scanf("%d", &empezarConteo);
    empezarConteo %= 1000;
    empezarConteo -= 1;

    if (fgets(buffer, sizeof(buffer), archivoCsvPtr) != NULL) {
        char *token = strtok(buffer, ",");
        int indColumna = 0;
        while (token && indColumna < MAX_COLUMNS) {
            if (strcmp(token, "Correo No.") == 0) {
                saltoPrimerColumna = indColumna;
            } else {
                nombresPalabras[indColumna] = strdup(token);
            }
            token = strtok(NULL, ",");
            indColumna++;
        }
    } else {
        fclose(archivoCsvPtr);
        return 1;
    }

    for (int i = 0; i < empezarConteo; ++i) {
        if (fgets(buffer, sizeof(buffer), archivoCsvPtr) == NULL) {
            fclose(archivoCsvPtr);
            return 1;
        }
    }

    for (int i = 0; i < MAX_ROWS; ++i) {
        if (fgets(buffer, sizeof(buffer), archivoCsvPtr) == NULL) {
            break;
        }
        char *token = strtok(buffer, ",");
        int indColumna = 0;
        while (token && indColumna < MAX_COLUMNS) {
            if (indColumna != saltoPrimerColumna) {
                long val = strtol(token, NULL, 10);
                if (val != 0 || (val == 0 && token[0] == '0')) {
                    frecuenciaPalabras[indColumna] += val;
                }
            }
            token = strtok(NULL, ",");
            indColumna++;
        }
    }

    fclose(archivoCsvPtr);

    FILE *archivoSalida = fopen("Resultado_emails.txt", "w");
    if (!archivoSalida) {
        perror("Error al abrir 'Resultado_emails.txt'");
        liberarMemoria(nombresPalabras, MAX_COLUMNS);
        return 1;
    }

    for (int i = 0; i < MAX_COLUMNS; i++) {
        if (i != saltoPrimerColumna && nombresPalabras[i] != NULL) {
            fprintf(archivoSalida, "La palabra '%s' se repite %d veces\n", nombresPalabras[i], frecuenciaPalabras[i]);
            free(nombresPalabras[i]);
        }
    }

    fclose(archivoSalida);
    printf("El conteo de las palabras fue transferido a 'Resultado_emails.txt'\n");

    return 0;
}

