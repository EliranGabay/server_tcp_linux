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
//the thread function
void *connection_handler(void *);
int runServer(int, char *);

int main(int argc, char *argv[])
{
    char *path = ".";
    runServer(1, path);
}

int runServer(int op, char *path)
{
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    dict = dict_new();
    if (op == 1)
        FileToDict(dict);
    else
        DirToDic(dict, path);

    socket_desc = socket(AF_INET, SOCK_STREAM, 0); //Create socket
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
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
                char tempval[20];
                strcpy(tempval, value);
                strcat(tempval, "\n");
                if (value != NULL)
                {
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
                char tempval[20];
                strcpy(tempval, value);
                strcat(tempval, "\n");
                if (value != NULL)
                {
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
        dictToFile(dict);
        //dictToDir(dict);
        //printdict(dict);
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    return 0;
}
