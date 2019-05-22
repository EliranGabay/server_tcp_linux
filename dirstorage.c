#include "dirstorage.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h> 

void dictToDir(Dictionary *dict)
{
    Dictionary *temp = dict;
    FILE *outfile;
    char str[100];
    char *token, path[100];
    for (int i = 0; i < getSize(); i++)
    {
        strcpy(str, temp->head->key);
        token = strtok(str, ".");
        strcpy(path, token);
        while (token != NULL)
        {
            mkdir(path, 0777);
            strcat(path, "/");
            token = strtok(NULL, ".");
            if (token != NULL)
            {
                strcat(path, token);
                mkdir(path, 0777);
            }
        }
        strcat(path, "/");
        strcat(path, "_value.txt");
        outfile = fopen(path, "w");
        if (outfile == NULL)
        {
            fprintf(stderr, "\nError opend file\n");
            return;
        }
        fprintf(outfile, "%s", temp->head->value);
        fclose(outfile);
        temp = temp->tail;
    }
}
void DirToDic(Dictionary *dict,char* path){
    struct dirent *entry ;
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir(path);
    if(dr == NULL)
        return;
    entry=readdir(dr);
    if(entry == NULL)
        return;
    while(entry != NULL){
        if(entry->d_type == 4 && entry->d_name[0] != '.'){//4 is for dir type
            printf("%s\n",entry->d_name);
            DirToDic(dict,entry->d_name);
        }
        entry = readdir(dr);
    } 
    closedir(dr);

}
