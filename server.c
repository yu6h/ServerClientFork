#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>                            //Server – server.c, gcc server.c -o server -lunp
#include <time.h>
#include <sys/wait.h>

double getCard();

int main(int argc , char *argv[]){
    char receiveMessage[256] = {};
    char message[256] = {"Hi, this is server.\n"};
    int sockfd = 0, clientSockfd = 0;          
    double player_score = 0;    
    pid_t pid;
                      
    sockfd = socket(AF_INET , SOCK_STREAM , 0); //socket的建立
    if (sockfd == -1){         printf("Fail to create a socket.");     }
    struct sockaddr_in serverInfo,clientInfo;                        //socket的連線
    int addrlen = sizeof(clientInfo);
   
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8700);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd, 8);
    do{
      clientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
      pid = fork();
      if (pid < 0) {      // error occurred 
        fprintf(stderr, "Fork Failed");
        return 1;
      }

    }while(pid!=0);

    send(clientSockfd, message, sizeof(message),0);
    while(1){
        
        recv(clientSockfd, receiveMessage, sizeof(receiveMessage),0);
        printf("Client said: %s\n",receiveMessage);
        
        if(strcmp("more",receiveMessage)==0){
            player_score = player_score + getCard();
            if(player_score<11.5){
                // printf("%f, player_score\n", player_score);
                sprintf(message,"%lf",player_score);    
            }else if (player_score==11.5){
                strcpy(message,"you win, 11.5 get");
            }else{
                strcpy(message,"you lose , greater than 11.5");
            }
            send(clientSockfd, message, sizeof(message),0);
           
        }
        else if(strcmp("end",receiveMessage)==0){
            break;
        }
    }

    printf("%s, close Socket\n", receiveMessage);
    close(sockfd);
    return 0;
}

double getCard() {
    srand( time(NULL));
    /* 指定亂數範圍 */
    int min = 1;
    int max = 13;
    /* 產生 [min , max] 的整數亂數 */
    int card = rand() % (max - min + 1) + min;
    double point = (double)card;
    if (point >10.0) point = 0.5;
    return point;
}

