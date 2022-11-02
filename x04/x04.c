/*
Read a square matrix unsigned char and store it using double pointer.
Store in a array the average of each row.
Store in other array the average of each column.
Write max and min.

CONSOLE --> gcc p0t4.c -o p0t4 -lm && ./p0t4 file_entry
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t4 filename\n");
        exit(-1);
    }
}

unsigned char** createMatrix(int *ROWS, int *COLUMNS){ // CREATE DYNAMIC MATRIX
    unsigned char **matrix;
    matrix = (unsigned char**) malloc(*ROWS * sizeof(unsigned char*));
    for(int i = 0; i < *ROWS; i ++){
        matrix[i] = (unsigned char*) malloc(*COLUMNS *sizeof(unsigned char));
    }
    return matrix;
}

void readFile(unsigned char **matrix, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < *ROWS; i++) {
            int bytes_read = fread(matrix[i], sizeof(unsigned char), *COLUMNS, f);  
        }
        fclose(f);
    }
}

void averageRows(unsigned char **matrix, int *ROWS, int *COLUMNS){
   
    int average, total;
    int array[*ROWS];

    for(int i = 0; i < *ROWS; i ++){
        total = 0;
        for(int j = 0; j < *COLUMNS; j ++){ 
            total += matrix[i][j];
        }
        average = total / *COLUMNS;
        array[i] = average;  
    }
    
    int higher = array[0], less = array[0];
    for(int i = 0; i < *ROWS; i ++){
        if (array[i] > higher){
            higher = array[i];
        }

        if (array[i] < less){
            less = array[i];
        }
    }
    
    FILE *f = fopen("exitData.txt", "w");
    fprintf(f, "----ROWS----\n");
    fprintf(f, "Higher: %i\n", higher);
    fprintf(f, "Less: %i\n", less);
}

void averageColumns(unsigned char **matrix, int *ROWS, int *COLUMNS){
   
    int average, total;
    int array[*COLUMNS];

    for(int i = 0; i < *COLUMNS; i ++){
        total = 0;
        for(int j = 0; j < *ROWS; j ++){ 
            total += matrix[j][i];
        }
        average = total / *COLUMNS;
        array[i] = average;  
    }

    int higher = array[0], less = array[0];
    for(int i = 0; i < *COLUMNS; i ++){
        if (array[i] > higher){
            higher = array[i];
        }

        if (array[i] < less){
            less = array[i];
        }
    }
    
    FILE *f = fopen("exitData.txt", "w");
    fprintf(f, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    fprintf(f, "----COLUMNS----\n");
    fprintf(f, "Higher: %i\n", higher);
    fprintf(f, "Less: %i\n", less);
  
}

int sizeMatrix(char *file_entry){
    
    FILE *f = fopen(file_entry, "rb");

    fseek(f, 0, SEEK_END ); // LET'S PUT THE CURSOR IN THE LAST POSITION
    int bytes_total = ftell(f); // WHERE IS THE CURSOR
    fseek(f, 0, SEEK_SET); // MOVE CURSOR TO FIRST POSITION, [0]

    int num_elements = bytes_total / 4;
    int squareRoot = sqrt(num_elements);

    return squareRoot;
}

int main(int argc, char *argv[]){

    argumentsOK(argc);
      
    int row = sizeMatrix(argv[1]);
    int col = row;   

    unsigned char **matrix = createMatrix(&row, &col);

    readFile(matrix, argv[1], &row, &col);

    averageRows(matrix, &row, &col);
    averageColumns(matrix, &row, &col);  

    free(matrix);
    return 0;
}