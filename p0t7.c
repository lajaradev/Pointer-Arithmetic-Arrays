/*
Read from binary file a square matrix unsigned int. Store in double pointer
Firts calculate average of firts row.
After row by row count all elements higher that average:
If the number is obtained is greater than half of the row size, this is the new average
If the number is minor a 5% of the row size, ends process.

CONSOLE --> gcc p0t7.c -o p0t7 && ./p0t7 file_entry
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t7 filename\n");
        exit(-1);
    }
}

int sizeMatrix(char *file_entry){ //unsigned int = 4 bytes
    
    FILE *f = fopen(file_entry, "rb");

    fseek(f, 0, SEEK_END ); // LET'S PUT THE CURSOR IN THE LAST POSITION
    int bytes_total = ftell(f); // WHERE IS THE CURSOR
    fseek(f, 0, SEEK_SET); // MOVE CURSOR TO FIRST POSITION, [0]

    int num_elements = bytes_total / 4;
    int squareRoot = sqrt(num_elements);

    return squareRoot;
}

unsigned int** createMatrix(int *ROWS, int *COLUMNS){ // CREATE DYNAMIC MATRIX
    
    unsigned int **matrix;    
    matrix = (unsigned int**)malloc(*ROWS * sizeof(unsigned int*));
    for (int i = 0; i < *ROWS; i++) {
        matrix[i] = (unsigned int*)malloc(*COLUMNS * sizeof(unsigned int));
    }
    return matrix;
}

void readFile (unsigned int **matrix, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < *ROWS; i++) {
            int bytes_read = fread(matrix[i], sizeof(unsigned int), *COLUMNS, f);  
        }
        fclose(f);
    }
}

int main(int argc, char *argv[]){

    argumentsOK(argc);

    int row = sizeMatrix(argv[1]);
    int col = row;  
    int repeat = 0;

    unsigned int **matrix = createMatrix(&row, &col);

    readFile(matrix, argv[1], &row, &col);
  
    unsigned int avgValue = 0;
    for(int j = 0; j < col; j ++){
        avgValue = avgValue + matrix[0][j] ;
    }
    avgValue = avgValue / col; 
    repeat ++;
    printf("Average First Row: %i\n", avgValue);


    int counter = 0;
    for(int i = 1; i < row; i ++){   //START ROW 
        for(int j = 0; j < col; j ++){
            
            if(matrix[i][j] > avgValue){
                counter ++;
            }

        }
                
        if(counter > (col / 2)){
            
            for(int j = 0; j < col; j ++){
                avgValue = avgValue + matrix[i][j];
            }
            avgValue = avgValue / col;     
            repeat ++;  
              
        }
        else if(counter < (0.05 * col)){
            break;
        }
       
       counter = 0;
       
    }

    printf("Repeat: %i\n", repeat); 


    return 0;
}