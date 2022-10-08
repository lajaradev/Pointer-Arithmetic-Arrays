#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** createMatrix(int *ROWS, int *COLUMNS){ // CREATE DYNAMIC MATRIX
    
    double **matrixRR;
    double **matrixEE;
    
    matrixRR = (double**)malloc(*ROWS * sizeof(double*));
    matrixEE = (double**)malloc(*ROWS * sizeof(double*));

    for (int i = 0; i < *ROWS; i++) {
        matrixRR[i] = (double*)malloc(*COLUMNS * sizeof(double));
        matrixEE[i] = (double*)malloc(*COLUMNS * sizeof(double));
    }
    
    return matrixRR, matrixEE;
}

void readFileRows (double **matrixRR, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < *ROWS; i++) {
            int bytes_read = fread(matrixRR[i], sizeof(double), *COLUMNS, f);  
        }
        fclose(f);
    }
}

void readFileElements (double **matrixEE, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ELEMENTS
    int accountant = 0;
    FILE *f = fopen(file_entry, "rb");
    if(f) {
        for (int i = 0; i < *ROWS; i++) {
            for (int j = 0; j < *COLUMNS; j++){
                int bytes_read = fread(&matrixEE[i][j], sizeof(double), 1, f);  
            }
            accountant = accountant + *COLUMNS;
        }
        fclose(f);
    }
}

void writeFileText (double **matrixRR, double **matrixEE, int *ROWS, int *COLUMNS){

    FILE *f, *memory;
    unsigned long long int addresses;

    f = fopen("exit5x2.txt", "w");
    
    fprintf(f, "DATA ROW BY ROW: \n");
    for(int i = 0; i < *ROWS; i++){
        for(int j = 0; j < *COLUMNS; j ++){
            fprintf(f, "%.5f ", matrixRR[i][j]);
        }
       fprintf(f, "\n");
    }
    fprintf(f, "\n");
   
    fprintf(f, "MEMORY ADDRESSES ROW BY ROW: \n");
    for(int i = 0; i < *ROWS; i++){
        for(int j = 0; j < *COLUMNS; j ++){ 
            if(j == 0){
                addresses = (unsigned long long int) &matrixRR[i][j];
                fprintf(memory, "%llu ", addresses);
            }
            else if(j == *COLUMNS - 1){
                addresses = (unsigned long long int) &matrixRR[i][j];
                fprintf(memory, "%llu\n", addresses);
            }
        }
    }
    fprintf(f, "\n\n");

    fprintf(f, "DATA ELEMENT BY ELEMENT: \n");
    for(int i = 0; i < *ROWS; i++){
        for(int j = 0; j < *COLUMNS; j ++){
            fprintf(f, "%.5f ", matrixEE[i][j]);
        }
       fprintf(f, "\n");
    }
    fprintf(f, "\n");
   
    fprintf(f, "MEMORY ADDRESSES ELEMENT BY ELEMENT: \n");
    for(int i = 0; i < *ROWS; i++){
        for(int j = 0; j < *COLUMNS; j ++){ 
            if(j == 0){
                addresses = (unsigned long long int) &matrixEE[i][j];
                fprintf(memory, "%llu ", addresses);
            }
            else if(j == *COLUMNS - 1){
                addresses = (unsigned long long int) &matrixEE[i][j];
                fprintf(memory, "%llu\n", addresses);
            }
        }
    }
    fprintf(f, "\n\n");
}

int main ( int argc, char *argv[] ){ //CONSOLE --> ./p0t2 file_entry rows columns

    int row = atoi(argv[2]);
    int col = atoi(argv[3]);

    double **matrixRR = createMatrix(&row, &col); // CREATE MATRIX 
    double **matrixEE = createMatrix(&row, &col); // CREATE MATRIX
  
    FILE *f = fopen(argv[1], "rb"); // OPEN FILE BY ARGUMENT

    if(f){
        readFileRows(matrixRR, argv[1], &row, &col); 
        readFileElements(matrixEE, argv[1], &row, &col);
        writeFileText(matrixRR, matrixEE, &row, &col);
    }
    else {
        printf("Error: The file don't exist\n");
        return -1;
    }

    free(matrixRR);
    free(matrixEE);
    return 0;
   
}
