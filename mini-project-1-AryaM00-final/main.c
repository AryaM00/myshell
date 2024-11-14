#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include "functionmanagement.h"
#include "display.h"
#include "inputmanagement.h"
#include "log.h"
#include "ping.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "alias.h"

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
int main()
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, 0);
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    char *home=(char*)malloc(sizeof(char)*maxlen);
    getcwd(home,maxlen);
    // create a log file if it does not exist and store its path
    char *logpath=(char*)malloc(sizeof(char)*maxlen);
    char *prevcommand=(char*)malloc(sizeof(char)*maxlen);
    strcpy(logpath,home);
    strcat(logpath,"/log.txt");
    lastcommand(logpath,prevcommand);
    FILE *fptr;
    fptr=fopen(logpath,"a");
    if(fptr==NULL)
    {
        printf("Could not open the file\n");
        return -1;
    }
    int curr=0;
    char prev2[maxlen];
    Alias arrayofaliases[10];
     mapofaliases(arrayofaliases);
    




    while(1)
    {
       
        if(curr>2){
            
            displayrequirements2(home,prev2,curr);
            
        }
        else
        {
            displayrequirements(home);

        }
       
        
        char *command=(char*)malloc(sizeof(char)*maxlen);
        size_t buffers=0;

        if(getline(&command,&buffers,stdin)==-1)
        {
            if (feof(stdin)) {
                handle_ctrl_d(); // End of file (Ctrl-D) detected
            }else
            {
            printf("Please enter the command again\n");
            continue;
            }
        }


        strcpy(prev2,command);
        storeinlog(command,prevcommand,logpath);
        lastcommand(logpath,prevcommand);
        int start=time(NULL);
        commandmanagement(command,home,logpath,arrayofaliases);
        int end=time(NULL);
        curr=end-start;
    }

}