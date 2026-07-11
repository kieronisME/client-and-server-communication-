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

    struct sockaddr_in s_client;

    //socket creation
    SOCKET client = SOCKET_ERROR;
    return_value  = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    s_client.sin_family      = AF_INET;
    s_client.sin_port        = htons(PORT);
    s_client.sin_addr.s_addr = inet_addr(ADDRESS);

    //bind socket to address
    return_value = bind(client,(struct sockaddr*)&s_client, sizeof(s_client));
    return_value = connect(client,(struct sockaddr*)&s_client, sizeof(s_client));
    while(1){
        

    }




    return 0;
}