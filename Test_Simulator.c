#include <stdio.h>
#include <stdlib.h>

#define Max_Instructions 8

char RA = 0;              // Register A
char RB = 0;              // Register B
char R0 = 0;              // Register 0
char Program_Counter = 0; // Program counter
char mode;

void load_program(const char *InputFile, char program[], int *program_size) {
    FILE *MyFile = fopen(InputFile, "rb");

    if (MyFile == NULL) {  // Check if the file opened successfully
        printf("Could not open this binary file\n");
        return;
    }

    *program_size = fread(program, sizeof(char), Max_Instructions, MyFile); // Load binary file into program array
    fclose(MyFile);

    printf("Program loaded with size %d\n", *program_size); // For debugging
}

char instraction_decode(char instruction,char *J ,char *C ,char *D1 , char *D0, char *Sreg,char *S)
{
        if(instruction & 0x80 )
        {*J=1;}
        else
        {*J=0;}
        if(instruction & 0x40)
        {*C=1; }
        else
        {*C=0;}
        if(instruction & 0x20)
        {*D1=1;}
        else
        {*D1=0;}
        if(instruction & 0x10)
        { *D0=1; }
        else
        {*D0=0;}
        if(instruction & 0x08)
        {*Sreg=1; }
        else
        {*Sreg=0;}
        if(instruction & 0x04)
        {*S=1;}
        else
        {*S=0;}
}

int ALU(int *Sum, int *carry,int operation_S)  //operation_S : to choice between Add ,Sub
{
        if(operation_S == 0) //Addition 
        {
                *Sum = RA + RB;
                if(*Sum>15)
                {
                        *carry=1; //Set carry if overflow 
                }
                else
                {
                        *carry=0; //Set carry if underflow 
                }
        }
        else if(operation_S == 1) //Subtraction
        {
                *Sum = (RA - RB);
                if(*Sum>15)
                {
                        *carry=1;
                }
                else
                {
                        *carry=0; //Set carry if underflow 
                }
        }
            return *Sum ;  // Return only the lower 4 bits 
}


void execute_instruction(char J, char C, char D1, char D0, char Sreg, char S) {
    if (Sreg == 0 && S == 0) { // RA = RA + RB
        if (D1 == 0 && D0 == 0) {
            RA = RA + RB;
            printf("RA = RA + RB => RA = %d\n", RA);
        }
    }
    else if (Sreg == 0 && S == 1) { // RA = RA - RB
        if (D1 == 0 && D0 == 0) {
            RA = RA - RB;
            printf("RA = RA - RB => RA = %d\n", RA);
        }
    }
    else if (Sreg == 1 && S == 0) { // RO = RA (push A to output)
        R0 = RA;
        printf("RO = RA => R0 = %d\n", R0);
    }
}

void handle_jump(char J, char C, int carry) {
    if (J && (C == 0 || carry)) {
        Program_Counter = Program_Counter & 0x0F;  // jump to specific instruction
        printf("Jumping to instruction %d\n", Program_Counter);
    } else {
        Program_Counter++;
    }
}

int main() {
//	RA=12;
//	RB=12;
	Program_Counter=0;

	int carry=0;
	int Sum;


    const char *InputFile = "fibonacci.bin"; //binary file name
    char program[Max_Instructions];
    int program_size;

    printf("Loading binary file: %s\n", InputFile);
    load_program(InputFile, program, &program_size);

    // Display loaded instructions for debugging
    for (int i = 0; i < program_size; i++) {
        printf("Instruction %d: %02X\n", i, program[i]);
    }

    char J, C, D1, D0, Sreg, S;
    for (int i = 0; i < program_size; i++) {
        instraction_decode(program[i], &J, &C, &D1, &D0, &Sreg, &S);
        printf("Decoded Instruction %d: J=%d, C=%d, D1=%d, D0=%d, Sreg=%d, S=%d\n", i, J, C, D1, D0, Sreg, S);
    }

    ALU(&Sum, &carry, 0); //addition
    printf("ALU Addition: RA=%d, RB=%d, Sum=%d, Carry=%d\n", RA, RB, Sum, carry);

    ALU(&Sum, &carry, 1); //subtraction
    printf("ALU Subtraction: RA=%d, RB=%d, Sum=%d, Carry=%d\n", RA, RB, Sum, carry);

     //  char J, C, D1, D0, Sreg, S;
    for (int i = 0; i < program_size; i++) {
        instraction_decode(program[i], &J, &C, &D1, &D0, &Sreg, &S);
        execute_instruction(J, C, D1, D0, Sreg, S); // Call execute function
    }
    handle_jump(J, C, carry); //Test jump
    printf("Program Counter after jump: %d\n", Program_Counter);

    return 0;
}
