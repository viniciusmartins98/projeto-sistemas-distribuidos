// slave server 1

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // close function

#include <netinet/in.h>

#include <string.h>
#include "lib/partial_calculator.h"

void setUpAndBindServerAddress(struct sockaddr_in server_address, int server_socket) {
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9006);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP an port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
}

int listenMaster(int server_socket) {
    int client_socket;

    // wait for client connection
    listen(server_socket, 5);

    // accept client connection and stores the client_socket information
    client_socket = accept(server_socket, NULL, NULL);

    return client_socket;
}

void startCommunication(int client_socket) {
    double k, final_result;
    int init, final;

    // recebe o intervalo de discretização (k)
    recv(client_socket, &k, sizeof(k), 0);
    if(k == -100.0) {
        return;
    }

    // recebe valor inicial do intervalo (init)
    recv(client_socket, &init, sizeof(init), 0);
    printf("(Slave 7) x inicial: %d\n", init);

    // recebe o valor final do intervalo (final)
    recv(client_socket, &final, sizeof(final), 0);
    printf("(Slave 7) x final: %d\n", final);

    // proccess data from client
    final_result = resolveIntegral(k, init, final);
    printf("(Slave 7) resultado parcial: %lf\n\n", final_result);

    // (SEND 1) - Envia o resultado parcial da integral
    send(client_socket, &final_result, sizeof(final_result), 0);
}

int main() {
    double final_result = 0;

    // vars of transaction between client and server
    double k = 0;
    int init = 0;
    int final = 0;

    // vars of server socket and server address
    int server_socket; // create the server socket
    struct sockaddr_in server_address; // defines slave address
    int client_socket; // client socket which is going to be receive the answer

    // set up the server address
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    setUpAndBindServerAddress(server_address, server_socket);

    client_socket = listenMaster(server_socket);

    startCommunication(client_socket);

    // close the socket;
    close(server_socket);

    return 0;
}