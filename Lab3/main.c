#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024
#define INPUT_FILE "example.txt"
#define OUTPUT_FILE "output.txt"
void reverse_file_by_bite(char *input_file_p,char * output_file_p);
void reverse_file_by_block(char *input_file_p,char * output_file_p);
int main(int argc,char **args){
    if(argc<3){
        printf("Wrong number of args\n");
        return 1;
    }
    FILE *file=fopen(args[1],"r");
    if(file==NULL){
        printf("file opening failed\n");
        return 1;
    }
    FILE *ft= fopen("pomiar_zad_2.txt","w+");
    fclose(file);
    clock_t start,end;
    start=clock();
    reverse_file_by_bite(args[1],args[2]);
    end=clock();
    fprintf(ft,"By byte: %f\n",(( double ) end - start)/CLOCKS_PER_SEC);
    printf("By Byte: %f\n",(( double ) end - start)/CLOCKS_PER_SEC);
    start=clock();
    reverse_file_by_block(args[1],args[2]);
    end=clock();
    printf("By Block: %f\n",(( double ) end - start)/CLOCKS_PER_SEC);
    fprintf(ft,"By Block: %f\n",(( double ) end - start)/CLOCKS_PER_SEC);
    fclose(ft);
    return 0;
}
