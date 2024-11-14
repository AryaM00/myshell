#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "fg.h"
#include "ping.h"
#include "functionmanagement.h"

void fg(char * command)
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
    // Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. 
    //Print “No such process found”, if no process with given pid exists.
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
    setpgid(pid, 0);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, pid);
    fg_pid=pid;
    setforegroundtoone(fg_pid);
    int status;

    waitpid(pid, &status, WUNTRACED);
    fg_pid=-1;
    setforegroundtozero(pid);

    tcsetpgrp(STDIN_FILENO, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    if(kill(pid,0)==-1)
    {
        remove_process(pid);
    }

    // i want to change foreground flag of the process with fg_pid to 1
    
    

    // if the process completes i will remove from my process list
    return;
}

