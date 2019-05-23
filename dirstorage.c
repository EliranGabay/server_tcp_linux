#include "dirstorage.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h> 
#include <stdlib.h>

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

void DirToDic(Dictionary *dict,char *basePath)
{
    char path[1000],val[10];
    FILE *outfile;
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (dp->d_name[0] != '.' && ( dp->d_type == 4 || dp->d_name[0] == '_' ))
        {   
            printf("%s\n", dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if(dp->d_name[0] == '_'){
                outfile = fopen(path, "r");
                if (outfile == NULL) 
                    return; 
                fscanf(outfile,"%s\n",val);
            
                dict_add(dict,pathToKey(basePath),val);
                fclose (outfile);
             }
            DirToDic(dict,path);
        }
    }

    closedir(dir);
}

char* pathToKey(char *path){
    char *str,*tmp,*con;
    tmp=path;
    tmp = strtok(tmp,"/");
    con = strtok(NULL,tmp);
    str=(char *)malloc(sizeof(char)*strlen(con));
    strcpy(str,con);
    for (int i = 0; i < strlen(str); i++)
    { 
        if(str[i] == '/')  
            str[i]='.';
        
    }
    return str;
}