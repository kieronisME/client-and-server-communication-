#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define ADDRESS     "127.0.0.1"
#define PORT        67
#define BUFFER_SIZE 500

// send messages to server on seprate thread
DWORD WINAPI Send_thread_function(LPVOID lpParam);

int main (){
    int return_value;
    WSADATA data;
    return_value = WSAStartup(MAKEWORD(2,2), &data);
    if(return_value != 0){
        printf("\n[ERROR] WSAstart up failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] WSAStartup initiated\n");
    }

    //socket creation
    SOCKET client = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == SOCKET_ERROR){
        printf("\n[ERROR] socket creation failed returning: %d\nWSAGetLastError returned: %d ", client, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] socket created\n");
    }

    struct sockaddr_in s_client;
    s_client.sin_family      = AF_INET;
    s_client.sin_port        = htons(PORT);
    s_client.sin_addr.s_addr = inet_addr(ADDRESS);


    return_value = connect(client, (struct sockaddr*)&s_client, sizeof(s_client));
    if(return_value == SOCKET_ERROR){
        if(WSAGetLastError() == 10061){
            printf("\n[ERROR] connecting to socket failed WSAGetLastError returned: %d, make sure server is on first", WSAGetLastError());
            return -1;
        }
        printf("\n[ERROR] connecting to socket failed returning: %d\nWSAGetLastError returned: %d ", return_value, WSAGetLastError());
        return -1;
    }else{
        printf("[SUCSESS] connected\n");
    }

    printf("count in main\n");
    DWORD thread_id;
    HANDLE send_thread = CreateThread(NULL, 0, Send_thread_function, &client, 0, &thread_id);
    if (send_thread){
        printf("[SUCSESS] send thread printed with ID: %d\n", thread_id);
    }else{
        printf("[ERROR] send thread failed returning: %d\n ", GetLastError());
    }

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

    //connnection done close thread
    return_value = CloseHandle(send_thread);
    if(return_value){
        printf("[SUCSESS] send thread closed\n");

    }

    //cleanup
    return_value = shutdown(client, SD_BOTH);
    if(return_value == SOCKET_ERROR){
        printf("\n[ERROR] shutdown failed returning: %d\nWSAGetLastError shows the folloing: %d\n", return_value, WSAGetLastError());
        closesocket(client);
        WSACleanup();
        return -1;

    }

    closesocket(client);
    WSACleanup();

    return 0;
}

DWORD WINAPI Send_thread_function(LPVOID lpParam){
    SOCKET client_again = *(SOCKET*)lpParam;
    char send_buffer[BUFFER_SIZE];
    int send_buffer_length;
    int return_value;

    while(1){
        memset(send_buffer, 0, sizeof(send_buffer));
        fgets(send_buffer, sizeof(send_buffer), stdin);
        send_buffer_length = strlen(send_buffer);     
        return_value = send(client_again, send_buffer, send_buffer_length, 0);
        if(return_value != send_buffer_length){
            printf("\n[ERROR] send failed\n");
            break;
        }
    
    }

    return 0;
}
