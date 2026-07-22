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
    SOCKET server_socket = SOCKET_ERROR;
    SOCKET client_socket = SOCKET_ERROR;
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
        if(return_value != 0){
            printf("[ERROR] listeng failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
            return -1;
        }else{
            printf("[SUCSESS] listening for clients\n");
        }

        //accept connetion
        client_size  = sizeof(struct sockaddr_in);
        return_value = accept(server_socket, (struct sockaddr*)&s_client, &client_size);
        if(return_value == INVALID_SOCKET){
            printf("[ERROR] accepting connection on socket _%d_ failed returning: %d\nWSAGetLastError returned: %d ", server_socket, return_value, WSAGetLastError());
            return -1;
        }else{
            printf("[SUCSESS] accepting connection attempts on socket %d\n",server_socket);
        }

        //receive connection
        recv_size  = recv(client_socket, recv_data_buffer, RECV_BUFFER_SIZE, 0);
        if(recv_size == SOCKET_ERROR){
            printf("[ERROR] receiving connection from client socket failed failed returning: %d\nWSAGetLastError returned: %d ", recv_size, WSAGetLastError());
            return -1;
        }else{
            printf("[SUCSESS] ready to receive data \n");
        }

        //turn client IP to string
        LPCSTR VALUE = inet_ntop(AF_INET, &(s_client.sin_addr), client_ip_buffer, sizeof(client_ip_buffer));
        if(return_value = NULL){
            printf("[ERROR] failed to turn IPv4 address into string returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
            return -1;
        }else{
            printf("[SUCSESS] IPv4 address converted into a string\n");
        }

        //clear buffer
        recv_data_buffer[recv_size] = '\0';
        //print to client hopfully 
        printf("Client | IP: %s\n%s\n", client_ip_buffer, recv_data_buffer);
        return_value = send(client_socket, message, strlen(message), 0);
        if(return_value == SOCKET_ERROR ){
            printf("[ERROR] failed to send data to socket _%d_ returning: %d \nWSAGetLastError returned: %d ", client_socket, return_value, WSAGetLastError());
            return -1;
        }
        
    }

    //clean up
    return_value = closesocket(server_socket);
    if(return_value == SOCKET_ERROR){
        printf("[ERROR] failed to close server socket returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] server socket is all cleaned up\n");
    }

    return_value = closesocket(client_socket);
    if(return_value == SOCKET_ERROR){
        printf("[ERROR] failed to close client socket returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] client socket is all cleaned up\n");
    }

    return_value = WSACleanup();
    if(return_value == SOCKET_ERROR){
        printf("[ERROR] WSACleanup failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] WSACleanup has finnished cleaning\n");
    }

    printf("bye bye\n");
    return 0;
}