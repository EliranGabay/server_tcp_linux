#include "dirstorage.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

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