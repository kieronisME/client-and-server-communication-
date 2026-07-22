#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib ")

#define ADDRESS          "127.0.0.1"
#define PORT             67
#define BUFFER_SIZE 512

// send messages to server on seprate thread
DWORD WINAPI Send_thread_function(LPVOID lpParam);

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

    LPDWORD thread_id;
    HANDLE send_thread = CreateThread(NULL, 0, Send_thread_function, &client, 0, thread_id);

    char recv_buffer[BUFFER_SIZE];
    do{
        return_value = recv(client, recv_buffer, BUFFER_SIZE, 0 );
        recv_buffer[return_value] = '\0';
        if( return_value > 0){
            printf("[SUCSESS] recvied (%d): %s\n", return_value, recv_buffer);

        }else if(!return_value){
            printf("server is closing connection\n");
        }else{
            printf("recived failed WSAGetLastError shows the folloing: %d\n ", WSAGetLastError());
            return -1;
        }


    }while(return_value > 0);
    //clean up to do



    return 0;
}

DWORD WINAPI Send_thread_function(LPVOID lpParam){
    SOCKET client_again = *(SOCKET*)lpParam;

    char send_buffer_length[BUFFER_SIZE];
    char send_buffer;
    while(1){
        int return_value = send(client_again, &send_buffer, send_buffer_length, 0);
        if(return_value != send_buffer_length){
            printf("[ERROR] send failed");
            break;

        }else if (memcmp(send_buffer_length, "/Q", 2) || memcmp(send_buffer_length, "/q", 2)){
            break;
        }

    }

    return 0;
}