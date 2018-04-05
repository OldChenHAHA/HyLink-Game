/*

	run in terminal: ./server <port>

*/


#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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


#define ADC_VALUE_DISP

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
    socklen_t clnt_addr_size;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        printf("sock() error\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int optval = 1;
    // setting address and port reusable
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind() error\n");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {
        printf("listen() error\n");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);

    const char * ClientRecv = "ok!";
    while(1){

        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,
                   &clnt_addr_size);
        if (clnt_sock < 0) {
            printf("accept() error\n");
            exit(1);
        }

        char buf[1024];
        printf("connected with ip: %s and port: %d\n",
               inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
               ntohs(clnt_addr.sin_port));

        while (1) {
	   
	        printf("fetch data\n");	
    	    AD7606_FetchValue();
    	    ssize_t size = write(clnt_sock, ADC_Bytes, sizeof(ADC_Bytes));
	        if (size < 0) {
                printf("send() error\n");
                break;
            }
            else{
                printf("send data success\n");
            }
            
/*
            memset(buf, 0, sizeof(buf));
            ssize_t size = read(clnt_sock, buf, sizeof(buf));
            if (size < 0) {
                printf("read() error\n");
                break;
            } else {
                buf[size] = '\0';
            }
	       printf("Receive msg from client: %s\n",buf);

            if ( strcmp(buf, ClientRecv) != 0)
            {
                printf("transmit to client stop \n");
                break;
            }
*/
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

