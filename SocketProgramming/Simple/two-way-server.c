#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
int main()
{
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;

    int welcomeSocket, newSocket;
    char buffer[1024];
    welcomeSocket = socket(AF_INET,SOCK_STREAM,0);
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(welcomeSocket,5)==0) {
        printf("Listening\n");
    } else {
        printf("Error\n");
    }

    socklen_t addr_size;

    addr_size = sizeof(serverStorage);
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverAddr, &addr_size);

    strcpy(buffer,"Server Started\n");
    send(newSocket,buffer,sizeof(buffer),0);
    recv(newSocket,buffer,1024,0);

    printf("DATA RECEIVED : %s\n",buffer);
    printf("Enter MSG : ");
    scanf("%s",buffer);

    send(newSocket,buffer,sizeof(buffer),0);
    return 0;


}