//Antigo cliente

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> // close function

#include <netinet/in.h> //Stores address information

#include <string.h>
#include<errno.h>
extern int errno;

#include <time.h> // Calculat spend time

#define error -1

// verifies if is executing on windows or linux OS.
#ifdef WIN32

#include <windows.h>

// function to calculate time on windows
double get_time() {
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}

#else

#include <sys/time.h>
#include <sys/resource.h>

// function to calculate time on linux
double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}

#endif


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

// Try to sent a double message 5 times, after that stops
int trySentDoubleMessage(int network_socket, double k) {
    int status = 0; // stores status of connection
    int count_try = 1; // count number of attempts to slave

    do {
        // send k (discretization interval)

        status = send(network_socket, &k, sizeof(k), 0);
        // verifies if sent information correctly
        if(status == -1) {
            printf("(Master) Tentativa %d falhou..\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 6) {
        close(network_socket);
        printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
        exit(0);
    }
}

// Try to sent an int message 5 times, after that stops
int trySentIntMessage(int network_socket, int k) {
    int status = 0; // stores status of connection
    int count_try = 1; // count number of attempts to slave

    do {
        // send k (discretization interval)
        status = send(network_socket, &k, sizeof(k), 0);
        // verifies if sent information correctly
        if(status == -1) {
            printf("(Master) Tentativa %d falhou..\n", count_try);
        }
        count_try++;
    }  while(status == error && count_try <= 5);
    if(count_try == 6) {
        close(network_socket);
        printf("(Master) Não foi possível enviar a mensagem ao escravo.\n");
        exit(0);
    }
}

// Pauses for a specified number of milliseconds.
void do_sleep( clock_t wait )
{
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

// start sending messages to slaves so they can proccess and return our answer
int startCommunication(int *network_socket, int slave_number) {
    // params for the Distributed System
    double k=0; // discretization interval number
    
    // init and final intervals of which slave is going to work   
    int init=0;      
    int final=100;

    // vars which is gonna store the partial integral result and final_result respectively
    double partial_result = 0; // result provided by a slave adding in a sum
    double final_result = 0; // final result = sum of all slaves result

    // variables to mesure processment time
    double start, finish;
    double duration;
    
    int interval = calculate_interval(slave_number, init, final);
    final = interval;

    int i = 0;
    // close slaves which isn't going to be used, -100.0 is the code for close slave connection
    for (i = slave_number; i < 10; i++) {
        trySentDoubleMessage(network_socket[i], -100.0);
    }

    // discretization interval which going to be sent to server.
    printf("(Master) Digite o valor do intervalo de discretização: ");
    scanf("%lf", &k);

    start = get_time(); // get start time operation
    // sends all parameters needed to calculation of partial integral values by slaves
    for (i = 0; i < slave_number; i++) {
        // send discretization interval for server
        trySentDoubleMessage(network_socket[i], k);

        // send init interval for server
        trySentIntMessage(network_socket[i], init);

        // send final interval for server
        trySentIntMessage(network_socket[i], final);

        // calculate new intervals to next slave
        init = init + interval;
        final = init + interval;
    }
    // receive all partial integral values of slaves
    for (i = 0; i < slave_number; i++) {
        // recieve processed data from each server (integral answer)
        recv(network_socket[i], &partial_result, sizeof(partial_result), 0);

        // in each iteration final_result is added to partial_results
        final_result = final_result + partial_result;
    }

    finish = get_time(); // get finish time operation
    duration = finish - start;
    printf("\n Tempo de execução: %2.6lf segundos\n", duration); // shows spend time in integral calculation
    // print out final server's response
    printf("(Mestre) Resultado final da integral: %f\n", final_result);

    return 1;
}

// set up socket configurations for each socket which is going to connect with slaves 
int setUpConnection(int network_socket, struct sockaddr_in server_address, int port_number) {

    network_socket = socket(AF_INET, SOCK_STREAM, 0); // define properties of network_socket
    server_address.sin_port = htons(port_number); // htons converts port from integer to an apropriate format

    //  0 => OK 
    // -1 => SOME ERROR IN CONNECTION
    
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // check for error with the connection
    if(connection_status == -1) {
        printf("\n\nerrno %d: %s\n\n", errno, strerror(errno));
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
    server_address.sin_addr.s_addr = INADDR_ANY;

    // get number of slaves which is going to be used
    printf("(Master) Digite o número de escravos: ");
    scanf("%d", &slave_number);
    
    // create an array which stores 10 network_sockets
    int * network_socket = malloc(10*sizeof(int));

    // set up connections
    int port_number = 9000; // starts at 9000 port ends at 9009
    for(int i=0; i<10; i++) {
        network_socket[i] = setUpConnection(network_socket[i], server_address, port_number);
        port_number++;
    }

    // start communication with slaves, so they can calculate our integral
    startCommunication(network_socket, slave_number);

    // close the socket
    for(int i=0; i<slave_number; i++)
        close(network_socket[i]);

    return 0;
}