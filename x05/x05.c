#include <stdio.h>
#include <stdlib.h>

void argumentsOK(int argc){
     if(argc != 3){
        printf("./x05 filename value\n");
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

int main(int argc, char *argv[]){ // CONSOLE --> gcc x05.c -o x05 && ./x05 file2.bin 200

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