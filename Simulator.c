#include<stdio.h>
#include<stdlib.h>
#define Max_Instructions 16 // I choice random value

char RA=0;
char RB=0;
char R0=0; 
		    
char Program_Counter=0;

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
	*program_size = fread(program,sizeof( char),Max_Instruction,MyFile);
	fclose(file);
}

int ALU(int *Sum, int *carry,int operation_S)  //operation_S : to choice between Add ,Sub
{
	char RA;
	char RB;
	if(operation_S == 0)
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
	else if(operation_S == 1)
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
	return 0;
}
	
void instraction_decode(char instruction,char *J ,char *C ,char *D1 , char *D0, char *Sreg,char *S)
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

void decode(char D0 , char D1 , char *enA, char *enB ,char *En0)
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

void MUX(char Sum,char imm,char Sreg)
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

void execute_program(char *program, int program_size) 
{
	char J,C,D1,D0,Sreg,S;
	char enA,enB,En0;
	char imm=0;
	char carry=0;
	char sum=0;

    while (Program_Counter < program_size) {
        char instruction = program[Program_Counter++];  // Fetch instruction

        instraction_decode(instruction, &J , &C , &D1 , &D0, &Sreg, &S);
        decode(D0, D1, &enA, &enB, &En0);  // Decode instruction
					  //
        int operation_S=S; //Use S as operation selector for ALU
        ALU(&Sum, &carry,RA,RB,operation_S);
	char Mux_output=MUX(Sum,imm,Sreg);

	//these two condation depend on the MUX function 
	if(enA == 1)
	{
		RA=Mux_output;
		printf("RA update to: %d\n",RA);
	}
	if(enB==1)
	{
		RB=Mux_output;
		printf("RB update to: %d\n",RB);
	}



        // Execute based on the decode results and instruction fields
        if (enA && enB=0 && En0=0) {  // RA = RA + RB
            RA = ALU(&Sum,&carry,0);
            printf("Instruction %d: RA=RA+RB\n", Program_Counter);
        } else if (enA=0 && enB && En0=0) {  // RB = RA + RB
            RB = ALU(&Sum,&carry,0);
            printf("Instruction %d: RB=RA+RB\n", Program_Counter);
        } else if (enA && enB=0 && En0) {  // RA = RA - RB
            RA = ALU(&Sum,&carry,1);
            printf("Instruction %d: RA=RA-RB\n", Program_Counter);
        } else if (enA=0 && enB && En0) {  // RB = RA - RB
            RB = ALU(&Sum,&carry,1);
            printf("Instruction %d: RB=RA-RB\n", Program_Counter);
        } else if (enA=0 && enB=0 && En0) {  // RO = RA
            RO = RA;
            printf("Instruction %d: RO=RA -> RO=%u\n", Program_Counter, RO);
   
        if (RO == RA) {  // Only display RO when RO=RA
            printf("Execution (Register RO output): RO=%u\n", RO);

	if(J==0 && C==0 && D1==0 && D0==0 && Sreg==1)
	{
		RA=S;
		printf("instruction %d: RA=%u\n",Program_Counter,RA);
	}
	if(J==0 && C==0 && D1==0 && D0==1 && Sreg==1)
        {
                RB=S;
                printf("instruction %d: RB=%u\n",Program_Counter-1,RB);
        }
//	if(J==0 && C==1 && D1==1 && D0==1 && Sreg==0)
//        {
//               printf("instruction %d: JC=%u\n",Program_Counter,S);
//		if(C) Program_Counter=S;
//        }
//	if(J==1 && C==0 && D1==1 && D0==1 && Sreg==0)
//        {
//		RA=S;
//                printf("instruction %d: J=%u\n",Program_Counter-1,S);
//        }


        }
    }
}

int main() {
     char J,C,D1,D0,Sreg,S;
     char enA,enB,En0;

     const char *MyFile = InputFile;  
     char program[Max_Instructions];

     decode(D0, D1, &enA, &enB, &En0);  // Decode instruction
     ALU(Sum, carry);
     instraction_decode(instruction, &J , &C , &D1 , &D0, &Sreg, &S);

    //Run the loaded program
    execute_program(&program, &program_size);
    printf("Execution completed.\n");
    return 0;
}
