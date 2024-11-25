//pass1 assembler to generate symbol table, pool table, literal table and intermdiate code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_LITERALS 100
#define MAX_MNEMONICS 100
#define MAX_LINE_LENGTH 100

// Symbol Table
typedef struct {
    char name[20];
    int address;
} Symbol;

// Literal Table
typedef struct {
    char value[20];
    int address;
} Literal;

// Mnemonic Table
typedef struct {
    char mnemonic[10];
    char type[5];
    int opcode;
} Mnemonic;

// Pool Table
typedef struct {
    int literal_index;
    int address;
} Pool;

// Global variables
Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

Literal literal_table[MAX_LITERALS];
int literal_count = 0;

Mnemonic mnemonic_table[MAX_MNEMONICS];
int mnemonic_count = 0;

Pool pool_table[MAX_LITERALS];
int pool_count = 0;

int location_counter = 0;

// Function prototypes
void initialize_mnemonic_table();
int search_mnemonic(const char* mnemonic);
int search_symbol(const char* symbol);
void add_symbol(const char* symbol, int address);
int add_literal(const char* literal);
void process_line(char* line, FILE* output_file);
void generate_intermediate_code(FILE* input, const char* output_file);
void print_tables();

// Error handling
void report_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
}

void initialize_mnemonic_table() {
    strcpy(mnemonic_table[mnemonic_count].mnemonic, "START");
    strcpy(mnemonic_table[mnemonic_count].type, "AD");
    mnemonic_table[mnemonic_count++].opcode = -1;

    strcpy(mnemonic_table[mnemonic_count].mnemonic, "END");
    strcpy(mnemonic_table[mnemonic_count].type, "AD");
    mnemonic_table[mnemonic_count++].opcode = -1;

    strcpy(mnemonic_table[mnemonic_count].mnemonic, "MOVR");
    strcpy(mnemonic_table[mnemonic_count].type, "IS");
    mnemonic_table[mnemonic_count++].opcode = 1;

    strcpy(mnemonic_table[mnemonic_count].mnemonic, "ADD");
    strcpy(mnemonic_table[mnemonic_count].type, "IS");
    mnemonic_table[mnemonic_count++].opcode = 2;

    strcpy(mnemonic_table[mnemonic_count].mnemonic, "DC");
    strcpy(mnemonic_table[mnemonic_count].type, "DL");
    mnemonic_table[mnemonic_count++].opcode = -1;

    strcpy(mnemonic_table[mnemonic_count].mnemonic, "DS");
    strcpy(mnemonic_table[mnemonic_count].type, "DL");
    mnemonic_table[mnemonic_count++].opcode = -1;
}

int search_mnemonic(const char* mnemonic) {
    for (int i = 0; i < mnemonic_count; i++) {
        if (strcmp(mnemonic_table[i].mnemonic, mnemonic) == 0) {
            return i;
        }
    }
    return -1;
}

int search_symbol(const char* symbol) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, symbol) == 0) {
            return i;
        }
    }
    return -1;
}

void add_symbol(const char* symbol, int address) {
    if (symbol_count < MAX_SYMBOLS) {
        strcpy(symbol_table[symbol_count].name, symbol);
        symbol_table[symbol_count].address = address;
        symbol_count++;
    } else {
        report_error("Symbol table full");
    }
}

int add_literal(const char* literal) {
    for (int i = 0; i < literal_count; i++) {
        if (strcmp(literal_table[i].value, literal) == 0) {
            return i;
        }
    }

    if (literal_count < MAX_LITERALS) {
        strcpy(literal_table[literal_count].value, literal);
        literal_table[literal_count].address = -1;  // Address will be assigned later
        return literal_count++;
    } else {
        report_error("Literal table full");
        return -1;
    }
}

