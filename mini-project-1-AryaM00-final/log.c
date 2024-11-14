#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include "log.h"
#include "functionmanagement.h"
#define maxlen 4096
#include "inputmanagement.h"
void lastcommand(char *logpath, char * prevcommand)
{
    // returns the last command stored in the log file
    FILE *fp;
    fp=fopen(logpath,"r");
    if(fp==NULL)
    {
        printf("Could not open this file\n");
        return ;
    }
    char *commands[15];
    int it=0;
    char *token=(char*)malloc(sizeof(char)*4096);
    char *temp=(char*)malloc(sizeof(char)*4096);
    while(fgets(token,4096,fp)!=NULL)
    {
        
        commands[it]=strdup(token);
        it++;
    }
    char *temp2=(char*)malloc(sizeof(char)*4096);
    temp2="noprev";
    if(it==0)
    {
        strcpy(prevcommand,temp2);
        return;
    }
    strcpy(prevcommand,commands[it-1]);
    return ;

}
void storeinlog(char * command,char *prevcommand,char *logpath)
{
    // if the command is not same as the previous command then store and it has "log" is not its substring then store
    if(strcmp(command,prevcommand)!=0 && strstr(command,"log")==NULL)
    {
        // store the command in the log file if total no of commands stored is less than 15 or else delete the oldest command and add the new command
        FILE *fp;
        fp=fopen(logpath,"a+");
        if(fp==NULL)
        {
            printf("Could not open the file\n");
            return;
        }
        // if log has equal to 15 commands store last 14 commands and the new command, clear the file ,add these 15 commands back
        char *commands[15];
        int it=0;
        char *token=(char*)malloc(sizeof(char)*4096);
        while(fgets(token,4096,fp)!=NULL)
        {   
            commands[it]=strdup(token);
            it++;
        }
        if(it<15)
        {
            
            fprintf(fp,"%s",command);
            fflush(fp);
            fclose(fp);
        }
        else
        {
            fclose(fp);
            fp=fopen(logpath,"w");
            for(int i=1;i<15;i++)
            {
                fprintf(fp,"%s",commands[i]);
                fflush(fp);
            }
            fprintf(fp,"%s",command);
            fflush(fp);
            fclose(fp);
        }

    }
}
void logpurge(char * logpath)
{
    // clears all the currently stored command
    FILE *fp;
    fp=fopen(logpath,"w");
    if(fp==NULL)
    {
        printf("Could not open the file\n");
        return;
    }
    fclose(fp);

}
void printlog(char * logpath)
{
    // we have to print from the last command to the first command
    FILE *fp;
    fp=fopen(logpath,"r");
    if(fp==NULL)
    {
        printf("Could not open the file\n");
        return;
    }
    char *commands[15];
    int it=0;
    char *token=(char*)malloc(sizeof(char)*4096);
    char *temp=(char*)malloc(sizeof(char)*4096);
    while(fgets(token,4096,fp)!=NULL)
    {
        commands[it]=strdup(token);
        it++;
    }
    for(int i=0;i<it;i++)
    {
        printf("%s",commands[i]);
    }
    fclose(fp);
}
void logexecute(int k,char *home,char *logpath, Alias *head)
{
    // executes the kth command from bottom of the log file
    FILE *fp;
    fp=fopen(logpath,"r");
    if(fp==NULL)
    {
        printf("Could not open the file\n");
        return;
    }
    char *commands[15];
    int it=0;
    char *token=(char*)malloc(sizeof(char)*4096);
    char *temp=(char*)malloc(sizeof(char)*4096);
    while(fgets(token,4096,fp)!=NULL)
    {
        commands[it]=strdup(token);
        it++;
    }
    if(k>it || k==0)
    {
        printf("Invalid number\n");
        return;
    }
    else
    {   char * temp=(char*)malloc(maxlen);
        temp=strdup(commands[it-k]);
        commandmanagement(commands[it-k],home,logpath,head);
    }
    fclose(fp);
    

}
void logmanagement(char * command,char * home, char * logpath, Alias *head)
{
    char *words[100];
    char * token;
    int it=0;
    while(token=strtok_r(command," \t\n",&command))
    {
        words[it]=token;
        it++;
    }
    if(it==1)
    {
        printlog(logpath);

    }
    else if(it==2)
    {
        if(strcmp(words[1],"purge")==0)
        {
            logpurge(logpath);
        }
        else
        {
            printf("Enter valid log command\n");
            return ;
        }
    }
    else if(it==3)
    {
        if(strcmp(words[1],"execute")==0)
        {
            int num=atoi(words[2]);

            logexecute(num,home,logpath,head);
            
        }
    }
    else
    {
          printf("Enter valid log command\n");
          return ;

    }
    // i want to write code for proclore
    
    


}

