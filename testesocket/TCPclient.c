#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h> //Stores address information

#define error -1


    //Cast server addres to different structure

/*------- SOCKET PARAMETERS-------------------------+
|AF_INET: Define that's gonna be on internet domain|
|SOCK_STREAM: Use that with TCP                    |
|0 - TCP                                           |
---------------------------------------------------+*/

/*--------CONNECTION PARAMETERS--------------------+
|socket: It's the socket just created              |
|server_address: Casted server_address             |
|size of the address: Size of server_address       |
|return: integer                                   |
---------------------------------------------------+*/
int startCommunication(int network_socket) {
    int message_info = 0;       // number of slaves
    double message_data = 0;    // discretization interval number
    int status = 0;
    int count_try = 0;

    char char_buffer [300];
    
    printf("(Client) Digite o número de escravos: ");
    scanf("%d", &message_info);
    
    // Try sending message 5 times, after that stops
    do {
        // (SEND 1) - send number os slaves to server
        status = send(network_socket, &message_info, sizeof(message_info), 0);
        // verifies if sent information correctly
        if(status == -1) {
            printf("(Client) Attempt %d failed...\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 5) {
        close(network_socket);
        printf("(Client) Não foi possível enviar a mensagem ao servidor\n");
        exit(0);
    }
    count_try = 0;

    // (RECV 1) - receive server confirmation
    recv(network_socket, &char_buffer, sizeof(char_buffer), 0);
    // print out the server's response
    printf("\n%s \n\n", char_buffer);
    
    // discretization interval which going to be sent to server.
    printf("(Client) Digite o valor do intervalo: ");
    scanf("%lf", &message_data);

    // Try sending message 5 times, after that stops
    do {
        // // (SEND 2) - send discretization interval to server
        send(network_socket, &message_data, sizeof(message_data), 0);
        // verifies if it sends information correctly
        if(status == -1) {
            printf("(Client) Attempt %d failed...\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 5) {
        close(network_socket);
        printf("(Client) Não foi possível enviar a mensagem ao servidor\n");
        exit(0);
    }
    count_try = 0;

    // (RECV 2) - confirm how many slaves was created
    recv(network_socket, &char_buffer, sizeof(char_buffer), 0);
    printf("\n%s \n\n", char_buffer);

    // (RECV 3) - recieve processed data from server (integral answer)
    recv(network_socket, &message_data, sizeof(message_data), 0);

    //print out final server's response
    printf("(Client) Resultado da integral: %f\n", message_data);

    return 1;
}

int main() {
    
    // create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000); // htons converts 9000 port integer to an apropriate format
    server_address.sin_addr.s_addr = INADDR_ANY;// set up the server address 0.0.0.0 = INADDR_ANY
    //This is the basic information to connect remotely

    // 0 - OK 
    //-1 - SOME ERROR IN CONNECTION
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status == -1) {
        printf("There was an error making a connection to the remote socket \n\n");
        close(network_socket);
        exit(0);
    }

    startCommunication(network_socket);

    // and then close the socket
    close(network_socket);

    return 0;
}