#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#define PORT 2947

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int i,max_clients = 5, max_sd, sd, activity,j;
    int count = 0, ret = 0;
    int addrlen = sizeof(address);
    int client_socket[5] = {0};
    char hello[1024] ;
	int sock_added = 0;

    for (i = 0; i < max_clients; i++)  {  
        client_socket[i] = 0;  
    }  

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    int flags; 


	/* Set socket to non-blocking */ 

	if ((flags = fcntl(server_fd, F_GETFL, 0)) < 0) 
	{ 
		perror("fcntl get error\n"); 
	} 


	if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0) 
	{ 
		perror("fcntl set error\n");
	} 
   //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen
    if (listen(server_fd, max_clients) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	while (count < 100)
	{
		/* New client connection */
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
						   (socklen_t*)&addrlen)) < 0) {
			perror("accept");
		} else {
		   //inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , 
				 new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
			for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0  && new_socket > 0)  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets index %d\n" , i);  
                    break; 
                }
            } 
		}

        //send data to all clients
        sprintf(hello,"Hello from server: count %d",count);

		for (j=0; j<5 ;j++) {
			if( client_socket[j] != 0 )  
			{ 
				fprintf(stderr, "Send start..\n");
				fflush(stderr);
				ret = send(client_socket[j], hello , 1024 , MSG_NOSIGNAL );
				fprintf(stderr, "Send ret = %d - \n", ret);
				fflush(stderr);
				if (-1 != ret) {
					fprintf(stderr, "Send success :\n");
				    fflush(stderr);
				} else {
					fprintf(stderr, "Send fail\n");
				    fflush(stderr);
					perror("send error \n"); 
					close(client_socket[j]);
					client_socket[j] = 0;
				}
				printf("Message count %d sent to client:%d : errorno %d\n",count, j, errno);
				//sleep(0.25);
			}
		}
		
		count++;
        sleep(1);
    }
	perror("server error");
	printf("Message count %d \n",count);

	//close all clients
	//sprintf(hello,"Hello from server: count %d",count);
	for (j=0; j<5 ;j++) {
		if( client_socket[j] != 0 )  
		{ 
			close(client_socket[j]);
		}
	}

	printf("Clients closed\n");
    close(server_fd);

	printf("Server closed\n");
    return 0;
}


