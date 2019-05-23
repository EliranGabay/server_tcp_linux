/*
    C socket server example, handles multiple clients using threads
    Compile
    gcc server.c -lpthread -o server

*/
//#include "dict.h"
#include "dirstorage.h"
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <string.h>    //strchr;
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread

Dictionary *dict;
int port;
char path[50];
char flag[3];
//the thread function
void *connection_handler(void *);
int runServer();
int main(int argc, char *argv[])
{
    
    strcpy(path, "."); //defult save dir
    port = 8888;       //defult port
    dict = dict_new();
    if (argc < 2) //file text seve in defult dir(Defult op)
    {
        FileToDict(dict,path);
        printf("Defult Run: storage: file.txt directory: '%s' port: '%d'\n", path, port);
        printf("-----------------------------------------------\n");
        runServer();
        dictToFile(dict,path);
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(argv[i], "-t") == 0) //type save
            {
                if ((strcmp(argv[i + 1], "file") == 0))
                    strcpy(flag, "t1");
                else if (strcmp(argv[i + 1], "dir") == 0)
                    strcpy(flag, "t2");
                else
                {
                    printf("Erorr: -t get only file/dir\n");
                    return 0;
                }
                
                i++;
            }
            else if (strcmp(argv[i], "-s") == 0) //path
            {
                if (strstr(argv[i+1],"/"))
                {
                    strcpy(path, argv[i + 1]);
                    i++;
                }
                else
                {
                    printf("Erorr: -s get only string at start '/'\n");
                    return 0;
                }
            }
            else if (strcmp(argv[i], "-p") == 0) //port connect
            {
                if (strcmp(argv[i + 1], "") != 0 && atoi(argv[i + 1]) > 10)
                {
                    port = atoi(argv[i + 1]);
                    i++;
                }
                else
                {
                    printf("Erorr: -p not get int value\n");
                    return 0;
                }
            }
            else if (strcmp(argv[i], "--help") == 0) //print help
            {
                printf("--Help\n");
            }
        }
        if (strcmp(flag,"t1")==0 || !strstr(flag, "t"))
        {
            FileToDict(dict,path);
            printf("Run: storage: file.txt | directory: '%s' | port: '%d'\n", path, port);
            printf("-----------------------------------------------\n");
            runServer();
            dictToFile(dict,path);
        }
        else
        {
            DirToDic(dict, path);
            printf("Run: storage: directory | directory: '%s' | port: '%d'\n", path, port);
            printf("-----------------------------------------------\n");
            runServer();
            dictToDir(dict);
        }
    }
}

int runServer()
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0); //Create socket
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    //server.sin_port = htons(9999);
    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc, 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection accepted");

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    int b, e;
    const char ch = ' ';
    char *message, client_message[2000], *ptr, *match, *key, *value, str[100];
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Send me (./+/*/-)path id.\n";
    write(sock, message, strlen(message));

    //Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        key = NULL;
        value = NULL;
        ptr = NULL;
        //end of string marker
        //client_message[read_size] = '\0';
        ptr = strpbrk(client_message, ".+*-");
        if (ptr != NULL && strlen(strchr(client_message, *ptr)) > 2)
        {
            //write(sock, "Ack\n", strlen("Ack\n"));
            if (strchr(client_message, ch) && client_message[0] == '.')
            { //message type .path id
                client_message[0] = '!';
                key = strtok(client_message, "!");
                key = strtok(key, " ");
                value = strtok(NULL, " ");
                value[strlen(value) - 1] = '\0';
                //write(sock, key, strlen(key));
                //write(sock, value, strlen(value));
                dict_add(dict, key, value);
            }
            //message type (./+/-/*)path //section 2,3,4,5
            else if (client_message[0] == '.')
            { //.
                client_message[0] = '!';
                key = strtok(client_message, "!");
                key[strlen(key) - 1] = '\0';
                value = dict_get(dict, key);
                if (value != NULL)
                {
                    char tempval[20];
                    strcpy(tempval, value);
                    strcat(tempval, "\n");
                    write(sock, tempval, strlen(tempval));
                }
            }
            else if (client_message[0] == '+')
            { //+
                client_message[0] = '!';
                key = strtok(client_message, "!");
                key[strlen(key) - 1] = '\0';
                value = dict_get(dict, key);
                if (value != NULL)
                {
                    if (atoi(value) > 0)
                    {
                        sprintf(value, "%d", (atoi(value) + 1));
                    }
                }
            }
            else if (client_message[0] == '*')
            {
                client_message[0] = '!';
                key = strtok(client_message, "!");
                key[strlen(key) - 1] = '\0';
                int t = checkrisha(key, dict);
                char tempval[20];
                sprintf(tempval, "%d\n", t);
                write(sock, tempval, strlen(tempval));
            }
            else if (client_message[0] == '-')
            {
                client_message[0] = '!';
                key = strtok(client_message, "!");
                key[strlen(key) - 1] = '\0';
                value = dict_get(dict, key);
                value[strlen(value)] = '\0';

                strcpy(str, key);
                strcat(str, ".");
                strcat(str, value);
                //str[strlen(str) - 1] = '\0';
                value = dict_get(dict, str);
                if (value != NULL)
                {
                    char tempval[20];
                    strcpy(tempval, value);
                    strcat(tempval, "\n");
                    write(sock, tempval, strlen(tempval));
                }
            }
            else
                write(sock, "BAD INPUT!\n", 12);
        }
        else
            write(sock, "BAD INPUT!\n", 12);
        //clear the message buffer
        memset(client_message, 0, 2000);
    }
    if (read_size == 0)
    {
        puts("Client disconnected");

        if (strcmp(flag,"t1")==0 || !strstr(flag, "t"))
        {
            dictToFile(dict,path);
        }
        else
            dictToDir(dict);
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    return 0;
}
