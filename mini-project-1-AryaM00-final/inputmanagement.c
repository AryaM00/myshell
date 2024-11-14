#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "inputmanagement.h"
#include "functionmanagement.h"
#include "log.h"
#include "alias.h"
#define maxlen 4096
void commandmanagement(char * command,char * home,char * logpath,Alias *head)
{
    char * token=(char*)malloc(sizeof(char)*maxlen);

    while(token=strtok_r(command,";\n",&command))
    {
        // first i will tokenize the command based on ;
        // then i will tokenize based of & and send all the tokens except the last one to function with mode flag 1 and the last token will be sent with modeflag 1 if my input command 
        // has & at the end otherwise modeflag 0
        if(strstr(token,"|")!=NULL)
        {
            pipehandler(token,home,logpath,head);
            continue;
        }

        char *words[100];
        char * token2=(char*)malloc(sizeof(char)*maxlen);
        int it=0;
        char *temp=(char*)malloc(sizeof(char)*maxlen);  
        strcpy(temp,token);
        int ampersandflag=0;
        // if last non space character is & then i will set ampersandflag to 1
        for(int i=strlen(temp)-1;i>=0;i--)
        {
            if(temp[i]==' ')
            {
                continue;
            }
            else if(temp[i]=='&')
            {
                ampersandflag=1;
                break;
            }
            else
            {
                break;
            }
        }
        while(token2=strtok_r(token,"&",&token))
        {
            words[it]=token2;
            it++;
        }
        if(ampersandflag==1)
        {
            // this means that the last token is empty and the command has & at the end
            for(int i=0;i<=it-1;i++)
            {
                // printf("words[i]:%s ampersandflag:1\n",words[i]);
                // if words[i] is empty or just spaces then i will not send it to function
                int flag=0;
                for(int j=0;j<strlen(words[i]);j++)
                {
                    if(words[i][j]!=' ')
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    continue;
                }

                function(words[i],home,logpath,1,head);
               
            }
           
        }
        else
        {
            for(int i=0;i<it-1;i++)
            {
                // printf("words[i]:%s ampersandflag:1\n",words[i]);
                int flag=0;
                for(int j=0;j<strlen(words[i]);j++)
                {
                    if(words[i][j]!=' ')
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    continue;
                }

                function(words[i],home,logpath,1,head);
            }
            int flag2=0;
            for(int j=0;j<strlen(words[it-1]);j++)
            {
                if(words[it-1][j]!=' ')
                {
                    flag2=1;
                    break;
                }
            }
            if(flag2==0)
            {
                continue;
            }
            // printf("words[it-1]:%s ampersandflag:0\n",words[it-1]);
            function(words[it-1],home,logpath,0,head);
        }
    }
    

    free(token);
}