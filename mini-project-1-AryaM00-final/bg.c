#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "bg.h"
void bg(char * command)
{
    char *token=(char*)malloc(sizeof(char)*1000);
    char words[100][1000];
    int word_count=0;
    while(token=strtok_r(command," \t\n",&command))
    {
        strcpy(words[word_count],token);
        word_count++;
    }
    if(word_count!=2)
    {
        printf("Please enter a valid command\n");
        return;
    }
    // Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, print “No such process found” to the terminal.
    int pid=atoi(words[1]);
    if(kill(pid,0)==-1)
    {
        printf("No such process found\n");
        return;
    }
    if(kill(pid,SIGCONT)==-1)
    {
        printf("Error in sending signal\n");
        return;
    }
    
}