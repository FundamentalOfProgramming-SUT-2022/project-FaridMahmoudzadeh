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

void start();

void BackslashFix(char str[]);

void getstring(char c, char str[]);

void createfile(char path[]);

void insert(char path[],char str[],int line,int point);

void cat(char path[]);

void removestr(char path[], int line, int point, int size, char type);



int main() {

    start();

}

void start(){

    char command[100];

    char temp[50];

    while(1){

        scanf("%s",command);

        if(strcmp(command,"exit") == 0) return;

        else if(strcmp(command,"createfile") == 0) {
            getchar();
            scanf("%s",temp);
            getchar();
            char path[100];
            gets(path);
            createfile(path);
        }

        else if(strcmp(command,"insertstr") == 0) {
            getchar();
            scanf("%s",temp);
            getchar();
            char path[100];
            char c = getchar();
            if(c == '"') {
                getchar();
                scanf("%[^\"]s",path);
                getchar();
            }
            else if(c == '/') scanf("%s",path);
            getchar();
            scanf("%s",temp);
            getchar();
            char str[100] = {};
            c = getchar();
            getstring(c, str);
            scanf("%s",temp);
            int line, point;
            scanf(" %d:%d",&line ,&point);
            insert(path,str,line,point);
        }

        else if(strcmp(command,"cat") == 0) {
            getchar();
            scanf("%s",temp);
            getchar();
            char path[100];
            gets(path);
            cat(path);
        }

        else if(strcmp(command,"removestr")) {
            getchar();
            scanf("%s",temp);
            getchar();
            char c = getchar();
            char path[100];
            if(c == '"') {
                getchar();
                scanf("%[^\"]s",path);
                getchar();
            }
            else if(c == '/') scanf("%s",path);
            getchar();
            scanf("%s",temp);
            int line, point;
            scanf(" %d:%d ", &line, &point);
            scanf("%s",temp);
            int size; 
            char type;
            scanf(" %d -%c", &size, &type);
            removestr(path, line, point, size, type);
        }
    }
}

void BackslashFix(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == '\\'){
            if(str[i + 1] == 'n'){
                str[i] = '\n';
                memmove( &str[i + 1], &str[i + 2], strlen(str) - i - 1);
            }
            else if(str[i + 1] == '\\'){
                str[i] = '\\';
                memmove( &str[i + 1], &str[i + 2], strlen(str) - i - 1);
            }
            else if(str[i + 1] == '\n'){
                str[i] = '"';
                memmove( &str[i + 1], &str[i + 2], strlen(str) - i - 1);
            }
        }
    }
}

void getstring(char c, char str[]){
    if(c == '"'){
        int d;
        int i = 0;
        c = getchar();
        while (c != '"')
        {
            d = c;
            str[i] = c;
            str[i + 1] = '\0';
            i++;
            c = getchar();
            if(c == '"' && d == '\\'){
                c = '\n';
            }
        }
        BackslashFix(str);
    }
    else{
        char str2[100];
        scanf("%s",str2);
        str[0] = c;
        strcat(str, str2);
    }
    getchar();
}

void createfile(char path[]){
    int p = strlen(path);
    FILE *file;
    char dir[100];
    int l;
    if(path[0] == '/') l = 1;
    else if(path[0] == '"') l = 2;
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
        printf("File With This Name Already Exists!\n");
        return;
    }
}

void insert(char path[],char str[],int line,int point){
    if(fopen(path,"r") == 0){
        printf("File With This Name Doesn't Exist!\n");
        return;
    }
    FILE *file;
    file = fopen(path,"r");
    int i = 1, j = 0;
    char c;
    char beforestr[10000] = {};
    int x = 0;
    while (i != line || j != point)
    {
        c = fgetc(file);
        if(c == EOF){
            printf("This Position Doesn't Exist!\n");
            fclose(file);
            return;
        }
        j++;
        beforestr[x] = c;
        beforestr[x + 1] = '\0';
        x++;
        if(c == '\n'){
            i++;
            j = 0;
        }
    }
    x = 0;
    char afterstr[10000];
    while(1){
        c = fgetc(file);
        if(c == EOF){
            break;
        }
        afterstr[x] = c;
        afterstr[x + 1] = '\0';
        x++;
    }
    fclose(file);
    file = fopen(path,"w");
    fprintf(file,"%s%s%s",beforestr,str,afterstr);
    fclose(file);
}

void cat(char path[]){
    if(path[0] == '"'){
        path[strlen(path) - 1] = '\0';
        memmove(&path[0], &path[2], strlen(path) - 1);
    }
    else if(path[0] == '/'){
        memmove(&path[0], &path[1], strlen(path));
    }
    if(fopen(path,"r") == 0){
        printf("File With This Name Doesn't Exist!\n");
        return;
    }
    FILE *file;
    file = fopen(path,"r");
    char c;
    int i = 0;
    char contents[10000];
    while(1){
        c = fgetc(file);
        if(c == EOF) break;
        contents[i] = c;
        i++;
        contents[i] = '\0';
    }
    fclose(file);
    printf("%s\n",contents);
}

void removestr(char path[], int line, int point, int size, char type){
    if(fopen(path,"r") == 0){
        printf("File With This Name Doesn't Exist!\n");
        return;
    }
    FILE *file;
    file = fopen(path,"r");
    if(type == 'f'){
        int i = 1, j = 0;
        char c;
        char rest[10000] = {};
        int x = 0;
        while (i != line || j != point)
        {
            c = fgetc(file);
            if(c == EOF){
                printf("This Position Doesn't Exist!\n");
                fclose(file);
                return;
            }
            j++;
            rest[x] = c;
            rest[x + 1] = '\0';
            x++;
            if(c == '\n'){
                i++;
                j = 0;
            }
        }
        for(int counter = 0; counter < size; counter++){
            c = fgetc(file);
            if(c == EOF){
                printf("Not Enough Characters to Remove\n");
                fclose(file);
                return;
            }
        }
        while(1){
            c = fgetc(file);
            if(c == EOF){
                break;
            }
            rest[x] = c;
            rest[x + 1] = '\0';
            x++;
        }
        fclose(file);
        file = fopen(path,"w");
        fprintf(file,"%s",rest);
        fclose(file);
    }
    else if(type == 'b'){
        int i = 1, j = 0;
        char c;
        char rest[10000] = {};
        int x = 0;
        while (i != line || j != point)
        {
            c = fgetc(file);
            if(c == EOF){
                printf("This Position Doesn't Exist!\n");
                fclose(file);
                return;
            }
            j++;
            rest[x] = c;
            rest[x + 1] = '\0';
            x++;
            if(c == '\n'){
                i++;
                j = 0;
            }
        }
        rest[strlen(rest) - size] = '\0';
        x -= size;
        while(1){
            c = fgetc(file);
            if(c == EOF){
                break;
            }
            rest[x] = c;
            rest[x + 1] = '\0';
            x++;
        }
        fclose(file);
        file = fopen(path,"w");
        fprintf(file,"%s",rest);
        fclose(file);        
    }
    else {
        printf("Invalid Type\n");
        fclose(file);
    }
}
