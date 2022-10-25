#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1200

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t10 filename\n");
        exit(-1);
    }
}

double **createMatrix(){ // CREATE DYNAMIC MATRIX
    double **matrix;
    matrix = (double**)malloc(N * sizeof(double*));
    for(int i = 0; i < N; i ++){
        matrix[i] = (double*)malloc(N * sizeof(double));
    }
    return matrix;
}

double *createArray(){ // CREATE DYNAMIC ARRAY
    double *arrayIdentity, *arrayResult;
    arrayIdentity = (double*)malloc(N * sizeof(double));
    arrayResult = (double*)malloc(N * sizeof(double));
    return arrayIdentity, arrayResult;
}

void AddValueArrayIdentity(double *arrayIdentity){ // FILL ARRAY VALUE 1
    for(int i = 0; i < N; i++){
        arrayIdentity[i] = (double) 1;
    }
}

void readFile (double **matrix, char *file_entry){ // READ MATRIX BY ROWS
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < N; i++) {
            int bytes_read = fread(matrix[i], sizeof(double), N, f);  
        }
        fclose(f);
    }
}

void multiplication(double **matrix, double *arrayIdentity, double *arrayresult){

    long double aux;
    for(int i = 0; i < N; i ++){
        aux = 0;
        for(int j = 0; j < N; j ++){
            aux += matrix[i][j] * arrayIdentity[i];
        }
        arrayresult[i] = aux;
    }
}

void absoluteValue(double *arrayResult){

    double absHigher = 0;
    int index;

    for (int j = 0 ; j < N ; j ++ ){ //CALCULATE THE GREATEST ABSOLUTE
        if( abs(arrayResult[j]) > absHigher ){ // IF THE ABSOLUTE OF X IS GREATER TO GREATER
            absHigher = abs(arrayResult[j]); // SAVE THE ABSOLUTE IN VARIABLE 1
            index = j;
        }
    }
 
    printf("INDEX: %i --> HIGHER ABS %.1f\n", index, absHigher);

}

int main(int argc, char *argv[]){ // ./p0t10 mat.bin
    
    argumentsOK(argc);

    double **matrix = createMatrix();
    double *arrayIdentity = createArray();
    double *arrayResult = createArray();
    
    AddValueArrayIdentity(arrayIdentity);

    readFile(matrix, argv[1]);
    FILE *f = fopen(argv[1], "rb");

    multiplication(matrix, arrayIdentity, arrayResult);
    
    absoluteValue(arrayResult);

    free(matrix); free(arrayIdentity); free(arrayResult);

    return 0;
}