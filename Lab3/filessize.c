//
// Created by stani on 18.03.2024.
//
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
int main(){
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    struct stat file_info;
    long long size=0;
    while((entry= readdir(dir))){

        if(stat(entry->d_name,&file_info)!=-1&&!S_ISDIR(file_info.st_mode)){
            size+=file_info.st_size;
        }
    }
    printf("%lld\n",size);
    closedir(dir);
}