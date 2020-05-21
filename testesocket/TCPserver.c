#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <string.h>
#include "calculator.h"

int main() {
    double final_result = 0;

    //Variables of transaction between client and server
    double db_client_message = 0;
    int int_client_message = 0;
    char char_server_message[300];

    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP an port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    // wait for client connection
    listen(server_socket, 5);

    // accept client connection
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    // (RECV 1) - receive number of slaves from client
    recv(client_socket, &int_client_message, sizeof(int_client_message), 0);

    // (SEND 1) - send a string confirmation to client
    sprintf(char_server_message, "(Server) Recebido, criando %d escravos...", int_client_message);
    send(client_socket, &char_server_message, sizeof(char_server_message), 0);
    
    //--------------------------
        int created_slaves = int_client_message; //Cria os escravos
    //---------------------------

    // (SEND 2) - confirma quantos escravos foram criados
    sprintf(char_server_message, "(Server) Foram criados %d escravos!", created_slaves);
    send(client_socket, &char_server_message, sizeof(char_server_message), 0);

    // (RECV 2) - recieve discretization interval
    recv(client_socket, &db_client_message, sizeof(db_client_message), 0);

    // proccess data from client------------------------
    final_result = resolveIntegral(db_client_message);

    //--------------------------------------------------
    // (SEND 3) - send the processed data to client
    send(client_socket, &final_result, sizeof(final_result), 0);

    // close the socket;
    close(server_socket);

    return 0;
}

    // int server_socket_array[int_client_message];
    // for (int i=0; i < int_client_message; i++){
    //     server_socket_array[i] = socket(AF_INET, SOCK_STREAM, 0);

    //     // define the server address
    //     struct sockaddr_in server_address;
    //     server_address.sin_family = AF_INET;
    //     server_address.sin_port = htons(9000);
    //     server_address.sin_addr.s_addr = INADDR_ANY;

    //     // bind the socket to our specified IP an port
    //     bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    //     // wait for client connection
    //     listen(server_socket, 5);
    // }