//Macro Expansion

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
    int paramCount;
    char actual[MAX_PARAMS][20];
} ParamTable;

MNTEntry mnt[MAX_MNT];
MDTEntry mdt[MAX_MDT];
ParamTable paramTable;
int mntCount = 0;
int mdtCount = 0;

void setupParamTable(char actualParams[][20], int count) {
    paramTable.paramCount = count;
    for (int i = 0; i < count; i++) {
        strcpy(paramTable.actual[i], actualParams[i]);
    }
}

void expandMacro(char *macroName, char actualParams[][20], int paramCount) {
    int mntIndex = -1;

    // Locate the macro in MNT
    for (int i = 0; i < mntCount; i++) {
        if (strcmp(mnt[i].name, macroName) == 0) {
            mntIndex = i;
            break;
        }
    }

    if (mntIndex == -1) {
        printf("Error: Macro %s not defined.\n", macroName);
        return;
    }

    setupParamTable(actualParams, paramCount);

    int mdtIndex = mnt[mntIndex].mdtIndex;
    int numLines = 11;  // Adjusted based on macro size in Assignment 4

    printf("Expanded Macro %s:\n", macroName);
    for (int i = mdtIndex; i < mdtIndex + numLines; i++) {
        char expandedLine[MAX_LINE_LENGTH];
        strcpy(expandedLine, mdt[i].line);

        // Substitute formal parameters with actual parameters
        for (int j = 0; j < paramTable.paramCount; j++) {
            char *paramPosition;
            while ((paramPosition = strstr(expandedLine, mnt[mntIndex].formal[j])) != NULL) {
                char tempLine[MAX_LINE_LENGTH];
                strncpy(tempLine, expandedLine, paramPosition - expandedLine);
                tempLine[paramPosition - expandedLine] = '\0';
                strcat(tempLine, paramTable.actual[j]);
                strcat(tempLine, paramPosition + strlen(mnt[mntIndex].formal[j]));
                strcpy(expandedLine, tempLine);
            }
        }

        printf("%s\n", expandedLine);
    }
}

int main() {
    // Define the macro as in Assignment 4
    char params[5][20] = {"&ARG1", "&ARG2", "&ARG3", "&ARG4", "&ARG5"};
    char body[11][MAX_LINE_LENGTH] = {
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

    strcpy(mnt[mntCount].name, "BIG_MACRO");
    mnt[mntCount].mdtIndex = mdtCount;
    mnt[mntCount].paramCount = 5;
    for (int i = 0; i < 5; i++) {
        strcpy(mnt[mntCount].formal[i], params[i]);
    }
    mntCount++;

    for (int i = 0; i < 11; i++) {
        strcpy(mdt[mdtCount++].line, body[i]);
    }

    // Expand the macro with actual parameters
    char actualParams[5][20] = {"X", "Y", "Z", "W", "V"};
    expandMacro("BIG_MACRO", actualParams, 5);

    return 0;
}
