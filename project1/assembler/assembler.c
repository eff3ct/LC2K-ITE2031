/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

// symbolic address table.
typedef struct symbol {
	char label[MAXLINELENGTH];
	int address;
	int value;
} symbol_t;
symbol_t symbol_table[MAXLINELENGTH];

/**
 * @brief find address of label.
 * @param label 
 * @return label address.
 */
int calc_label(char* label) {
	for (int i = 0; i < MAXLINELENGTH; i++) {
		if (!strcmp(symbol_table[i].label, label)) {
			return symbol_table[i].address;
		}
	}
	printf("error: %s label doesn't exist.\n", label);
	exit(1);
}

/**
 * @brief return 1 if label is exist.
 * @param label
 * @return 1 if label is exist.
 */ 
int is_exist_label(char* label) {
	for (int i = 0; i < MAXLINELENGTH; i++) {
		if (!strcmp(symbol_table[i].label, label)) {
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/**
	 * Phase 1, calculate the address of the label.
	 */
	int address = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		if (label[0] != '\0') {
			if (is_exist_label(label)) {
				printf("error: %s label already exists.\n", label);
				exit(1);
			}
			strcpy(symbol_table[address].label, label);
			symbol_table[address].address = address;
		}
		address++;
	}

	/**
	 * Phase 2, translate the assembly code.
	 */
	address = 0;
	rewind(inFilePtr);
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		/**
		 * opcode is "add", "nor", "lw", "sw", "beq", "jalr", "halt", or "noop" 
		 * below codes are opcode switching part.
		 */
		int inst = 0;

   		// 000
		if (!strcmp(opcode, "add")) {
			inst |= (0 << 22);
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
			inst |= (atoi(arg2) << 0);
		}
    	// 001
		else if (!strcmp(opcode, "nor")) {
			inst |= (1 << 22);
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
			inst |= (atoi(arg2) << 0);
		}
	  	// 010
    	else if (!strcmp(opcode, "lw")) {
			inst |= (2 << 22);
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
			if (isNumber(arg2) && ((1 << 15) <= atoi(arg2) || -(1 << 15) > atoi(arg2))) {
				printf("error: offsetField is out of range.\n");
				exit(1);
			}
			inst |= (isNumber(arg2) ? atoi(arg2) : calc_label(arg2));
		}
		// 011
    	else if (!strcmp(opcode, "sw")) {
			inst += 3 << 22;
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
			if (isNumber(arg2) && ((1 << 15) <= atoi(arg2) || -(1 << 15) > atoi(arg2))) {
				printf("error: offsetField is out of range.\n");
				exit(1);
			}
			inst |= (isNumber(arg2) ? atoi(arg2) : calc_label(arg2));
		}
		// 100
		else if (!strcmp(opcode, "beq")) {
			inst |= 4 << 22;
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
			if (isNumber(arg2) && ((1 << 15) <= atoi(arg2) || -(1 << 15) > atoi(arg2))) {
				printf("error: offsetField is out of range.\n");
				exit(1);
			}
			inst |= (isNumber(arg2) ? atoi(arg2) : (unsigned short)(calc_label(arg2) - address - 1));
		}
		// 101
    	else if (!strcmp(opcode, "jalr")) {
			inst |= 5 << 22;
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: arg0, arg1 must be a number.\n");
				exit(1);
			}
			if ((atoi(arg0) < 0 || 7 < atoi(arg0)) || (atoi(arg1) < 0 || 7 < atoi(arg1))) {
				printf("error: arg0, arg1 must be 0 <= arg0, arg1 <= 7.\n");
				exit(1);
			}
			inst |= (atoi(arg0) << 19);
			inst |= (atoi(arg1) << 16);
		}
    	// 110
		else if (!strcmp(opcode, "halt")) {
			inst |= 6 << 22;
		}
		// 111
    	else if (!strcmp(opcode, "noop")) {
			inst |= 7 << 22;
		}
		else if (!strcmp(opcode, ".fill")) {
			inst = isNumber(arg0) ? atoi(arg0) : calc_label(arg0);
		}
		else {
			printf("error: unrecognized opcode %s.\n", opcode);
			exit(1);
		}
		fprintf(outFilePtr, "%d\n", inst);
		address++;
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}
