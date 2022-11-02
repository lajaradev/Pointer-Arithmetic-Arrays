#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t8 filename\n");
        exit(-1);
    }
}

int sizeMatrix(char *file_entry){ //unsigned char = 1 bytes
    
    FILE *f = fopen(file_entry, "rb");

    fseek(f, 0, SEEK_END ); // LET'S PUT THE CURSOR IN THE LAST POSITION
    int bytes_total = ftell(f); // WHERE IS THE CURSOR
    fseek(f, 0, SEEK_SET); // MOVE CURSOR TO FIRST POSITION, [0]
    int squareRoot = sqrt(bytes_total);
  
    return squareRoot;
}

unsigned char** createMatrix(int *ROWS, int *COLUMNS){ // CREATE DYNAMIC MATRIX
    
    unsigned char **matrixOriginal;
    unsigned char **matrixFiltered;
    
    matrixOriginal = (unsigned char**)malloc(*ROWS * sizeof(unsigned char*));
    matrixFiltered = (unsigned char**)malloc(*ROWS * sizeof(unsigned char*));

    for (int i = 0; i < *ROWS; i++) {
        matrixOriginal[i] = (unsigned char*)malloc(*COLUMNS * sizeof(unsigned char));
        matrixFiltered[i] = (unsigned char*)malloc(*COLUMNS * sizeof(unsigned char));
    }
    
    return matrixOriginal, matrixFiltered;
}

void readFile (unsigned char **matrix, char *file_entry, int *ROWS, int *COLUMNS){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < *ROWS; i++) {
            int bytes_read = fread(matrix[i], sizeof(unsigned char), *COLUMNS, f);  
        }
        fclose(f);
    }
}

void writeFile(unsigned char **matrix, int *ROWS, int *COLUMNS){
    FILE *f = fopen("filteredImage.raw", "wb");
    for (int i = 0; i < *ROWS; i++) {
                size_t r1 = fwrite(matrix[i], sizeof(unsigned char), *COLUMNS, f);
     }
  fclose(f);
}

void averageMatrix(unsigned char **matrixOriginal, unsigned char **matrixFiltered, int *ROWS, int *COLUMNS){

        /*SHOW ORIGINAL MATRIX 10 ELEMENTS*/ // printf("\n\nORIGINAL MATRIX\n\n"); for(int i = 0; i < 10; i++){  for(int j = 0; j < 10;j++){   printf("%i   ",matrixOriginal[i][j]);  }   printf("\n"); }    
     
        for(int i = 0; i < *ROWS; i++){ // COPY BORDERS
            memcpy(matrixFiltered[i], matrixOriginal[i], *ROWS * sizeof(char) );}
            
        for(int j = 0; j < *COLUMNS; j++){ // COPY BORDERS
             memcpy(matrixFiltered[j], matrixOriginal[j], *COLUMNS * sizeof(char) );}

        for(int i = 1; i < *ROWS; i++){ // BEGIN THE BLUCLE TO DO THE CALCULATIONS
            for (int j = 1; j < *COLUMNS; j++){
            
                if (  (i) > 0 && (j) > 0 && (i+1) < *ROWS && (j+1) < *COLUMNS  ) {
              // COPY THE VALUE THAT IS IN THE POSITIONS
                int topLeft = (int) matrixOriginal[i-1][j-1];
                int topCenter = (int) matrixOriginal[i-1][j];
                int topRight = (int) matrixOriginal[i-1][j+1];
                int centerLeft = (int) matrixOriginal[i][j-1];
                int centerCenter = (int) matrixOriginal[i][j];
                int centerRight = (int) matrixOriginal[i][j+1];
                int downLeft  = (int) matrixOriginal[i+1][j-1];
                int downCenter  = (int) matrixOriginal[i+1][j];
                int downRight  = (int) matrixOriginal[i+1][j+1];
            
                int average = (topLeft + topCenter + topRight + centerLeft + centerCenter + centerRight + downLeft + downCenter + downRight) / 9;         
                matrixFiltered[i][j] = average;  // IN THE POSITION I COPY THE VALUE OF THE CALCULATED AVERAGE
                }       
            }
        }
        
        /*SHOW FILTERED MATRIX 10 ELEMENTS*/ // printf("\n\nFILTERED MATRIX \n\n"); for(int i = 0; i < 10; i++){  for(int j = 0; j < 10;j++){   printf("%i   ",matrixFiltered[i][j]);  } printf("\n");   }   
        
        writeFile(matrixFiltered, ROWS, COLUMNS);

}

int main(int argc, char *argv[]){

    argumentsOK(argc);

    int row = sizeMatrix(argv[1]);
    int col = row;  

    unsigned char **matrixOriginal = createMatrix(&row, &col);
    unsigned char **matrixFiltered = createMatrix(&row, &col);

    readFile(matrixOriginal, argv[1], &row, &col);
    averageMatrix(matrixOriginal, matrixFiltered, &row, &col);

    free(matrixOriginal);
    free(matrixFiltered);

    return 0;
}