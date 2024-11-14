#include "display.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define maxlength 4096
#define RESET   "\033[0m"    // Reset to default
#define BLACK   "\033[1;30m" // Bold Black
#define RED     "\033[1;31m" // Bold Red
#define GREEN   "\033[1;32m" // Bold Green
#define YELLOW  "\033[1;33m" // Bold Yellow
#define BLUE    "\033[1;34m" // Bold Blue
#define MAGENTA "\033[1;35m" // Bold Magenta
#define CYAN    "\033[1;36m" // Bold Cyan
#define WHITE   "\033[1;37m" // Bold White
void displayrequirements(char * home)
{
    char *username=(char*)malloc(sizeof(char)*maxlength);
    getlogin_r(username,maxlength);
    char *sysname=(char*)malloc(sizeof(char)*maxlength);
    gethostname(sysname,maxlength);
    char *cwd=(char*)malloc(sizeof(char)*maxlength);
    getcwd(cwd,maxlength);
    if(strstr(cwd,home)!=NULL)
    {
        memmove(cwd,cwd+strlen(home),strlen(cwd)-strlen(home)+1);
        for(int i=strlen(cwd);i>=0;i--)
        {
            cwd[i+1]=cwd[i];
        }
        cwd[0]='~';
    }
    printf("<%s%s%s@%s%s%s:%s%s%s> ",GREEN,username,RESET,RED,sysname,RESET,BLUE,cwd,RESET);
    free(username);
    free(sysname);
    free(cwd);
    
}
void displayrequirements2(char * home,char * prev, int curr)
{
    char *username=(char*)malloc(sizeof(char)*maxlength);
    getlogin_r(username,maxlength);
    char *sysname=(char*)malloc(sizeof(char)*maxlength);
    gethostname(sysname,maxlength);
    char *cwd=(char*)malloc(sizeof(char)*maxlength);
    getcwd(cwd,maxlength);
    if(strstr(cwd,home)!=NULL)
    {
        memmove(cwd,cwd+strlen(home),strlen(cwd)-strlen(home)+1);
        for(int i=strlen(cwd);i>=0;i--)
        {
            cwd[i+1]=cwd[i];
        }
        cwd[0]='~';
    }
    char words[4096];
    // i just want to replace \n of command with \0 
    for(int i=0;i<strlen(prev);i++)
    {
        if(prev[i]=='\n')
        {
            words[i]='\0';
            break;
        }
        words[i]=prev[i];
    }
    printf("<%s%s%s@%s%s%s:%s%s%s %s : %ds > ",GREEN,username,RESET,RED,sysname,RESET,BLUE,cwd,RESET,words,curr);
    free(username);
    free(sysname);
    free(cwd);
    
}
