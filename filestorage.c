#include "filestorage.h"
#include <stdio.h>

void dictToFile(Dictionary *dict, char *path)
{
    FILE *outfile;
    Dictionary *temp = dict;
    char path1[50];
    strcpy(path1, path);
    strcat(path1, "/");
    strcat(path1, "storage.txt");
    // open file for writing
    outfile = fopen(path1, "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        return;
    }
    for (int i = 0; i < getSize(); i++)
    {
        fprintf(outfile, "%s %s\n", temp->head->key, temp->head->value);
        temp = temp->tail;
    }
    if (fwrite != 0)
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");
    // close file
    fclose(outfile);
}

void FileToDict(Dictionary *dict, char *path)
{
    FILE *outfile;
    char key[10], value[10], path1[50];
    // open file for writing
    strcpy(path1, path);
    strcat(path1, "/");
    strcat(path1, "storage.txt");
    outfile = fopen(path1, "r");
    if (outfile == NULL)
    {
        return;
    }
    //rewind(outfile);
    while (!feof(outfile))
    {
        fscanf(outfile, "%s %s\n", key, value);
        dict_add(dict, key, value);
    }
    // close file
    fclose(outfile);
}
