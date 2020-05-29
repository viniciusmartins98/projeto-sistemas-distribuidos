//Antigo cliente

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> // close function

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

// calculates size of interval which is going to be divided for servers
int calculate_interval(int num_slave, int init, int final){
    int interval = final - init;
    return interval/num_slave;
}

// Try to sent message 5 times, after that stops
int trySentDoubleMessage(int network_socket, double k) {
    int status=0;
    int count_try=0;

    do {
        // send k (discretization interval)

        status = send(network_socket, &k, sizeof(k), 0);
        // verifies if sent information correctly
        if(status == -1) {
            printf("(Master) Conexão %d falhou..\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 5) {
        close(network_socket);
        printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
        exit(0);
    }
}
int trySentIntMessage(int network_socket, int k) {
    int status=0;
    int count_try=0;

    do {
        // send k (discretization interval)
        status = send(network_socket, &k, sizeof(k), 0);
        // verifies if sent information correctly
        if(status == -1) {
            printf("(Master) Conexão %d falhou..\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 5) {
        close(network_socket);
        printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
        exit(0);
    }
}

int startCommunication(int *network_socket, int slave_number) {
    // params for the Distributed System
    double k = 0; // discretization interval number
    int init=0; // init and final intervals of wich server is going to work        
    int final=100; 

    // vars which is gonna store the partial integral result and final_result respectively
    double integral_result = 0; // result provided by a slave adding in a sum
    double final_result = 0; // final result = sum of all slaves result

    int status = 0; // stores status of connection
    int count_try = 0; // count number of attempts to slave
    
    int interval = calculate_interval(slave_number, init, final);
    final = interval;

    // discretization interval which going to be sent to server.
    printf("(Master) Digite o valor do intervalo de discretização: ");
    scanf("%lf", &k);
    
    for (int i = 0; i < slave_number; i++) {
        // send discretization interval for server
        trySentDoubleMessage(network_socket[i], k);

        // send init interval for server
        trySentIntMessage(network_socket[i], init);

        // send final interval for server
        trySentIntMessage(network_socket[i], final);

        // (RECV 1) - recieve processed data from server (integral answer)
        recv(network_socket[i], &integral_result, sizeof(integral_result), 0);

        final_result = final_result + integral_result;
        init = init + interval;
        final = init + interval;
    }

    //print out final server's response
    printf("(Mestre) Resultado final da integral: %f\n", final_result);

    return 1;
}

int setUpConnection(int network_socket, struct sockaddr_in server_address, int port_number) {
    //  0 => OK 
    // -1 => SOME ERROR IN CONNECTION
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_port = htons(port_number); // htons converts 9000 port integer to an apropriate format

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status == -1) {
        printf("Houve um erro ao fazer a conexão com um socket remoto.\n\n");
        close(network_socket);
        exit(0);
    }


    return network_socket;
}

int main() {
    int slave_number = 0;


    // specify address configurations for  all sockets, port number is going to be set later (one port by server)
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // set up the server address 0.0.0.0 = INADDR_ANY

    printf("(Master) Digite o número de escravos: ");
    scanf("%d", &slave_number);

    printf("%d", slave_number);
    // create an array of sockets with size of number of slaves
    int * network_socket = malloc(slave_number*sizeof(int));

    // set up connections
    int port_number = 9000;
    for(int i=0; i<slave_number; i++) {
        network_socket[i] = setUpConnection(network_socket[i], server_address, port_number);
        port_number++;
    }

    startCommunication(network_socket, slave_number);

    // and then close the socket
    for(int i=0; i<slave_number; i++)
        close(network_socket[i]);

    return 0;
}