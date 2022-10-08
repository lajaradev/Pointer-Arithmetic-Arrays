#include <stdio.h>

int main(){

    FILE *f, *f5, *f10; // f1 (reading), f5 (decimals), f10 (decimals)
    f = fopen ("data10doubles.bin", "rb");
    
    if (f == NULL){ // Check that file is not empty
        printf("Can't be opened data10doubles.bin");
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