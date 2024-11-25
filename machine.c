//machine code

#include <stdio.h>
#include <string.h>

#define MAX_LINES 100

typedef struct {
    int lineAddress;
    char label[20];
    char operation[10];
    char operand1[20];
    char operand2[20];
} IntermediateCode;

typedef struct {
    char operation[10];
    char binary[10];
} Opcode;

Opcode opcodeTable[] = {
    {"MOV", "0001"},
    {"ADD", "0010"},
    {"SUB", "0011"},
    {"JNZ", "0100"},
    {"HLT", "1111"}
};
int opcodeCount = sizeof(opcodeTable) / sizeof(opcodeTable[0]);

IntermediateCode intermediateTable[MAX_LINES];
int lineCount = 0;

// Function to add to intermediate code
void addToIntermediateCode(int address, const char *label, const char *operation, const char *operand1, const char *operand2) {
    intermediateTable[lineCount].lineAddress = address;
    strcpy(intermediateTable[lineCount].label, label ? label : "-");
    strcpy(intermediateTable[lineCount].operation, operation);
    strcpy(intermediateTable[lineCount].operand1, operand1 ? operand1 : "-");
    strcpy(intermediateTable[lineCount].operand2, operand2 ? operand2 : "-");
    lineCount++;
}

// Function to get the binary equivalent of an opcode
char *getBinaryForOpcode(char *operation) {
    for (int i = 0; i < opcodeCount; i++) {
        if (strcmp(opcodeTable[i].operation, operation) == 0) {
            return opcodeTable[i].binary;
        }
    }
    return "0000"; // Default binary for unknown opcodes
}

// Function to print the machine code
void generateMachineCode() {
    printf("\nMachine Code:\n");
    printf("Address\tBinary Code\n");

    for (int i = 0; i < lineCount; i++) {
        char *binaryOpcode = getBinaryForOpcode(intermediateTable[i].operation);

        // Simplify operands for demonstration (real assembly would have a more complex mapping)
        char operand1Binary[20] = "0000";
        char operand2Binary[20] = "0000";

        if (strcmp(intermediateTable[i].operand1, "AX") == 0) strcpy(operand1Binary, "0001");
        if (strcmp(intermediateTable[i].operand2, "BX") == 0) strcpy(operand2Binary, "0010");
        if (strstr(intermediateTable[i].operand2, "='")) strcpy(operand2Binary, "LITERAL");

        printf("0x%04X\t%s %s %s\n", 
            intermediateTable[i].lineAddress, 
            binaryOpcode, 
            operand1Binary, 
            operand2Binary);
    }
}

int main() {
    // Hardcoded intermediate code as input
    addToIntermediateCode(0x4000, "START", "MOV", "AX", "BX");
    addToIntermediateCode(0x4004, "-", "ADD", "AX", "='5'");
    addToIntermediateCode(0x4008, "LOOP", "SUB", "AX", "='10'");
    addToIntermediateCode(0x400C, "-", "JNZ", "LOOP", "-");
    addToIntermediateCode(0x4010, "-", "HLT", "-", "-");

    // Generate and print the machine code
    generateMachineCode();

    return 0;
}
