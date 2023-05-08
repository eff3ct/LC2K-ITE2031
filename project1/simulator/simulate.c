/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

/**
 * @brief decompose machinary code into readable fragments.
 * 
 * @param machine_code mc
 * @param opcode opcode
 * @param reg0 regA
 * @param reg1 regB
 * @param reg2 destReg
 * @param offset offsetField
 */
void decompose_line(int machine_code, int* opcode, int* reg0, int* reg1, int* reg2, int* offset) {
    *opcode = machine_code >> 22; machine_code &= ~(*opcode << 22);
    *reg0 = machine_code >> 19; machine_code &= ~(*reg0 << 19);
    *reg1 = machine_code >> 16; machine_code &= ~(*reg1 << 16);
    *offset = convertNum(machine_code);
    *reg2 = (machine_code & 7);
}

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    /**
     * simulator part.
     */

    // init regs
    for(int i = 0; i < NUMREGS; ++i) state.reg[i] = 0;

    // decompose mc and simulate.
    state.pc = 0;
    int opcode, reg0, reg1, reg2, offset, delta, is_end = 0, inst_cnt = 0;
    while(1) {
        delta = 1;
        printState(&state);

        if (is_end) break;

        inst_cnt++;
        decompose_line(state.mem[state.pc], &opcode, &reg0, &reg1, &reg2, &offset);

        switch (opcode) {
            case 0:
                state.reg[reg2] = state.reg[reg0] + state.reg[reg1];
                break;
            case 1:
                state.reg[reg2] = ~(state.reg[reg0] | state.reg[reg1]);
                break;
            case 2:
                state.reg[reg1] = state.mem[ state.reg[reg0] + offset ];
                break;
            case 3:
                state.mem[ state.reg[reg0] + offset ] = state.reg[reg1];
                break;
            case 4:
                if (state.reg[reg0] == state.reg[reg1]) delta += offset;
                break;
            case 5:
                state.reg[reg1] = state.pc + 1;
                state.pc = state.reg[reg0];
                delta = 0;
                break;
            case 6:
                is_end = 1;
                printf("machine halted.\n");
                printf("total of %d instructions executed.\n", inst_cnt);
                printf("final state of machine:\n");
                break;
            case 7:
                break;        
        }

        state.pc += delta;
    }

    exit(0);

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
