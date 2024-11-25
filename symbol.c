// Symbol Table

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABELS 100
#define MAX_LINE_LENGTH 100

typedef struct {
    char label[50];
    int address;
} Symbol;

Symbol symbolTable[MAX_LABELS];
int symbolCount = 0;

// Function to add a label to the symbol table
void addToSymbolTable(char *label, int address) {
    strcpy(symbolTable[symbolCount].label, label);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

// Function to print the symbol table
void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("Label\t\tAddress\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\t\t0x%04X\n", symbolTable[i].label, symbolTable[i].address);
    }
}

int main() {
    // Hardcoded assembly code
    const char *assemblyCode[] = {
        "START:  MOV AX, BX",
        "        ADD AX, 5",
        "LOOP:   DEC AX",
        "        JNZ LOOP",
        "        HLT"
    };

    int numLines = sizeof(assemblyCode) / sizeof(assemblyCode[0]);
    int address = 0x1000; // Starting memory address

    // Process the hardcoded assembly code
    for (int i = 0; i < numLines; i++) {
        char line[MAX_LINE_LENGTH];
        strcpy(line, assemblyCode[i]);

        char *label = strtok(line, " \t\n");
        if (label && label[strlen(label) - 1] == ':') { // Check for labels
            label[strlen(label) - 1] = '\0'; // Remove colon from the label
            addToSymbolTable(label, address);
        }
        address += 4; // Increment address (e.g., assuming 4 bytes per instruction)
    }

    // Print the symbol table
    printSymbolTable();

    return 0;
}
