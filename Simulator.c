#include<stdio.h>
#include<stdlib.h>
#define Max_Instructions 8 

char RA=0;
char RB=0;
char R0=0; 
char Program_Counter=0;
char mode;

void load_program(const char *InputFile, char program[], int *program_size)
{
	FILE *MyFile = fopen(InputFile , "rb");

	if(MyFile==NULL) //if the file did not open , print this massage
	{
		printf("Could not open this binary file\n");
		return ;
	}

	// fread(destination ,size of each element , number of elements , file pointer);
	// read data from the file (MyFile) into program array 
	*program_size = fread(program,sizeof( char),Max_Instructions,MyFile);

	printf("Program loaded with size %d\n",*program_size); // For debugging

	fclose(MyFile);
}

int ALU(int *Sum, int *carry,int operation_S)  //operation_S : to choice between Add ,Sub
{
	if(operation_S == 0) //Addition 
	{
		*Sum = (RA + RB);
		if(*Sum>15)
		{
			*carry=1;
		}
		else 
		{
			*carry=0;
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
			*carry=0;
		}
	}
	return *Sum;
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

char decode(char D0 , char D1 , char *enA, char *enB ,char *En0)
{
	if(D0==0 && D1==0)
	{
		*enA=1;
		*enB=0;
		*En0=0;
	}
	else if(D0==1 && D1 == 0)
	{
		*enA=0;
		*enB=1;
		*En0=0;
	}
	else if(D0==0 && D1 ==1)
	{
		*enA=0;
		*enB=0;
		*En0=1;
	}
	else if(D0==1 && D1==1)
	{
		*enA=0;
		*enB=0;
		*En0=0;
	}
}

char MUX(int Sum,char imm,char Sreg)
{

	if(Sreg==1) // if Sreg =1 , Select immediate value
	{
		return imm;
	}
	else 
	{
		return Sum; //if Sreg =0, Result from the ALU
	}
}

int execute_program(char *program, int program_size) 
{
	char J,C,D1,D0,Sreg,S;
	char enA,enB,En0;
	char imm=0;
	int carry=0;
	int Sum=0;
	int step=0;

printf("Starting Simulator in %s mode...\n", (mode == 'S') ? "step-by-step" : "continuous");

    while (Program_Counter < program_size) {
        char instruction = program[Program_Counter++];  // Fetch instruction

        instraction_decode(instruction, &J , &C , &D1 , &D0, &Sreg, &S);
        decode(D0, D1, &enA, &enB, &En0);  // Decode instruction
        int operation_S=S; //Use S as operation selector for ALU
        ALU(&Sum, &carry,operation_S);
	char Mux_output=MUX(Sum,imm,Sreg);

	//these two condation depend on the MUX function 
	if(enA)
	{
		RA=Mux_output;
	}
	if(enB)
	{
		RB=Mux_output;
	}

	 if (enA && !enB && !En0) { // RA = RA + RB
            RA = ALU(&Sum, &carry, 0);
            printf("Instruction %d: RA=RA+RB -> RA=%d\n", Program_Counter, RA);
        } else if (!enA && enB && !En0) { // RB = RA + RB
            RB = ALU(&Sum, &carry, 0);
            printf("Instruction %d: RB=RA+RB -> RB=%d\n", Program_Counter, RB);
        } else if (enA && !enB && En0) { // RA = RA - RB
            RA = ALU(&Sum, &carry, 1);
            printf("Instruction %d: RA=RA-RB -> RA=%d\n", Program_Counter, RA);
        } else if (!enA && enB && En0) { // RB = RA - RB
            RB = ALU(&Sum, &carry, 1);
            printf("Instruction %d: RB=RA-RB -> RB=%d\n", Program_Counter, RB);
        } else if (!enA && !enB && En0) { // RO = RA
            R0 = RA;
            printf("Instruction %d: RO=RA -> RO=%d\n", Program_Counter, R0);
        }


	 // Jump condition (if J is set)
        if (J && (C == 0 || carry)) {
            Program_Counter = instruction & 0x0F;  // Example jump to specific instruction
            printf("Jumping to instruction %d ", Program_Counter);
        }
	else
	{
		Program_Counter++;
	}
	  // Jump condition (if J is set)
    //    if (J==0 && (C == 0 || carry)) {
        //    Program_Counter = instruction & 0x0F;  // Example jump to specific instruction
      //      printf("Jumping to start %d ", Program_Counter+1);
    //    }

  //      printf("\n");

        // If step-by-step mode, pause until user presses Enter
        if (mode == 'S') {
            printf("[Press Enter to continue]");
           while (getchar() != '\n'); // Wait for Enter
        }
        step++;
    }

    printf("Execution completed.\n");
   
        
    }

//void handle_jump(char J, char C, int carry) {
//    if (J && (C == 0 || carry)) {
 //       Program_Counter = Program_Counter & 0x0F;  // Jump to specific instruction
//        printf("Jumping to instruction %d\n", Program_Counter);
 //   } else {
//        Program_Counter++;
//    }
//}


int main() 
{
//	int RA=5;
//	int RB=3;
    const char *InputFile = "fibonacci.bin";  // Specify the binary file name
    char program[Max_Instructions];
    int program_size;

    printf("Select mode (R - continuous, S - step-by-step): ");
    scanf(" %c", &mode);
    getchar();  // To consume the newline after mode input

    printf("Loading binary file: %s\n", InputFile);
    load_program(InputFile, program, &program_size);

    // Execute the loaded program
    execute_program(program, program_size);

  //  if(J==1 || C && *carry)
   // {
//	    program_counter=1
  //  }
   // else 
    //{
//	    program_counter=program_counter+1;
  //  }

    return 0;
}
