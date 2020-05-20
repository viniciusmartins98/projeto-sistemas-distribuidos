#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int main() {

    double server_message = 0;

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

    // recieve data from client
    recv(client_socket, &server_message, sizeof(server_message), 0);

    // processe data from client
    server_message = server_message * 10000;

    // send the processed data to client
    send(client_socket, &server_message, sizeof(server_message), 0);

    printf("Client data is: %f \n", server_message);

    // close the socket;
    close(server_socket);

    return 0;
}