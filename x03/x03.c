/*
Read from a binary file a square matrix of integers.
Store the size of the matrix in a double pointer.
The filename is passed by argv.
Then we reduced the matrix.

CONSOLE --> gcc p0t3.c -o p0t3 -lm && ./p0t3 file_entry
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t3 file_entry\n");
        exit(-1);
    }
}

int** createMatrix(int *ROWS, int *COLUMNS){ // CREATE DYNAMIC MATRIX
    
    int **matrix;
    matrix = (int**)malloc(*ROWS * sizeof(int*));
    for (int i = 0; i < *ROWS; i++) {
        matrix[i] = (int*)malloc(*COLUMNS * sizeof(int));
    }
    return matrix;
}

void readFile (int **matrix, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < *ROWS; i++) {
            int bytes_read = fread(matrix[i], sizeof(int), *COLUMNS, f);  
        }
        fclose(f);
    }
}

void writeBinaryFile (int **matrix, int *ROWS, int *COLUMNS){

    FILE *f = fopen("exitOriginalMatrix.bin", "wb");

    for (int i = 0; i < *ROWS; i++) {
        size_t r1 = fwrite(matrix[i], sizeof(int), *COLUMNS, f);
     }           
    fclose(f);
    
}

void writeTextFile (int **matrix, int *ROWS, int *COLUMNS){

    FILE *f = fopen("exitOriginalMatrix.txt", "w");
    
    for(int i = 0; i < *ROWS; i++){
        for(int j = 0; j < *COLUMNS; j ++){
            fprintf(f, "%i ", matrix[i][j]);
        }
       fprintf(f, "\n");
    }
}

int main ( int argc, char *argv[] ){ //CONSOLE --> main file_entry
    
    argumentsOK(argc);
 
    FILE *f = fopen(argv[1], "rb"); //OPEN FILE

    fseek(f, 0, SEEK_END ); // LET'S PUT THE CURSOR IN THE LAST POSITION
    int bytes_total = ftell(f); // WHERE IS THE CURSOR
    fseek(f, 0, SEEK_SET); // MOVE CURSOR TO FIRST POSITION, [0]
    printf ("Bytes totals: %i\n", bytes_total);

    int num_elements = bytes_total / 4;
    printf("Number Elements: %i\n", num_elements);

    int squareRoot = sqrt(num_elements);
    printf("Square Root: %i\n", squareRoot);
    
    int row = squareRoot;
    int col = squareRoot;
    
    int **matrix = createMatrix(&row, &col); // CREATE MATRIX
    
    if(f){

        readFile(matrix, argv[1], &row, &col);
        writeTextFile(matrix, &row, &col);
        writeBinaryFile (matrix, &row, &col);

        // COPIES WITH FEWER ROWS AND COLUMNS
        int row_red = row - 10;
        int col_red = col - 10;
        int **matrixReduced = createMatrix(&row_red, &col_red); // CREATE NEW MATRIX
        
        for(int i = 0; i < row_red; i ++){
            for (int j = 0; j < col_red; j++){
                memcpy (matrixReduced, matrix, row_red * sizeof(int));
            }
        }

        FILE *f = fopen("exitReducedMatrix.bin", "wb");
        for (int i = 0; i < row_red; i++) {
            size_t r1 = fwrite(matrixReduced[i], sizeof(int), col_red, f);
        }
        fclose(f);
        free(matrixReduced);
    }
    else {
        printf("Error: The file don't exist\n");
        return -1;
    }

    free(matrix);
}