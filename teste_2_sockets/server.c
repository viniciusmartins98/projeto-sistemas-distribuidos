#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <string.h>

void startCommunication(int * server_socket) {
    for (int i = 0; i < 2; i++) {
      printf("Iteração %d\n", i);
        // accept client connection
        int client_socket = accept(server_socket[i], NULL, NULL);

        char client_message[300];
        char server_message[300];
        
        if(i == 0) {
            strcpy(server_message, "Hello ");
        } 
        if (i == 1) {
            strcpy(server_message, "World! ");
            printf("Teste");
        }
        
        send(client_socket, &server_message, sizeof(server_message), 0);
        printf("depois do send!");

        close(server_socket[i]);
    }
}

int main()
{
    int server_socket[2];

    for (int i = 0; i < 2; i++)
    {
        server_socket[i] = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(9000);
        server_address.sin_addr.s_addr = INADDR_ANY;

        // bind the socket to our specified IP an port
        bind(server_socket[i], (struct sockaddr *)&server_address, sizeof(server_address));

        // wait for client connection
        listen(server_socket[i], 5);

    }
    startCommunication(server_socket);

    return 0;
}