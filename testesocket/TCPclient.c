#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h> //Stores address information


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
    
    printf("Digite o número de escravos: ");
    scanf("%d", &message_info);
    
    // send number os slaves to server
    send(network_socket, &message_info, sizeof(message_info), 0);

    // receive server confirmation
    recv(network_socket, &message_info, sizeof(message_info), 0);
    
    // print out the server's response
    printf("Confirmação número de escravos: %d\n", message_info);
    
    // data which is gonna be sent to server
    printf("Digite o valor do intervalo: ");
    scanf("%lf", &message_data);

    // send data to server
    send(network_socket, &message_data, sizeof(message_data), 0);

    // recieve processed data from server
    recv(network_socket, &message_data, sizeof(message_data), 0);

    //print out the server's response
    printf("Resultado da integral: %f\n", message_data);

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