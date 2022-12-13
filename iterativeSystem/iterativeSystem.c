#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define N 15000 // Size

/* Function to validate the number of arguments */
void argumentsOK(int argc){
     
     if(argc != 3){ // If argc is diferent of 3 argument --> error
        printf("./ite file iterations\n");
        exit(-1);
    }

}

double **createMatrix(){ // CREATE DYNAMIC MATRIX
    
    double **matrix;
    matrix = (double**)malloc(N * sizeof(double*)); // Reserve memory for rows
    for(int i = 0; i < N; i ++){
        matrix[i] = (double*)malloc(N * sizeof(double)); // Reserve memory for columns
    }
    return matrix;

}

double *createArray(){ // CREATE DYNAMIC ARRAY
    
    double *array;

    array = (double*)malloc(N * sizeof(double)); // Reserve memory for array
   
    return array;

}

void AddValueArrayIdentity(double *array){ // FILL ARRAY VALUE 1
 
    for(int i = 0; i < N; i++){
        array[i] = (double) 1; // All array = 1.0
    }

}

/*  In this function we open the file that we indicate by argument.
    Read the file by lines
    Store it in an matrix */
void readFile (double **matrix, char *file_entry){ // READ MATRIX BY ROWS
 
    FILE *f = fopen(file_entry, "rb");
    if(f) { 
        for (int i = 0; i < N; i++) {
            int bytes_read = fread(matrix[i], sizeof(double), N, f);  
        }
        fclose(f);
    }

}

/* In this function we open a new file and write inside by rows the values of a matrix */
void writeFile(double **matrix, char *file_entry){
    FILE *f = fopen(file_entry, "wb");
    for (int i = 0; i < N; i++) {
        size_t r1 = fwrite(matrix[i], sizeof(double), N, f);
    }
    fclose(f);
}

/* This function is used to show us a few data from the matrix of the image that we read */
void showMatrix(double **matrix){  // SHOW MATRIX 10 ELEMENTS

    printf("\n\nORIGINAL MATRIX\n\n"); 
    for(int i = 0; i < 10; i++){  
        for(int j = 0; j < 10;j++){   
            printf("%.2f   ",matrix[i][j]);  
        }   
        printf("\n"); } 
        
}

/* This function is used to show us a few data from the array of array */
void showArray(double *array){  // SHOW ARRAY 10 ELEMENTS

    printf("\nARRAY\n"); 
    for(int i = 0; i < 10; i++){  
        printf("%f   ",array[i]);  
        printf("\n"); } 
        
}

/* Assign random values to the new matrix */
void valueMatrix(double **matrix){ // GIVE VALUES TO THE MATRIX
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i == j){ // DIAGONAL VALUES = 1
                matrix[i][j] = 1.0;
            }else if(i > j){ // POSITIVE VALUE (BOTTOM) 
                matrix[i][j] = (double)50*(i+1)*(j+1)/((double)N*N*10000);;
            }else{ // NEGATIVE VALUE (TOP) 
                matrix[i][j] = (double)-50*(i+1)*(j+1)/((double)N*N*10000);
            }
        }
    }
}

/* This function does the calculations of each iteration */
void iterations(double **matrix, int m){
   
    int i, j, k, absHigher = 0; //  Declaration of auxiliary variables that are used to perform different operations
    double accumulate = 0; 
    double absCompare = 0; // Is the value of maximum representable finite floating-point (double) number
    double absReal;
    double *x1 = createArray(); // Create a array
    double *x2 = createArray(); // Create a new array
    AddValueArrayIdentity(x2); // New array = 1

    for(i = 0; i < N; i++){ // First iteration
        accumulate = 0;
        for(j = 0; j < N; j++){
            accumulate += matrix[i][j] * x2[i]; // The result vector is the matrix * the unit vector
        }
        x1[i] = accumulate;
    }
    //showArray(x1);
    for(k = 1; k < m; k ++){ // Rest of iterations
         
        for(i = 0; i < N; i ++){ // Access each element of the matrix to make the product for each element of the previous vector
            accumulate = 0;
            for(j = 0; j < N; j ++){
                accumulate += matrix[i][j]*x1[j]; // The result vector is the matrix * the vector
            }
            x2[i] = accumulate;
        }
        //showArray(x2);
        absCompare = 0;
        for(i = 0; i < N; i ++){ // Get the absolute value and compare it with the auxiliary variable that is updated
            if(absCompare < fabs(x2[i])){
                absCompare = fabs(x2[i]);
                absReal = x2[i];
                absHigher = i; // Tells us the position of the maximum value
            }
        }
        
        //printf("Data %i: %1.1f (%i) ABSREAL: %1.1f\n", k, absCompare, absHigher, absReal);  // Print the results
        
        if(absCompare > 25.0){ // Divide each position of the vector by the element with the largest absolute value.
            for(i = 0; i < N; i ++){
                x2[i] = x2[i] / absReal; // Fill the vector x1 with the results to use it in the next iteration           
            }
        }
       //showArray(x1);
        printf("Max in iteration %i: %1.1f (%i)\n", k, absReal, absHigher);  // Print the results
        
        for(i = 0; i < N; i ++){
            x1[i] = x2[i];
        }
       
    }

    free(x1); // Free memory array
    free(x2); // Free memory array

}

int main(int argc, char *argv[]){ // CONSOLE --> gcc iterativeSystem.c -o it && ./it matrix.bin 5
    
    argumentsOK(argc); // Funtion control arguments

    int m = atoi(argv[2]); // Fetch the value and cast it to int

    double **matrix = createMatrix(); // Create new matrix
    
    FILE *f = fopen(argv[1], "rb"); // open file by argument

    if(! f){ // if the file.bin no exists ...
        printf("Create new file...\n"); 
        valueMatrix(matrix); // Value random for a new matrix
        writeFile(matrix, argv[1]); // Write the matrix
    }
     
    readFile(matrix, argv[1]); // If file exists, read this file
 
    iterations(matrix, m); // Do operations
   
    free(matrix);  // Free memory matrix

    return 0;
}