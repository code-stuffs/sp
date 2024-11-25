// Literal Table

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LITERALS 100
#define MAX_LINE_LENGTH 256

// Structure to hold literal information
typedef struct {
    char literal[MAX_LINE_LENGTH];
    int address;
} Literal;

int main() {
    // Hardcoded assembly code (for demonstration purposes)
    const char *assemblyCode[] = {
        "LOAD R1, =7",
        "LOAD R2, =10",
        "ADD R1, R2",
        "STORE R1, =result",
        "LOAD R3, =20",
        "SUB R1, R3",
        "END"
    };

    Literal literals[MAX_LITERALS];
    int literalCount = 0;

    // Process the assembly code to extract literals
    for (int i = 0; i < sizeof(assemblyCode) / sizeof(assemblyCode[0]); i++) {
        const char *line = assemblyCode[i];
        char *token = strstr(line, "=");
        if (token) {
            // Extract the literal value
            token++; // Move past '='
            char *end = strchr(token, ' '); // Find the next space
            if (end) {
                *end = '\0'; // Terminate the string at the space
            }
            // Check if the literal already exists
            int exists = 0;
            for (int j = 0; j < literalCount; j++) {
                if (strcmp(literals[j].literal, token) == 0) {
                    exists = 1;
                    break;
                }
            }
            // If it's a new literal, add it to the table
            if (!exists) {
                strcpy(literals[literalCount].literal, token);
                literals[literalCount].address = 1000 + (literalCount * 4); // Assign address starting from 1000
                literalCount++;
            }
        }
    }

    // Print the literal table
    printf("Literal Table:\n");
    printf("Address\tLiteral\n");
    printf("-------------------\n");
    for (int i = 0; i < literalCount; i++) {
        printf("%d\t%s\n", literals[i].address, literals[i].literal);
    }

    return 0;
}
