#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLUMNAS 3100
#define MAX_FILAS 51
#define BUFFER_SIZE 1024 * 1024

int main() {
    const char *csvFileName = "emails.csv";
    FILE *csvFile = fopen(csvFileName, "r");
    
    if (!csvFile) {
        perror("Error");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int wordCounts[MAX_COLUMNAS] = {0};
    char *wordList[MAX_COLUMNAS];
    int startCount;
    int skipFirstColumn = -1;

    printf("Ingrese un numero\n");
    scanf("%d", &startCount);
    startCount %= 1000;
    startCount -= 1;

    if (fgets(buffer, sizeof(buffer), csvFile) != NULL) {
        char *token = strtok(buffer, ",");
        int columnIndex = 0;
        
        while (token && columnIndex < MAX_COLUMNAS) {
            if (strcmp(token, "Email No.") == 0) {
                skipFirstColumn = columnIndex;
            } else {
                wordList[columnIndex] = strdup(token);
            }
            
            token = strtok(NULL, ",");
            columnIndex++;
        }
    } else {
        fclose(csvFile);
        return 1;
    }

    for (int i = 0; i < startCount; ++i) {
        if (fgets(buffer, sizeof(buffer), csvFile) == NULL) {
            fclose(csvFile);
            return 1;
        }
    }

    for (int i = 0; i < MAX_FILAS; ++i) {
        if (fgets(buffer, sizeof(buffer), csvFile) == NULL) {
            break;
        }
        
        char *token = strtok(buffer, ",");
        int columnIndex = 0;
        
        while (token && columnIndex < MAX_COLUMNAS) {
            if (columnIndex != skipFirstColumn) {
                long val = strtol(token, NULL, 10);
                if (val != 0 || (val == 0 && token[0] == '0')) {
                    wordCounts[columnIndex] += val;
                }
            }
            
            token = strtok(NULL, ",");
            columnIndex++;
        }
    }

    fclose(csvFile);

    FILE *outputFile = fopen("Resultados.txt", "w");
    
    if (!outputFile) {
        perror("Error al abrir Resultados.txt");
        return 1;
    }

    for (int i = 0; i < MAX_COLUMNAS; i++) {
        if (i != skipFirstColumn && wordList[i] != NULL) {
            fprintf(outputFile, "%s %d\n", wordList[i], wordCounts[i]);
            free(wordList[i]);
        }
    }

    fclose(outputFile);
    printf("Se ha creado el conteo en Resultados.txt\n");

    return 0;
}
