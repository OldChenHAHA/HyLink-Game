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
    //WriteWord_LCD12864(0x80,"Hello LCD12864 Welcome to RaspberryPi");
    WriteWord_LCD12864_2("Hello LCD12864 Welcome to RaspberryPi");
    printf("Peripherals init done!\n");
}

void Disp_ADC_Value(){
    char buf[64];
    sprintf(buf, "ADC Channel 1 : %10.5f V", ADC_Values[0]); 
    Clear_LCDScreen();
    WriteWord_LCD12864_2(buf);
    printf("Refresh LCD\n");
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
    setsockopt(serv_sock, SOL_SOCKET,SO_LINGER, (char *)&li, sizeof(li));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind to port error\n");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {
        printf("listen() error\n");
        exit(1);
    }


    int flags = fcntl(serv_sock, F_GETFL, 0);
    fcntl(serv_sock, F_SETFL, flags | O_NONBLOCK);

    const char * ClientRecv = "ok!";

    while(1){

        AD7606_FetchValue();
        Disp_ADC_Value();

        socklen_t clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,
                   &clnt_addr_size);
        if (clnt_sock < 0) 
        {
            if (errno==EAGAIN || errno == EWOULDBLOCK)
            {
                delay(1000);
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

            long recv_start_time, recv_finish_time;
            double duration;
	    int disconnect_flag=0;

	        ssize_t size;
    	    AD7606_FetchValue();
            Disp_ADC_Value();
    	    
#ifdef DEBUG_INFO
            printf("send data begin\n");
#endif

            size = send(clnt_sock, ADC_Bytes, sizeof(ADC_Bytes), MSG_DONTWAIT);
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

#ifdef DEBUG_INFO
            printf("read data begin\n");
#endif

            recv_start_time = clock();
            while(1){
                size = recv(clnt_sock, buf, sizeof(buf), MSG_DONTWAIT);
                if ( size > 0 )
                {

#ifdef DEBUG_INFO
                    printf("Receive msg from client: %s\n", buf);
#endif

                    break;
                }
                recv_finish_time = clock();
                duration = (double)(recv_finish_time - recv_start_time) / CLOCKS_PER_SEC; 
#ifdef DEBUG_INFO
                //printf("duration: %f\n", duration);
#endif          

		if (duration > 5)
		{
		    disconnect_flag = 1;
		    break;
            	
		}
	    }
            
	    if (disconnect_flag == 1)
	    {
	        printf("Receive msg time out! \n");
                break;
            }            

            if ( strcmp(buf, ClientRecv) != 0)
            {
                printf("Client's msg is not 'ok!' \n");
                break;
            }

	        delay(1000);

        }


        printf("disconnected with ip: %s and port: %d\n",
               inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
               ntohs(clnt_addr.sin_port));
        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}

