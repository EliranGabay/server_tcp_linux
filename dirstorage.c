#include "dirstorage.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

void dictToDir(Dictionary *dict){
    Dictionary *temp=dict;
    for (int i = 0; i < getSize(); i++)
    {   
        mkdir(temp->head->key,0700);
        temp=temp->tail;
    }
    
}