void process_line(char* line, FILE* output_file) {
    char label[20] = "", opcode[10] = "", operand1[20] = "", operand2[20] = "";
    char* token = strtok(line, " \t\n");
    int token_count = 0;

    while (token != NULL && token_count < 4) {
        switch (token_count) {
            case 0:
                if (strchr(token, ':') != NULL) {
                    strncpy(label, token, strchr(token, ':') - token);
                    token = strtok(NULL, " \t\n");
                    strcpy(opcode, token);
                } else {
                    strcpy(opcode, token);
                }
                break;
            case 1:
                strcpy(operand1, token);
                break;
            case 2:
                if (token[strlen(token)-1] == ',') {
                    token[strlen(token)-1] = '\0';
                }
                strcpy(operand2, token);
                break;
        }
        token = strtok(NULL, " \t\n");
        token_count++;
    }

    if (strlen(label) > 0) {
        add_symbol(label, location_counter);
    }

    int mnemonic_index = search_mnemonic(opcode);
    if (mnemonic_index == -1) {
        report_error("Invalid mnemonic");
        return;
    }

    fprintf(output_file, "%d\t", location_counter);

    if (strcmp(mnemonic_table[mnemonic_index].type, "IS") == 0) {
        fprintf(output_file, "(IS,%02d)\t", mnemonic_table[mnemonic_index].opcode);

        if (strlen(operand1) > 0) {
            if (isalpha(operand1[0])) {
                int symbol_index = search_symbol(operand1);
                if (symbol_index == -1) {
                    add_symbol(operand1, -1);  // Forward reference
                    symbol_index = symbol_count - 1;
                }
                fprintf(output_file, "(S,%02d)\t", symbol_index);
            } else {
                fprintf(output_file, "(C,%s)\t", operand1);
            }
        }

        if (strlen(operand2) > 0) {
            if (operand2[0] == '=') {
                int literal_index = add_literal(operand2);
                fprintf(output_file, "(L,%02d)\t", literal_index);

                // Update pool table
                if (pool_count == 0 || pool_table[pool_count - 1].literal_index != literal_index) {
                    pool_table[pool_count].literal_index = literal_index;
                    pool_table[pool_count].address = location_counter;
                    pool_count++;
                }
            } else if (isalpha(operand2[0])) {
                int symbol_index = search_symbol(operand2);
                if (symbol_index == -1) {
                    add_symbol(operand2, -1);  // Forward reference
                    symbol_index = symbol_count - 1;
                }
                fprintf(output_file, "(S,%02d)\t", symbol_index);
            } else {
                fprintf(output_file, "(C,%s)\t", operand2);
            }
        }

        location_counter++;
    } else if (strcmp(mnemonic_table[mnemonic_index].type, "DL") == 0) {
        fprintf(output_file, "(DL,%02d)\t", mnemonic_table[mnemonic_index].opcode);

        if (strcmp(opcode, "DC") == 0) {
            fprintf(output_file, "(C,%s)\t", operand1);
            location_counter++;
        } else if (strcmp(opcode, "DS") == 0) {
            int size = atoi(operand1);
            fprintf(output_file, "(C,%d)\t", size);
            location_counter += size;
        }
    } else if (strcmp(mnemonic_table[mnemonic_index].type, "AD") == 0) {
        fprintf(output_file, "(AD,%02d)\t", mnemonic_table[mnemonic_index].opcode);

        if (strcmp(opcode, "START") == 0) {
            location_counter = atoi(operand1);
        } else if (strcmp(opcode, "END") == 0) {
            // Assign addresses to literals
            for (int i = 0; i < literal_count; i++) {
                if (literal_table[i].address == -1) {
                    literal_table[i].address = location_counter++;
                }
            }
        }
    }

    fprintf(output_file, "\n");
}

void generate_intermediate_code(FILE* input, const char* output_file) {
    FILE *output = fopen(output_file, "w");

    if (output == NULL) {
        report_error("Unable to open output file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input)) {
        if (line[0] != '\n' && line[0] != '\0') {
            process_line(line, output);
        }
    }

    fclose(output);
}

void print_tables() {
    printf("\nSymbol Table:\n");
    printf("Name\tAddress\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\t%d\n", symbol_table[i].name, symbol_table[i].address);
    }

    printf("\nLiteral Table:\n");
    printf("Value\tAddress\n");
    for (int i = 0; i < literal_count; i++) {
        printf("%s\t%d\n", literal_table[i].value, literal_table[i].address);
    }

    printf("\nPool Table:\n");
    printf("Literal Index\tAddress\n");
    for (int i = 0; i < pool_count; i++) {
        printf("%d\t\t%d\n", pool_table[i].literal_index, pool_table[i].address);
    }
}

int main() {
    // Hardcoded input as if it were read from a file
    char* input_lines[] = {
        "START 100",
        "LOOP: MOVR A, 5",
        "ADD A,  B",
        "MOVR B, =10",
        "DC 20",
        "VAR: DS 2",
        "END"
    };

    FILE* input = tmpfile();  // Create a temporary file to simulate input
    for (int i = 0; i < 7; i++) {
        fputs(input_lines[i], input);
        fputs("\n", input);
    }
    rewind(input);  // Rewind to the beginning of the file

    initialize_mnemonic_table();
    generate_intermediate_code(input, "intermediate_code.txt");
    print_tables();

    fclose(input);  // Close the temporary file
    return 0;
}
