#include <stdio.h>
#include <stdlib.h>
#define BLOCK_SIZE 1024
void reverse_file_by_bite(char *input_file_p,char * output_file_p){
    FILE *input_file=fopen(input_file_p,"r");
    FILE *output_file= fopen(output_file_p,"w+");
    char bufor;
    int i=-1;
    while(fseek(input_file,i--,SEEK_END)==0) {
        fread(&bufor, sizeof(char), 1, input_file);
        fwrite(&bufor, sizeof(char),1,output_file);
    }
    fclose(input_file);
    fclose(output_file);

}
void reverse_file_by_block(char *input_file_p,char * output_file_p){
    FILE *input_file=fopen(input_file_p,"r");
    FILE *output_file= fopen(output_file_p,"w+");
    char bufor[BLOCK_SIZE];
    int i=-1;
    ssize_t res;

    while (fseek(input_file, i-- * BLOCK_SIZE, SEEK_END) == 0 || (res = fread(bufor, sizeof(char), BLOCK_SIZE, input_file)) != 0) {
        char *dataw = malloc(sizeof(char) * res);
        for (int j = 0; j < res; j++) {
            dataw[j] = bufor[res - 1 - j];
        }
        fwrite(dataw, sizeof(char), res, output_file);
        free(dataw);
    }
}
//int main(int argc,char **args){
//    if(argc<3){
//        printf("Wrong number of args\n");
//        return 1;
//    }
//    FILE *file=fopen(args[1],"r");
//    if(file==NULL){
//        printf("file opening failed\n");
//        return 1;
//    }
//    fclose(file);
//    //reverse_file_by_bite(args[1],args[2]);
//    reverse_file_by_block(args[1],args[2]);
//
//}