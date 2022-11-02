#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct times{ // CONTROL OF EXECUTION TIMES
    int create;
    clock_t run_start;
    clock_t run_end;
}*Times;

void argumentsOK(int argc){
     if(argc != 3){
        printf("./x09 filename quicksort/bubble\n");
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

void exchangeQuicksort(int *V, unsigned int l, unsigned int r){ 
    int aux;
    aux = V[l];
    V[l] = V[r];
    V[r] = aux;
}

void exchangeBubble(int *a, int *b) {
  int aux = *a;
  *a = *b;
  *b = aux;
}

void bubble(int *V, int length) { 
  for (int i = 0; i < length; i ++) {
    for (int j = 0; j < length - 1; j ++) {
      int k = j + 1;
      if (V[j] > V[k]) {
        exchangeBubble(&V[j], &V[k]);
      }
    }
  }
}

void quicksort(int *V, unsigned int left, unsigned int right){ // SORT A ARRAY

    unsigned int l, r, p;
    int pivot;

    p = (left + right) / 2;;

    if(p > 0){
        pivot = V[p];
        l = left;
        r = right;

        while(l <= r){
            while(V[l] < pivot)
                l++;

            while(V[r] > pivot)
                r--;

            if(l <= r){
                exchangeQuicksort(V, l, r);

                l++;
                r--;
            }
        }

        if(left < r)
            quicksort(V, left, r);
        if(l < right)
            quicksort(V, l, right);
    }
}

void medianMatrix(unsigned char **matrixOriginal, unsigned char **matrixFiltered, int *ROWS, int *COLUMNS, char *opt, Times times){

        /*SHOW ORIGINAL MATRIX 10 ELEMENTS*/ //printf("\n\nORIGINAL MATRIX\n\n"); for(int i = 0; i < 10; i++){  for(int j = 0; j < 10;j++){   printf("%i   ",matrixOriginal[i][j]);  }   printf("\n"); }    

        times -> run_start = clock();

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

                int V[9] = {topLeft, topCenter, topRight, centerLeft, centerCenter, centerRight, downLeft, downCenter, downRight};

                if(strcmp(opt, "quicksort") == 0){
                    quicksort(V,0,9); // ORDER ARRAY POSITION [0 - 8]
                }
                else if(strcmp(opt, "bubble") == 0){
                    bubble(V, 9);
                }
               
                unsigned int median = V[4]; // TAKE CENTER VALUE

                matrixFiltered[i][j] = median; // IN THE POSITION I COPY THE VALUE OF THE CALCULATED AVERAGE          
                }          
            }
        }
        
        /*SHOW FILTERED MATRIX 10 ELEMENTS*/ // printf("\n\nFILTERED MATRIX \n\n"); for(int i = 0; i < 10; i++){  for(int j = 0; j < 10;j++){   printf("%i   ",matrixFiltered[i][j]);  } printf("\n");   }   
       
        writeFile(matrixFiltered, ROWS, COLUMNS);
        
        times -> run_end = clock();

        double run_times = (double)(times -> run_end - times -> run_start) / CLOCKS_PER_SEC;
        printf(" RUN TIME: %fs.\n ", run_times);

}

int main(int argc, char *argv[]){ // CONSOLE --> gcc x09.c -o x09 -lm && ./x09 lena512x512.bin

    argumentsOK(argc);

    int row = sizeMatrix(argv[1]);
    int col = row;  

    unsigned char **matrixOriginal = createMatrix(&row, &col);
    unsigned char **matrixFiltered = createMatrix(&row, &col);

    readFile(matrixOriginal, argv[1], &row, &col);

    Times times = (struct times*)malloc(sizeof(struct times));

    medianMatrix(matrixOriginal, matrixFiltered, &row, &col, argv[2], times);

    free(matrixOriginal);
    free(matrixFiltered);

    return 0;
}