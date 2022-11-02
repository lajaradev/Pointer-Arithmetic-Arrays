#include <stdio.h>

int main(){ // CONSOLE --> gcc x01.c -o x01 && ./x01

    FILE *f, *f5, *f10; // f1 (reading), f5 (decimals), f10 (decimals)
    f = fopen ("doubles10data.bin", "rb");
    
    if (f == NULL){ // Check that file is not empty
        printf("Can't be opened doubles10data.bin");
        return -1;
    }

    int bytes_read;
    double num_doubles [10];
    
    bytes_read = fread(&num_doubles, sizeof(double), 10, f);
    
    f5 = fopen("exit_5_decimals.txt", "w");

    for(int i = 0; i < 10; i++){
        fprintf(f5, "%.5f\n", num_doubles[i]);
    }

    f10 = fopen("exit_10_decimals.txt", "w");

    for(int i = 0; i < 10; i++){
        fprintf(f10, "%.10f\n", num_doubles[i]);
    }

    fclose(f);
    return 0;
}