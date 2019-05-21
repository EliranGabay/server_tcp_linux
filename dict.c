#include "dict.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int size = 0;

Dictionary *dict_new()
{
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    assert(dictionary != NULL);
    dictionary->head = NULL;
    dictionary->tail = NULL;
    return dictionary;
}

void dict_add(Dictionary *dictionary, const char *key, char *value)
{
    if (dict_has(dictionary, key))
        dict_remove(dictionary, key);
    if (dictionary->head != NULL)
    {
        while (dictionary->tail != NULL)
        {
            dictionary = dictionary->tail;
        }
        Dictionary *next = dict_new();
        dictionary->tail = next;
        dictionary = dictionary->tail;
    }
    int key_length = strlen(key) + 1;
    int value_length = strlen(value) + 1;
    dictionary->head = (KVPair *)malloc(sizeof(KVPair));
    assert(dictionary->head != NULL);
    dictionary->head->key = (char *)malloc(key_length * sizeof(char));
    dictionary->head->value = (char *)malloc(value_length * sizeof(char));
    assert(dictionary->head->key != NULL);
    strcpy(dictionary->head->key, key);
    assert(dictionary->head->value != NULL);
    strcpy(dictionary->head->value, value);
    size++;
}

int dict_has(Dictionary *dictionary, const char *key)
{
    if (dictionary->head == NULL)
        return 0;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0)
            return 1;
        dictionary = dictionary->tail;
    }
    return 0;
}

char *dict_get(Dictionary *dictionary, const char *key)
{
    Dictionary *temp = dictionary;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(temp->head->key, key) == 0)
        {
            return temp->head->value;
        }
        temp = temp->tail;
    }
    return NULL;
}

void dict_remove(Dictionary *dictionary, const char *key)
{
    if (dictionary->head == NULL)
        return;
    Dictionary *previous = NULL;
    while (dictionary != NULL)
    {
        if (strcmp(dictionary->head->key, key) == 0)
        {
            if (previous == NULL)
            {
                free(dictionary->head->key);
                dictionary->head->key = NULL;
                if (dictionary->tail != NULL)
                {
                    Dictionary *toremove = dictionary->tail;
                    dictionary->head->key = toremove->head->key;
                    dictionary->tail = toremove->tail;
                    free(toremove->head);
                    free(toremove);
                    return;
                }
            }
            else
            {
                previous->tail = dictionary->tail;
            }
            free(dictionary->head->key);
            free(dictionary->head);
            free(dictionary);
            return;
        }
        previous = dictionary;
        dictionary = dictionary->tail;
    }
}

void printdict(Dictionary *dict)
{
    Dictionary *temp = dict;
    for (int i = 0; i < size; i++)
    {
        printf("key: %s ,value: %s\n", temp->head->key, temp->head->value);
        temp = temp->tail;
    }
    printf("size is : %d\n", size);
}

int checkrisha(char *key, Dictionary *dict)
{
    int result = 0, flag = 0;
    Dictionary *temp = dict;
    char *tempkey, *tempvalue;
    for (int i = 0; i < size; i++)
    {
        tempkey = temp->head->key;
        tempvalue = temp->head->value;
        flag = 0;
        for (int j = 0; j < strlen(key); j++)
        {
            if (key[j] != tempkey[j] && flag == 0)
            {
                flag = 1;
            }
        }
        if (flag == 0)
        {
            result += atoi(tempvalue);
        }
        temp = temp->tail;
    }
    return result;
}

void dict_free(Dictionary *dictionary)
{
    if (dictionary == NULL)
        return;
    free(dictionary->head->key);
    free(dictionary->head);
    Dictionary *tail = dictionary->tail;
    free(dictionary);
    dict_free(tail);
}
