/*

	run in terminal: ./server <port>

*/


#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <time.h>

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

#include "bsp_LCD12864.h"
#include "bsp_AD7606.h"


#define DEBUG_INFO

void InitPeripherals(){
    wiringPiSetup();
    AD7606_Init();
    printf("AD7606 init done!\n");
    
    Init_LCD12864();
    Clear_LCDScreen();
    WriteWord_LCD12864(0x80,"Hello LCD12864 Welcome to RaspberryPi");
    
    printf("Peripherals init done!\n");
}


int main(int argc, char *argv[])
{
    InitPeripherals();

    int serv_sock, clnt_sock;

    struct sockaddr_in serv_addr, clnt_addr;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        printf("Create socket error\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int optval = 1;
    // setting address and port reusable
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct linger li;
    li.l_onoff = 1;
    li.l_linger = 1;
    setsockopt(serv_sock, SOL_SOCKET,SO_LINGER, (chaer *)&li, sizeof(li));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind to port error\n");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {
        printf("listen() error\n");
        exit(1);
    }


    int flags = fcntl(servfd, F_GETFL, 0);
    fcntl(serv_sock, F_SETFL, flags | 0_NONBLOCK);

    const char * ClientRecv = "ok!";

    while(1){

        printf("Waiting connect\n");

        socklen_t clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,
                   &clnt_addr_size);
        if (clnt_sock < 0) 
        {
            if (errno==EAGAIN || errno == EWOULDBLOCK)
            {
                delay(100);
                continue;
            }
            else
            {
                perror("call accept error\n");
                break;
            }
        }

        char buf[1024];
        printf("Connected with ip: %s and port: %d\n",
               inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
               ntohs(clnt_addr.sin_port));

        while (1) {
	   
    	    AD7606_FetchValue();
    	    ssize_t size = write(clnt_sock, ADC_Bytes, sizeof(ADC_Bytes));
	        if (size < 0) {
                printf("send() error\n");
                break;
            }
            else{
                #ifdef DEBUG_INFO
                printf("send data success\n");
                #endif
            }
            

            memset(buf, 0, sizeof(buf));
            ssize_t size = read(clnt_sock, buf, sizeof(buf));
            if (size < 0) {
                printf("read() error\n");
                break;
            } else {
                buf[size] = '\0';
            }
	       // printf("%d Receive msg from client: %s\n", size, buf);

        //     if ( strcmp(buf, ClientRecv) != 0)
        //     {
        //         printf("transmit to client stop \n");
        //         break;
        //     }

	        delay(500);

        }


        printf("disconnected with ip: %s and port: %d\n",
               inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
               ntohs(clnt_addr.sin_port));
        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}

