//MACRO CALL AND MACRO DEFINITION TABLE

#include <stdio.h>
#include <string.h>

#define MAX_MNT 10
#define MAX_MDT 50
#define MAX_PARAMS 10
#define MAX_LINE_LENGTH 100

typedef struct {
    char name[20];
    int mdtIndex;
    int paramCount;
    char formal[MAX_PARAMS][20];
} MNTEntry;

typedef struct {
    char line[MAX_LINE_LENGTH];
} MDTEntry;

typedef struct {
    int mntIndex;
    char actual[MAX_PARAMS][20];
} ParamTableEntry;

MNTEntry mnt[MAX_MNT];
MDTEntry mdt[MAX_MDT];
ParamTableEntry pTable[MAX_MNT];
int mntCount = 0;
int mdtCount = 0;

void defineMacro(char *name, char params[][20], int paramCount, char body[][MAX_LINE_LENGTH], int numLines) {
    strcpy(mnt[mntCount].name, name);
    mnt[mntCount].mdtIndex = mdtCount;
    mnt[mntCount].paramCount = paramCount;

    for (int i = 0; i < paramCount; i++) {
        strcpy(mnt[mntCount].formal[i], params[i]);
        strcpy(pTable[mntCount].actual[i], params[i]);
    }
    pTable[mntCount].mntIndex = mntCount;

    for (int i = 0; i < numLines; i++) {
        strcpy(mdt[mdtCount++].line, body[i]);
    }

    mntCount++;
}

void displayTablesToConsole() {
    printf("\nMacro Name Table (MNT):\n");
    printf("Name        MDT Index     Num Params    Formals\n");
    for (int i = 0; i < mntCount; i++) {
        printf("%-12s %-12d %-12d ", mnt[i].name, mnt[i].mdtIndex, mnt[i].paramCount);
        for (int j = 0; j < mnt[i].paramCount; j++) {
            printf("%s ", mnt[i].formal[j]);
        }
        printf("\n");
    }

    printf("\nMacro Definition Table (MDT):\n");
    for (int i = 0; i < mdtCount; i++) {
        printf("%d   %s\n", i, mdt[i].line);
    }

    printf("\nParameter Table:\n");
    printf("MNT Index    Parameters\n");
    for (int i = 0; i < mntCount; i++) {
        printf("%-12d ", pTable[i].mntIndex);
        for (int j = 0; j < mnt[i].paramCount; j++) {
            printf("%s ", pTable[i].actual[j]);
        }
        printf("\n");
    }
}

int main() {
    // Hardcoded Input
    char name[] = "BIG_MACRO";
    int paramCount = 5;
    char params[MAX_PARAMS][20] = { "&ARG1", "&ARG2", "&ARG3", "&ARG4", "&ARG5" };
    int numLines = 11;
    char body[MAX_MDT][MAX_LINE_LENGTH] = {
        "MOV A, &ARG1",
        "ADD A, &ARG2",
        "SUB A, &ARG3",
        "MUL A, &ARG4",
        "DIV A, &ARG5",
        "MOV &ARG1, A",
        "STORE A, &ARG1",
        "STORE B, &ARG2",
        "STORE C, &ARG3",
        "STORE D, &ARG4",
        "STORE E, &ARG5"
    };

    // Define the macro
    defineMacro(name, params, paramCount, body, numLines);

    // Display tables
    displayTablesToConsole();

    return 0;
}
