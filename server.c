#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib ")

#define ADDRESS          "127.0.0.1"
#define PORT             67
#define RECV_BUFFER_SIZE 500


int main (){
    int return_value;
    WSADATA data;
    return_value = WSAStartup(MAKEWORD(2,2), &data);
    if(return_value != 0){
        printf("[ERROR] WSAstart up failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] WSAStartup initiated\n");
    }
    struct sockaddr_in s_server;
    struct sockaddr_in s_client;


    //socket creation
    SOCKET server_socket;
    SOCKET client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket == INVALID_SOCKET){
        printf("[ERROR] socket creation failed returning: %d\nWSAGetLastError returned: %d ", INVALID_SOCKET, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] socket created\n");
    }


    s_server.sin_family      = AF_INET;
    s_server.sin_port        = htons(PORT);
    s_server.sin_addr.s_addr = inet_addr(ADDRESS);

    return_value = bind(server_socket, (struct sockaddr*)&s_server, sizeof(s_server));
    if(return_value == SOCKET_ERROR){
        printf("[ERROR] binding address to socket failed returning: %d\nWSAGetLastError returned: %d ", INVALID_SOCKET, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] sock bound to address.\nListening for client...\n");
    }

    int recv_size;
    int client_size;
    const char* message = "server received message\n";
    char recv_data_buffer[RECV_BUFFER_SIZE];
    char client_ip_buffer[20];

    while(1){
        // listen for connection
        return_value = listen(server_socket, 3);
        //accept connetion
        client_size  = sizeof(struct sockaddr_in);
        return_value = accept(server_socket, (struct sockaddr*)&s_client, &client_size);
        //receive connection
        recv_size = recv(client_socket, recv_data_buffer, RECV_BUFFER_SIZE, 0);
        //turn client IP to string
        inet_ntop(AF_INET, &(s_client.sin_addr), client_ip_buffer, sizeof(client_ip_buffer));
        //cleasr buffer
        recv_data_buffer[recv_size] = '\0';
        //print to client hopfully 
        printf("Client | IP: %s\n%s\n", client_ip_buffer, recv_data_buffer);
        send(client_socket, message, strlen(message), 0);
    }

//clean up
//todo
    return 0;
}