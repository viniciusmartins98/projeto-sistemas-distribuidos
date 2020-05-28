//Antigo servidor

//calcularIntegral(k, init, init + result);

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <string.h>
#include "partial_calculator.h"

int main() {
    double final_result = 0;

    //Variables of transaction between client and server
    double k = 0;
    int init = 0;
    int final = 0;

    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the slave address
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

    // (RECV 1) - recebe o intervalo de discretização (k)
    recv(client_socket, &k, sizeof(k), 0);
    printf("K : %lf\n", k);

    // (RECV 2) - recebe valor inicial do intervalo (init)
    recv(client_socket, &init, sizeof(init), 0);
    printf("Inicial : %d\n", init);

    // (RECV 3) - recebe o valor final do intervalo (final)
    recv(client_socket, &final, sizeof(final), 0);
    printf("Final : %d\n", final);

    // proccess data from client------------------------
    final_result = resolveIntegral(k, init, final);
    printf("Resultado final : %lf\n", final_result);

    // (SEND 1) - Envia o resultado parcial da integral
    send(client_socket, &final_result, sizeof(final_result), 0);

    // close the socket;
    close(server_socket);

    return 0;
}