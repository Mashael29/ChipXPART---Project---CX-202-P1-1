#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//used functions
void assembler(char *inputFilename, char *outputFilename);
void instruction_LUT(char *line);

//Get the file name by command line 
int main(int argc ,char *argv[]) {
    (void) argc; // can not delete the int beacuse of the main declration and there is no use of it , so voided
    char *filename = argv[1];
    // Set the output filename
    char outputFilename[105];//slightly larger than input 
    snprintf(outputFilename, sizeof(outputFilename), "%s.bin", filename);

    // Assemble the file
    assembler(filename, outputFilename);

    return 0;
  
}

// Function to assemble the file into binary output
void assembler(char *inputFilename, char *outputFilename) {
    int maxlen = 20;//maximum line length
    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error! Could not open file %s.\n", inputFilename);
        return;
    }

    FILE *outputFile = fopen(outputFilename, "wb");  // wb= bainary mode
    if (outputFile == NULL) {
        fprintf(stderr, "Error! Could not open output file %s.\n", outputFilename);
        fclose(inputFile);
        return;
    }

    char line[maxlen];
    printf("Starting Assembler...\n");

    // Read instruction from the file
    while (fgets(line, sizeof(line), inputFile)) {
        // Deleting spacing and newline 
	line[strcspn(line,"\n")] = '\0';
	line[strcspn(line, " ")] = '\0';

        // instruction to binary using the LUT
        instruction_LUT(line);

        // Write the binary code to the output file in binary format
        fwrite(line, 1, strlen(line), outputFile);
        fwrite("\n", 1, 1, outputFile);
    }

    printf("Assembly completed successfully. Output written to %s\n", outputFilename);

    fclose(inputFile);
    fclose(outputFile);}

void instruction_LUT(char *line) {
    // Look UP Table for instructions and their binary codes
	int numofinstruction = 9;
	int immediate;
	char instructions[9][2][20] = {
        {"RA=RA+RB", "00000000"},
        {"RB=RA+RB", "00010000"},
        {"RA=RA-RB", "00000100"},
        {"RB=RA-RB", "00010100"},
        {"RO=RA",    "00100000"},
        {"RA=imm",   "00001000"},
        {"RB=imm",   "00011001"},
        {"JC=imm",   "01110000"},
        {"J=imm",    "10110010"}
    };
		    
    // Loop through the LUT to find a match 
	for (int i = 0; i < numofinstruction; i++){
		char *search =strstr(instructions[i][0],"imm");//
                if (search != NULL){
		 int baseLength = strlen(instructions[i][0]) - strlen("imm");
                 if (strncmp(line, instructions[i][0], baseLength) == 0) {
                // 
                immediate = atoi(line + baseLength);

                // 
                char binaryCode[25]; //Add more for the binary to avoid truncation warning
                snprintf(binaryCode, sizeof(binaryCode), "%s%d", instructions[i][1], immediate);

                // Replace the line with the final binary code
                strcpy(line, binaryCode);
                return;
            }}
		
		else if (strcmp(line, instructions[i][0]) == 0) {  // lookup and match
            	strcpy(line, instructions[i][1]);  // Replace with binary code
            	return;
        }
		
		
    }

    // Print an error if not found
    printf("Error: Unrecognized instruction '%s'\n", line);
}

