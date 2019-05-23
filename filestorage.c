#include "filestorage.h"
#include <stdio.h>

void dictToFile(Dictionary *dict)
{
    FILE *outfile;
    Dictionary *temp = dict;
    // open file for writing
    outfile = fopen("storage.txt", "w");
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

void FileToDict(Dictionary *dict)
{
    FILE *outfile;
    char key[10], value[10];
    // open file for writing
    outfile = fopen("storage.txt", "r");
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
