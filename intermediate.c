//intermediate code

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

IntermediateCode intermediateTable[MAX_LINES];
int lineCount = 0;

// Function to add an entry to the intermediate code table
void addToIntermediateCode(int address, const char *label, const char *operation, const char *operand1, const char *operand2) {
    intermediateTable[lineCount].lineAddress = address;
    strcpy(intermediateTable[lineCount].label, label ? label : "-");
    strcpy(intermediateTable[lineCount].operation, operation);
    strcpy(intermediateTable[lineCount].operand1, operand1 ? operand1 : "-");
    strcpy(intermediateTable[lineCount].operand2, operand2 ? operand2 : "-");
    lineCount++;
}

// Function to print the intermediate code table
void printIntermediateCode() {
    printf("\nIntermediate Code:\n");
    printf("Address\tLabel\tOperation\tOperand1\tOperand2\n");
    for (int i = 0; i < lineCount; i++) {
        printf("0x%04X\t%s\t%s\t\t%s\t\t%s\n", 
            intermediateTable[i].lineAddress, 
            intermediateTable[i].label, 
            intermediateTable[i].operation, 
            intermediateTable[i].operand1, 
            intermediateTable[i].operand2);
    }
}

int main() {
    // Hardcoded assembly code
    const char *assemblyCode[] = {
        "START:  MOV AX, BX",
        "        ADD AX, ='5'",
        "LOOP:   SUB AX, ='10'",
        "        JNZ LOOP",
        "        HLT"
    };

    int numLines = sizeof(assemblyCode) / sizeof(assemblyCode[0]);
    int address = 0x4000; // Starting memory address

    // Process each line
    for (int i = 0; i < numLines; i++) {
        char line[100];
        strcpy(line, assemblyCode[i]);

        // Extract components
        char *label = strtok(line, " \t\n");
        char *operation = NULL;
        char *operand1 = NULL;
        char *operand2 = NULL;

        // Check if label exists
        if (label && label[strlen(label) - 1] == ':') {
            label[strlen(label) - 1] = '\0'; // Remove colon
            operation = strtok(NULL, " \t\n");
        } else {
            operation = label;
            label = NULL;
        }

        operand1 = strtok(NULL, ", ");
        operand2 = strtok(NULL, ", ");

        // Add to intermediate code
        addToIntermediateCode(address, label, operation, operand1, operand2);

        // Increment address
        address += 4;
    }

    // Print the intermediate code table
    printIntermediateCode();

    return 0;
}
