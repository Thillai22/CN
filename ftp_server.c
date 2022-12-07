#include "header.h"
int main(int argc,char* argv[]){
        int servsock1 =socket(AF_INET,SOCK_STREAM,0);
        char new[100],cwd[1000];
        int port1,port2,clisock1,clisock2;
        port1 = atoi(argv[1]);
        port2 = atoi(argv[2]);
        if(servsock1<0){
                printf("[SERVER]Error in creating the control socket!");
                exit(1);
        }
        printf("Server Control socket created!\n");
        char uname[100],pwd[20],file[100],buffer[1024];
        struct sockaddr_in serv_addr;
        serv_addr.sin_family =AF_INET;
        serv_addr.sin_port =htons(port1);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if(bind(servsock1,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
                printf("Error in binding the control socket!\n");
                exit(1);
        }

        if(listen(servsock1,5)<0){
                printf("Error in listening to the port!\n");
                exit(1);
        }
        clisock1 = accept(servsock1,(struct sockaddr *)NULL,NULL);
        if(clisock1)
                printf("[SERVER] Control connection done!\n");
        while(1){
                recv(clisock1,uname,sizeof(uname),0);
                recv(clisock1,pwd,sizeof(pwd),0);

                if((strncmp(uname,"vj",strlen("vj"))==0) && (strncmp(pwd,"vjaj",strlen("vjaj"))==0)){
                        printf("User granted access!\n");
                        send(clisock1,"valid",strlen("valid")+1,0);
                        break;
                }
                else{
                        printf("Invalid access attempt! Try again!\n");
                }
        }
        serv_addr.sin_port = htons(port2);
        int servsock2= socket(AF_INET,SOCK_STREAM,0);
        if(servsock2<0){
                printf("[SERVER]Error in creating the data socket!");
                exit(1);
        }
        if(bind(servsock2,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
                printf("Error in binding the control socket!\n");
                exit(1);
        }

        if(listen(servsock2,5)<0){
                printf("Error in listening to the port!\n");
                exit(1);
        }
        clisock2 = accept(servsock2,NULL,NULL);
        if(clisock2)
                printf("[SERVER] Data connection done!\n");
        while(1){
                recv(clisock2,file,sizeof(file),0);
                if((strncmp(file,"exit",strlen("exit")))==0){
                        printf("Exiting!");
                        close(clisock2);
                        close(clisock1);
                        break;
                }
                else{
                        getcwd(cwd,sizeof(cwd));
                        strcat(cwd,"/Data/");
                        strcat(cwd,file);
                        FILE *fd = fopen(cwd,"r");
                        if(fd<0){
                                printf("Requestedfile not found!\n");
                                send(clisock2,"not found",strlen("not found")+1,0);
                        }
                        else{
                                printf("File found!\n");
                                send(clisock2,"found",strlen("found")+1,0);
                                recv(clisock2,buffer,sizeof(buffer),0);
                                if(strncmp(buffer,"yes",strlen("yes"))==0){
                                        printf("Sending file to the client!");
                                        fread(buffer,sizeof(char),1024,fd);
                                        send(clisock2,buffer,strlen(buffer)+1,0);
                                        bzero(&buffer,sizeof(buffer));
                                }
                        }
                }
        }
        return 0;
}
