#include "proclore.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define maxlen 4096
#define RESET   "\033[0m"    // Reset to default
#define BLACK   "\033[1;30m" // Bold Black
#define RED     "\033[1;31m" // Bold Red
#define GREEN   "\033[1;32m" // Bold Green
#define YELLOW  "\033[1;33m" // Bold Yellow
#define BLUE    "\033[1;34m" // Bold Blue
#define MAGENTA "\033[1;35m" // Bold Magenta
#define CYAN    "\033[1;36m" // Bold Cyan
#define WHITE   "\033[1;37m" // Bold White
void proclore(char *command)
{
    
    char *words[100];
    int wordcount=0;
    char *token;
    token = strtok(command, " \t");
    while (token != NULL) {
        words[wordcount]=strdup(token);
        wordcount++;
        token = strtok(NULL, " \t");
    }

    
    if(wordcount==1)
    {
        
        char fileinfo[4096];
        int pid=getpid();
        snprintf(fileinfo,4096,"/proc/%d/stat",pid);
        FILE *fptr=fopen(fileinfo,"r");
        char temp[4096];
        char pids[4096];
        char groupid[4096];
        char virtualsize[4096];
        char foregrouppid[4096];
        char state[4096];
        for(int i=1;i<=23;i++)
        {
            fscanf(fptr,"%s",temp);
            if(i==1)
            {
               strcpy(pids,temp);
            }
             if(i==3)
            {
                strcpy(state,temp);
            }
             if(i==5)
            {
                strcpy(groupid,temp);
            }
             if(i==8)
            {
                strcpy(foregrouppid,temp);
            }
             if(i==23)
            {
                strcpy(virtualsize,temp);
            }
        }
        printf("pid: %s\n",pids);
        if(strcmp(foregrouppid,groupid)==0)
        {
            printf("Process Status %s+\n",state);
        }
        else
        {
            printf("Process Status %s\n",state);
        }
        printf("Process Group id: %s\n",groupid);
        printf("Virtual Memory Size: %s\n",virtualsize);
        fclose(fptr);
        char executablestack[4096];
        snprintf(executablestack,4096,"/proc/%d/exe",pid);
        char exepath[4096];
        ssize_t len=readlink(executablestack,exepath,4096);
        if(len!=-1)
        {
            exepath[len]='\0';
            printf("Executable Path: %s\n",exepath);
        }
        else
        {
            printf("%sExecutable Path: Not found%s\n",RED,RESET);
        }


    }
    else
    {
        if(wordcount>2)
        {
            printf("%sPlease enter valid command%s\n",RED,RESET);
        }
        else
        {
            char fileinfo[4096];
            int pid=atoi(words[1]);
            snprintf(fileinfo,4096,"/proc/%d/stat",pid);
            FILE *fptr=fopen(fileinfo,"r");
            if(!fptr){
                printf("%sProcess with pid %d not found%s\n",RED,pid,RESET);
                return;
            }
            char temp[4096];
            char pids[4096];
            char groupid[4096];
            char virtualsize[4096];
            char foregrouppid[4096];
            char state[4096];
            for(int i=1;i<=23;i++)
            {
                fscanf(fptr,"%s",temp);
                if(i==1)
                {
                    strcpy(pids,temp);
                }
                 if(i==3)
                {
                    strcpy(state,temp);
                }
                 if(i==5)
                {
                    strcpy(groupid,temp);
                }
                 if(i==8)
                {
                    strcpy(foregrouppid,temp);
                }
                 if(i==23)
                {
                    strcpy(virtualsize,temp);
                }
            }
            printf("pid: %d\n",pid);
            if(strcmp(foregrouppid,groupid)==0)
            {
                printf("Process Status %s+\n",state);
            }
            else
            {
                printf("Process Status %s\n",state);
            }
            printf("Process Group id: %s\n",groupid);
            printf("Virtual Memory Size: %s\n",virtualsize);
            fclose(fptr);
            char executablestack[4096];
            snprintf(executablestack,4096,"/proc/%d/exe",pid);
            char exepath[4096];
            ssize_t len=readlink(executablestack,exepath,4096);
            if(len!=-1)
            {
                exepath[len]='\0';
                printf("Executable Path: %s\n",exepath);
            }
            else
            {
                printf("%sExecutable Path: Not found%s\n",RED,RESET);
            }



        }
    }

    
}