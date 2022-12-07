#include "header.h"
int main(int argc,char* argv[]){
        char cwd[1000];
        int clisock1 =socket(AF_INET,SOCK_STREAM,0);
        char new[100];
        int port1,port2;
        port1 = atoi(argv[1]);
        port2 = atoi(argv[2]);
        if(clisock1<0){
                printf("Error in creating the control socket!");
                exit(1);
        }
        printf("Control socket created!");
        char uname[100],pwd[20],file[100],buffer[1024];
        struct sockaddr_in serv_addr;
        serv_addr.sin_family =AF_INET;
        serv_addr.sin_port =htons(port1);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        int status1 = connect(clisock1,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

        if(status1<0){
                printf("\n Error in creating connection!");
                exit(1);
        }
        while(1){
                printf("\n Enter the username: ");
                scanf("%s",uname);
                send(clisock1,uname,strlen(uname)+1,0);
                printf("Enter the password: ");
                scanf("%s",pwd);
                send(clisock1,pwd,strlen(pwd)+1,0);
                recv(clisock1,buffer,sizeof(buffer),0);
                if((strncmp(buffer,"valid",strlen("valid")))==0){
                        printf("Valid login!\n");
                        break;
                }
                else{
                        printf("Invalid login! Please try again!");
                }
        }
        serv_addr.sin_port =htons(port2);
        int clisock2 = socket(AF_INET,SOCK_STREAM,0);
        if(clisock2<0){
                printf("Error in creating the data connection!");
                exit(1);
        }
        printf("Data socket is created\n");
        int status2 = connect(clisock2,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

        if(status2<0){
                printf("\n Error in creating connection!");
                exit(1);
        }
        printf("\nData Connection established with the ftp server!");
        while(1){
                printf("Enter the file name: ");
                scanf("%s",file);

                if(strncmp(file,"exit",strlen("exit"))==0){
                        send(clisock2,"exit",strlen("exit")+1,0);
                        printf("Disconnecting from the server!");
                        break;
                }
                send(clisock2,file,strlen(file)+1,0);
                recv(clisock2,buffer,sizeof(buffer),0);
                if((strncmp(buffer,"found",strlen("found")))==0){
                        printf("File found!");
                        printf("\nRequest for file content?(y/n) ");
                        scanf("%s",new);
                        send(clisock2,new,strlen(new)+1,0);

                        if((strncmp(new,"yes",strlen("yes")))==0){
                                recv(clisock2,buffer,sizeof(buffer),0);
                                FILE *fd = fopen(file,"w");
                                int i=0;
                                for( ;i<sizeof(buffer)&&buffer[i]!='\0';i++);
                                fwrite(buffer,sizeof(char),i,fd);


                                printf("The content of the file is:\n %s",buffer);
                                bzero(&buffer,sizeof(buffer));

                        }
                }
                else{
                        printf("File not found!Try again!");
                }
        }
        close(clisock1);
        close(clisock2);
        return 0;
}
