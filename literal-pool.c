#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LITERALS 100
#define MAX_POOLS 10

typedef struct {
    char literal[50];
    int address;
} Literal;

typedef struct {
    int startIndex;
} Pool;

Literal literalTable[MAX_LITERALS];
int literalCount = 0;

Pool poolTable[MAX_POOLS];
int poolCount = 0;

// Function to add a literal to the literal table
void addToLiteralTable(char *literal) {
    strcpy(literalTable[literalCount].literal, literal);
    literalTable[literalCount].address = -1; // Address to be assigned later
    literalCount++;
}

// Function to add a pool index to the pool table
void addToPoolTable(int startIndex) {
    poolTable[poolCount].startIndex = startIndex;
    poolCount++;
}

// Function to assign addresses to literals in the current pool
void assignAddresses(int *currentAddress) {
    for (int i = poolTable[poolCount - 1].startIndex; i < literalCount; i++) {
        literalTable[i].address = *currentAddress;
        *currentAddress += 4; // Increment address for each literal
    }
}

// Function to print the literal table
void printLiteralTable() {
    printf("\nLiteral Table:\n");
    printf("Literal\t\tAddress\n");
    for (int i = 0; i < literalCount; i++) {
        printf("%s\t\t0x%04X\n", literalTable[i].literal, literalTable[i].address);
    }
}

// Function to print the pool table
void printPoolTable() {
    printf("\nPool Table:\n");
    printf("Pool Number\tStart Index\n");
    for (int i = 0; i < poolCount; i++) {
        printf("%d\t\t%d\n", i + 1, poolTable[i].startIndex);
    }
}

int main() {
    // Hardcoded assembly code with literals and LTORG
    const char *assemblyCode[] = {
        "MOV AX, ='5'",
        "ADD AX, ='10'",
        "LTORG",
        "LOOP:   SUB AX, ='15'",
        "        LTORG",
        "        HLT"
    };

    int numLines = sizeof(assemblyCode) / sizeof(assemblyCode[0]);
    int address = 0x2000; // Starting memory address

    // Initialize the first pool
    addToPoolTable(0);

    // Process the assembly code line by line
    for (int i = 0; i < numLines; i++) {
        char line[100];
        strcpy(line, assemblyCode[i]);

        // Check for LTORG directive
        if (strstr(line, "LTORG")) {
            assignAddresses(&address);   // Assign addresses to the current pool
            addToPoolTable(literalCount); // Start a new pool
            continue;
        }

        // Look for literal in the format ='value'
        char *literal = strstr(line, "='");
        if (literal) {
            literal += 2; // Skip "='"
            char *end = strchr(literal, '\''); // Find closing quote
            if (end) {
                *end = '\0'; // Terminate the literal string
                addToLiteralTable(literal);
            }
        }
    }

    // Assign addresses to any remaining literals
    assignAddresses(&address);

    // Print the literal and pool tables
    printLiteralTable();
    printPoolTable();

    return 0;
}
