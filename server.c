#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib ")

#define ADDRESS "127.0.0.1"
#define PORT     67


int main (){
    int return_value;
    WSADATA data;
    return_value = WSAStartup(MAKEWORD(2,2), &data);
    if(return_value != 0){
        printf("[ERROR] WSAstart up failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] WSAStartup initiated");
    }
    struct sockaddr_in s_server;
    struct sockaddr_in s_client;


    //socket creation
    SOCKET server;
    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server == INVALID_SOCKET){
        printf("[ERROR] socket creation failed returning: %d\nWSAGetLastError returned: %d ", INVALID_SOCKET, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] socket created");
    }


    s_server.sin_family      = AF_INET;
    s_server.sin_port        = htons(PORT);
    s_server.sin_addr.s_addr = inet_adrr(ADDRESS);

    return_value = bind(server, (struct sockaddr*)&s_server, sizeof(s_server));
    if(return_value == SOCKET_ERROR){
        printf("[ERROR] binding address to socket failed returning: %d\nWSAGetLastError returned: %d ", INVALID_SOCKET, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] sock bound to address.\nListening for client...");
    }


    




    



    



}