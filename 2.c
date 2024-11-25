#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char label[20];
    int address;
} Symbol;

typedef struct {
    char literal[20];
    int address;
} Literal;

typedef struct {
    char opcode[10];
    char operand[20];
} IntermediateCode;

Symbol symbolTable[MAX];
Literal literalTable[MAX];
IntermediateCode intermediateCode[MAX];
int symbolCount = 0, literalCount = 0, icCount = 0, currentAddress = 0;

void addSymbol(char *label) {
    strcpy(symbolTable[symbolCount].label, label);
    symbolTable[symbolCount++].address = currentAddress++;
}

void addLiteral(char *literal) {
    strcpy(literalTable[literalCount].literal, literal);
    literalTable[literalCount++].address = -1;
}

void generateIntermediateCode(char *opcode, char *operand) {
    strcpy(intermediateCode[icCount].opcode, opcode);
    strcpy(intermediateCode[icCount++].operand, operand);
    currentAddress++;
}

void processSourceCode(char **lines, int lineCount) {
    char label[20], opcode[10], operand[20];
    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        label[0] = opcode[0] = operand[0] = '\0';

        if (strchr(line, ':')) {
            sscanf(line, "%[^:]: %s %s", label, opcode, operand);
            addSymbol(label);
        } else {
            sscanf(line, "%s %s", opcode, operand);
        }

        if (operand[0] == '#') addLiteral(operand);
        generateIntermediateCode(opcode, operand);
    }
}

void printTable(char *header, void *table, int count, int isSymbol) {
    printf("%s\n", header);
    for (int i = 0; i < count; i++) {
        if (isSymbol)
            printf("%s\t%d\n", ((Symbol *)table)[i].label, ((Symbol *)table)[i].address);
        else
            printf("%s\t%d\n", ((Literal *)table)[i].literal, ((Literal *)table)[i].address);
    }
}

void printIntermediateCode() {
    printf("Intermediate Code:\n");
    for (int i = 0; i < icCount; i++) {
        printf("%s\t%s\n", intermediateCode[i].opcode, intermediateCode[i].operand);
    }
}

int main() {
    // Hardcoded input lines
    char *sourceLines[] = {
        "START",
        "MOV R1, #100",
        "ADD R2, #50",
        "JUMP LOOP",
        "LOOP:",
        "MOV R3, #200",
        "END"
    };

    int lineCount = sizeof(sourceLines) / sizeof(sourceLines[0]);
    processSourceCode(sourceLines, lineCount);

    // Print tables and intermediate code
    printTable("Symbol Table:", symbolTable, symbolCount, 1);
    printTable("Literal Table:", literalTable, literalCount, 0);
    printIntermediateCode();

    return 0;
}
