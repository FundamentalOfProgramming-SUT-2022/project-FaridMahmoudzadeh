#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <unistd.h>

void createfile(char path[]){
    int p = strlen(path);
    FILE *file;
    char dir[100];
    int l;
    if(path[0]=='/') l = 1;
    else if(path[0]=='"') l = 2;
    for(int i = l; i < p; i++){
        if(path[i] != '"'){
            dir[i - l] = path[i];
            dir[i - l + 1] = '\0';
        }
        if(path[i + 1] == '/'){
            mkdir(dir);
        }
    }
    if(fopen(dir,"r") == 0){
        file = fopen(dir,"w");
        fclose(file);
    }
    else{
        printf("File With This Name Already Exists\n");
    }
}

int main() {

    char command[100];

    while(1){

        scanf("%s",command);

        if(strcmp(command,"exit") == 0) return 0;

        else if(strcmp(command,"createfile") == 0) {
            getchar();
            char s[5];
            scanf("%s",s);
            getchar();
            char path[100];
            gets(path);
            createfile(path);
        }
    }
}