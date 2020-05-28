//Antigo cliente

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

int calculate_interval(int num_slave, int init, int final){
    int interval = final - init;
    return interval/num_slave;
}

int startCommunication(int *network_socket) {
    int slave_number = 0;       // number of slaves
    double k = 0;    // discretization interval number
    int status = 0;
    int count_try = 0;
    char char_buffer [300];
    double integral_result = 0;
    double final_result = 0;

    int init = 0;
    int final = 100;
    
    printf("(Master) Digite o número de escravos: ");
    scanf("%d", &slave_number);
    int interval = calculate_interval(slave_number, init, final);
    final = interval;

    // discretization interval which going to be sent to server.
    printf("(Master) Digite o valor do intervalo de discretização: ");
    scanf("%lf", &k);
    
    for (int i = 0; i < slave_number; i++) {
        // // (RECV 1) - receive server confirmation
        // recv(network_socket, &char_buffer, sizeof(char_buffer), 0);
        // // print out the server's response
        // printf("\n%s \n\n", char_buffer);

        // Try sending message 5 times, after that stops
        do {
            // (SEND 1) - send k
            status = send(network_socket[i], &k, sizeof(k), 0);
            // verifies if sent information correctly
            if(status == -1) {
                printf("(Master) Conexão %d falhou..\n", count_try);
            }
            count_try++;
        }  while(status == error && count_try <= 5);
        if(count_try == 5) {
            close(network_socket[i]);
            printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
            exit(0);
        }
        count_try = 0;

        // Try sending message 5 times, after that stops
        do {
            // (SEND 1) - send init
            status = send(network_socket[i], &init, sizeof(init), 0);
            // verifies if sent information correctly
            if(status == -1) {
                printf("(Master) Conexão %d falhou..\n", count_try);
            }
            count_try++;
        }  while(status == error && count_try <= 5);
        if(count_try == 5) {
            close(network_socket[i]);
            printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
            exit(0);
        }
        count_try = 0;

        // Try sending message 5 times, after that stops
        do {
            // (SEND 1) - send final
            status = send(network_socket[i], &final, sizeof(final), 0);
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
        count_try = 0;

        // (RECV 1) - recieve processed data from server (integral answer)
        recv(network_socket[i], &integral_result, sizeof(integral_result), 0);

        final_result += integral_result;
        printf("Resultado : %lf", final_result);

        init += interval;
        final = init + interval;
    }

    //print out final server's response
    printf("(Mestre) Resultado final da integral: %f\n", final_result);

    return 1;
}

int main() {
    
    // create a socket
    int network_socket[2];
    network_socket[0] = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9000); // htons converts 9000 port integer to an apropriate format
    server_address.sin_addr.s_addr = INADDR_ANY;// set up the server address 0.0.0.0 = INADDR_ANY
    //This is the basic information to connect remotely

    // 0 - OK 
    //-1 - SOME ERROR IN CONNECTION
    int connection_status1 = connect(network_socket[0], (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status1 == -1) {
        printf("Houve um erro ao fazer a conexão com um socket remoto.\n\n");
        close(network_socket[0]);
        exit(0);
    }

    network_socket[1] = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_port = htons(9001); // htons converts 9000 port integer to an apropriate format

    // 0 - OK 
    //-1 - SOME ERROR IN CONNECTION
    int connection_status2 = connect(network_socket[1], (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status2 == -1) {
        printf("Houve um erro ao fazer a conexão com um socket remoto.\n\n");
        close(network_socket[1]);
        exit(0);
    }

    startCommunication(network_socket);

    // and then close the socket
    close(network_socket[0]);
    close(network_socket[1]);

    return 0;
}