#include <stdio.h>
#include <stdlib.h>

void argumentsOK(int argc){
     if(argc != 2){
        printf("./p0t5, filename\n");
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
    int arraySizeUnsigShortInt = arraySize / 2;
    int arraySizeUnsigInt = arraySize / 4;
    int counter1, counter2;
    unsigned char *array = createArray(&arraySize);
    
    readFile(array, argv[1], &arraySize);
    
    unsigned char *ptrUC;
    unsigned short int *ptrUSI;
    unsigned int * ptrUI;

    ptrUC = (unsigned char*)&array[0];
    ptrUSI = (unsigned short int*)&array[0];
    ptrUI = (unsigned int*)&array[0];


    /* UNSIGNED CHAR 1 BYTE*/
    counter1 = 0;
    counter2 = 0;
    printf("UNSIGNED CHAR\n");
    for(int i = 0; i < arraySize; i++){
        counter2 ++;
        if( (int)*ptrUC %2 == 0){           
            counter1 ++;
        }
        *ptrUC ++;
    }
    for(int j = 0; j < 2; j ++){
        printf("MemoryAddres: %p\n", ptrUC );
        *ptrUC ++;
    }
    printf("TotalArray: %i\n", counter2);
    printf("Total Even Numbers: %d\n", counter1);
    
    /* UNSIGNED SHORT INT 2 BYTES*/
    counter1 = 0;
    counter2 = 0;
    printf("\nUNSIGNED SHORT INT\n");
    for(int i = 0; i < arraySizeUnsigShortInt; i++){
        counter2 ++;
        if( (int)*ptrUSI %2 == 0){
            counter1 ++;
        }
        *ptrUSI ++;
    }
    for(int j = 0; j < 2; j ++){
        printf("MemoryAddres: %p\n", ptrUSI );
        *ptrUSI ++;
    }
    printf("TotalArray: %i\n", counter2);
    printf("Total Even Numbers: %d\n", counter1);

    /* UNSIGNED INT 4 BYTES*/
    counter1 = 0;
    counter2 = 0;
    printf("\nUNSIGNED INT\n");
    for(int i = 0; i < arraySizeUnsigInt; i++){
         counter2 ++;
        if( (int)*ptrUI %2 == 0){
            counter1 ++;
        }
        *ptrUI ++;
    }
    for(int j = 0; j < 2; j ++){
        printf("MemoryAddres: %p\n", ptrUI );
        *ptrUI ++;
    }
    printf("TotalArray: %i\n", counter2);
    printf("Total Even Numbers: %d\n", counter1);


    free(array);
    return 0;
}