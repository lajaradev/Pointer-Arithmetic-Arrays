#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1200

void argumentsOK(int argc){
     
     if(argc != 1){
        printf("./ite\n");
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
    
    double *arrayIdentity;
    double *arrayResult;

    arrayIdentity = (double*)malloc(N * sizeof(double));
    arrayResult = (double*)malloc(N * sizeof(double));

    return arrayIdentity, arrayResult;

}

void AddValueArrayIdentity(double *arrayIdentity){ // FILL ARRAY VALUE 1
 
    for(int i = 0; i < N; i++){
        arrayIdentity[i] = (double) 1.0;
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

double absoluteValue(double *array){

    double absHigher = 0;
    int index;

    for (int i = 0 ; i < N; i ++ ){ //CALCULATE THE GREATEST ABSOLUTE
        if(fabs(array[i]) > absHigher ){ // IF THE ABSOLUTE OF X IS GREATER TO GREATER
            absHigher = array[i]; // SAVE THE ABSOLUTE IN VARIABLE 1
            index = i;
        }
    }
    printf("INDEX: %i --> HIGHER ABS %f\n", index, absHigher);
    return absHigher;
    
}

void showMatrix(double **matrix){  // SHOW ORIGINAL MATRIX 10 ELEMENTS

    printf("\n\nORIGINAL MATRIX\n\n"); 
    for(int i = 0; i < 10; i++){  
        for(int j = 0; j < 10;j++){   
            printf("%.2f   ",matrix[i][j]);  
        }   
        printf("\n"); } 
        
}

void showArray(double *array){  // SHOW ORIGINAL MATRIX 10 ELEMENTS

    printf("\nARRAY\n"); 
    for(int i = 0; i < 10; i++){  
        printf("%f   ",array[i]);  
        printf("\n"); } 
        
}

void valueMatrix(double **matrix){ // GIVE VALUES TO THE MATRIX
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i == j){ // DIAGONAL VALUES = 1
                matrix[i][j] = 1.0;
            }else if(i > j){ // POSITIVE VALUE (BOTTOM) 
                 matrix[i][j] = (double)50*(i+1)*(j+1)/(N+N);
            }else{ // NEGATIVE VALUE (TOP) 
                matrix[i][j] = (double)-50*(i+1)*(j+1)/(N+N);
            }
        }
    }
}

void iterations(double **matrix, int m){
    
    double *arrayX = createArray();
    AddValueArrayIdentity(arrayX);

    double *arrayY = createArray();
    int i = 0, j, t;
    double absHigher;
    double counter;

    printf("ITERATION --> %i\n", i);
    for(i = 0; i < N; i ++){ // FIRST ITERATION
        arrayY[i] = 0;
        for(j = 0; j < N; j ++){
            arrayY[i] += matrix[i][j] * arrayX[i];
        }
    }
   
    
   for(t = 1; t < m; t ++){ // ITERATIONS LOOP
    
    printf("ITERATION --> %i\n", t);
        for(i = 0; i < N; i ++){
            arrayX[i] = 0;
            for(j = 0; j < N; j ++){
                arrayX[i] += matrix[i][j] * arrayY[i];
            }
        }

        absHigher = absoluteValue(arrayX); 
        printf("VALUE: %f\n ", absHigher);

        for(i = 0; i < N; i ++){
            arrayY[i] = arrayX[i] / absHigher;
        }
      
       

       showArray(arrayX); 

  
    }
    
  
    
    free(arrayX); free(arrayY);
}


int main(int argc, char *argv[]){ // ./p0t10 mat.bin
    
    argumentsOK(argc);

    double **matrix = createMatrix();
   

    valueMatrix(matrix);
    showMatrix(matrix);
    
  
    iterations(matrix, 5);
    

   /* readFile(matrix, argv[1]);
    FILE *f = fopen(argv[1], "rb");

    multiplication(matrix, arrayIdentity, arrayResult);
    
    absoluteValue(arrayResult);*/

    free(matrix); 

    return 0;
}