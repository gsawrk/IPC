#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#define PORT 2947
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0,i, ret;
	int sock_available = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    } else {
		sock_available = 1;
	}
	#if 1
	//set of socket descriptors 
    fd_set readfds;
	FD_ZERO(&readfds);
	//add read socket to set 
    FD_SET(sock, &readfds); 
	
	while(sock_available){
		ret = select(sock + 1, &readfds, NULL, NULL, NULL);
        if (ret > 0) {
			ret = read(sock, buffer, 1024);
			if (ret > 0){
				printf("ret = %d\t%s\n", ret, buffer);
			} else {
			    sock_available = 0;
				printf("read failure %d: errno = %d\n", ret, errno);
				printf("Close response : %d\n",close(sock));
				sock = -1;	
			}
		} else {
			sock_available = 0;
			printf("select failure %d: errno = %d\n", ret, errno);
			printf("Close response : %d\n",close(sock));
			sock = -1;		
		}
		
	}
	
	#else
    for (i = 0; i < 10 ; i++) {
    read(sock, buffer, 1024);
    //sleep(1);
    printf("%s\n",buffer );
    }
	#endif
	printf("Client exiting...\n");

    return 0;
}
