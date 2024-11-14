
#include "iman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#define maxlen 4096
#define maxsize 20000
void iman(char * command)
{
    char *token=(char*)malloc(sizeof(char)*1000);
    char words[100][1000];
    int word_count=0;
    while(token=strtok_r(command," \t\n",&command))
    {
        strcpy(words[word_count],token);
        word_count++;
    }
    char hostname[maxlen]="man.he.net";
    char service[maxlen]="http";

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    s=getaddrinfo(hostname,service,&hints,&result);
    if(s!=0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(s));
        return;
    }
    for(rp=result;rp!=NULL;rp=rp->ai_next)
    {
        sfd=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(sfd==-1)
        {
            continue;
        }
        if(connect(sfd,rp->ai_addr,rp->ai_addrlen)!=-1)
        {
            break;
        }
        close(sfd);
    }
    if(rp==NULL)
    {
        fprintf(stderr,"Could not connect\n");
        return;
    }
    freeaddrinfo(result);
    char locater[maxlen];
    snprintf(locater,maxlen,"/man1/%s.1",words[1]);
    char getrequest[maxlen];
       snprintf(getrequest, sizeof(getrequest), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", words[1]);

 if (write(sfd, getrequest, strlen(getrequest)) == -1)
    {
        perror("write");
        close(sfd);
    }

    char response[maxsize];
    ssize_t bytes_received;
    int flag=0;
        int in_tag = 0;
        int startprint=0;
 
    while (( bytes_received= recv(sfd, response, maxsize - 1, 0)) > 0)
    {
        response[bytes_received] = '\0';
       

        flag=1;


        if(response[0]=='<')
        {
            in_tag=1;
        }
        for(int i=0;i<bytes_received;i++)
        {
            if(response[i]=='<')
            {
                in_tag=1;
                startprint=1;
            }
            if(in_tag==0)
            {
                if(startprint==1)
                {
                    printf("%c",response[i]);
                }
                
            }
            if(response[i]=='>')
            {
                in_tag=0;
            }
        }
    }
    if(flag==0)
    {
        printf("ERROR\n\tNo such command\n");
    }

    if (bytes_received < 0) {
        perror("Response receiving error");
        return ;
        //return remaining_token;

    }

    close(sfd);
    




    

}