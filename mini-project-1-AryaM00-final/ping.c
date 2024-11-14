#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "ping.h"
#include "functionmanagement.h"
#include "activity.h"

#define maxlen 4096
int fg_pid=-1;

void handle_sigint(int sig)
{

    if (fg_pid > 0)  // If a foreground process exists
    {
        kill(fg_pid, SIGINT);  // Send SIGINT to foreground process
        return;
    }

}
void handle_sigtstp(int sig)
{

    if (fg_pid > 0)  // If there's a foreground process
    {
        // if the process is already stopped then i will not stop it again
        if(kill(fg_pid,0)==-1)
        {
            return ;
        }
        
        kill(fg_pid, SIGTSTP);  // Stop the foreground process
        // i want to change foreground flag of the process with fg_pid to 0
        setforegroundtozero(fg_pid);


        fg_pid = -1;

        return;
    }
    else
    {
        return ;
    }
}
void ping(char *command)
{
    char *token=(char*)malloc(sizeof(char)*1000);
    char words[100][1000];
    int word_count=0;
    while(token=strtok_r(command," \t\n",&command))
    {
        strcpy(words[word_count],token);
        word_count++;
    }
    if(word_count!=3)
    {
        printf("Please enter a valid command\n");
        return;
    }
    int pid=atoi(words[1]);
    int signal=atoi(words[2]);
    signal%=32;

    if(kill(pid,0)==-1)
    {
        printf("No such process found\n");
        return;
    }
    if(kill(pid,signal)==-1)
    {
        printf("Error in sending signal\n");
        return;
    }
    printf("Sent signal %d to process with pid %d\n", signal, pid);   


}
