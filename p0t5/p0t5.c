/*
Read from a binary file a total of 256 Kb in contiguous memory. Pointer Simple unsigned char
Calculate the number of data that are less than or equal to a value (argv[])
Elements can NOT be accessed using the array index.

CONSOLE --> gcc p0t5.c -o p0t5 && ./p0t5 file_entry value
*/

#include <stdio.h>
#include <stdlib.h>

void argumentsOK(int argc){
     if(argc != 3){
        printf("./p0t5 filename value\n");
        exit(-1);
    }
}

unsigned char* createArray(int *SIZE){
    unsigned char *array;
    array = (unsigned char*) malloc(*SIZE * sizeof(unsigned char*));
    return array;
}

void readFile(unsigned char *array, char *fily_entry, int *SIZE){
    FILE *f = fopen(fily_entry, "rb");
    if(f){
        int bytes = fread (&array[0], sizeof(unsigned char), *SIZE, f);
        fclose(f);
    }
}

int main(int argc, char *argv[]){

    argumentsOK(argc);
    
    int kB = 1024;
    int data = 256;
    int arraySize = data * kB;
    int counter = 0;

    unsigned char *array = createArray(&arraySize);
    
    readFile(array, argv[1], &arraySize);
    
    unsigned char *ptr;
    ptr = &array[0];

    for(int i = 0; i < arraySize; i++){
        if( (int)*ptr <= atoi(argv[2]) ){
            counter ++;
        }
        *ptr ++;
    }
    printf("Total: %d\n", counter);
    
    free(array);
    return 0;
}