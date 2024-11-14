#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "functionmanagement.h"
#include "hop.h"
#include "log.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
#include "pipe.h"
#include "activity.h"
#include "ping.h"
#include "fg.h"
#include "bg.h"
#include "iman.h"
#include "alias.h"
#include "neonate.h"
#define RESET "\033[0m"      // Reset to default
#define BLACK "\033[1;30m"   // Bold Black
#define RED "\033[1;31m"     // Bold Red
#define GREEN "\033[1;32m"   // Bold Green
#define YELLOW "\033[1;33m"  // Bold Yellow
#define BLUE "\033[1;34m"    // Bold Blue
#define MAGENTA "\033[1;35m" // Bold Magenta
#define CYAN "\033[1;36m"    // Bold Cyan
#define WHITE "\033[1;37m"   // Bold White
#define maxlen 4096
char prevdirectory[maxlen] = "noprev";
ProcessInfo *process_list = NULL;
// set foreground to zero
void setforegroundtozero(int pid)
{
    ProcessInfo *current = process_list;
    while (current)
    {

        if ((current->pid) == pid)
        {
            current->foreground = 0;
        }
        current = current->next;
    }

}
// set foreground to one
void setforegroundtoone(int pid)
{
    ProcessInfo *current = process_list;
    while (current)
    {

        if ((current->pid) == pid)
        {
            current->foreground = 1;
        }
        current = current->next;
    }

}
void add_process(int pid, char *command,int flag)
{
    ProcessInfo *new_process = (ProcessInfo *)malloc(sizeof(ProcessInfo));
    new_process->pid = pid;
    strcpy(new_process->command, command);
    new_process->next = process_list;
    process_list = new_process;
    new_process->foreground=flag;
}
void remove_process(pid_t pid)
{
    ProcessInfo **current = &process_list;
    while (*current)
    {
        ProcessInfo *entry = *current;
        if (entry->pid == pid)
        {
            *current = entry->next;
            free(entry);
            return;
        }
        current = &entry->next;
    }
}

void handle_sigchld(int sig)
{
    // Clean up all terminated child processes
    int status;
    int pid;
    char *command = (char *)malloc(sizeof(char) * maxlen);
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        get_command_from_pid(pid, command);
        char* wordsincommand[100];
        for(int i=0;i<100;i++)
        {
            wordsincommand[i]=(char*)malloc(maxlen);
        }
        // i want to see the first word of my command;
        char *token;
        int it = 0;
        char *temp = (char *)malloc(sizeof(char) * maxlen);
        strcpy(temp, command);
        while (token = strtok_r(command, " \t\n", &command))
        {
            wordsincommand[it] = strdup(token);
            it++;
        }
        if (WIFEXITED(status))
        {
            printf("%s terminated normally with exit status %d (%d)\n", wordsincommand[0], WEXITSTATUS(status),pid);
        }
        else if (WIFSIGNALED(status))
        {
            printf("%s terminated by signal %d (%d)\n", wordsincommand[0], WTERMSIG(status),pid);
        }
        else if (WIFSTOPPED(status))
        {
            printf("%s stopped by signal %d (%d)\n", wordsincommand[0], WSTOPSIG(status),pid);
        }
        else if (WIFCONTINUED(status))
        {
            printf("%s continued (%d)\n", wordsincommand[0],pid);
        }
        remove_process(pid);
        for(int i=0;i<100;i++)
        {
            free(wordsincommand[i]);
        }
        

    }

}
void handle_ctrl_d()
{
    // Loop through all processes and terminate them
    ProcessInfo *current = process_list;
    while (current)
    {
        kill(current->pid, SIGKILL); // Force kill all processes
        current = current->next;
    }

    printf("\nLogging out and killing all processes.\n");
    exit(0); // Exit the shell
}
const char *get_command_from_pid(int pid, char *command)
{
    ProcessInfo *current = process_list;
    while (current)
    {
        if ((current->pid) == pid)
        {
            strcpy(command, current->command);
            return current->command;
        }
        current = current->next;
    }
    strcpy(command, "Unknown");

    return "Unknown";
}
void function(char *command, char *home, char *logpath, int modeflag, Alias *head)
{
    if (strcmp(command, "exit") == 0)
    {
        printf("Closing the most wonderful shell ever made\n");
        exit(0);
    }
    else
    {
        // i need break command based on spaces
        char *words[100];
        char *token;
        int it = 0;
        char *temp = (char *)malloc(sizeof(char) * maxlen);
        strcpy(temp, command);
     
        // i want to go to create a map of aliases and actual commands

       

        if(strstr(command,"|")!=NULL || strstr(command,"<")!=NULL || strstr(command,">")!=NULL)
        {
            // printf("command");
            pipehandler(command,home,logpath,head);
            return;
        }
        while (token = strtok_r(command, " \t\n", &command))
        {
            words[it] = token;
            it++;
        }
        
       

        int aliasp=0;
        char originalcommand[4096];
        for(int i=0;i<10;i++)
        {
            if(head[i].valid==1)
            {
                if(strcmp(words[0],head[i].alias)==0)
                {
                    aliasp=1;
                    strcpy(originalcommand,head[i].originalcommand);
                    break;
                }
            }
        }
        if (strcmp(words[0], "hop") == 0)
        {
            hop(temp, prevdirectory, home);
        }
        else if (strcmp(words[0], "log") == 0)
        {
            logmanagement(temp, home, logpath);
        }
        else if (strcmp(words[0], "reveal") == 0)
        {
            char *cwd = (char *)malloc(sizeof(char) * maxlen);
            getcwd(cwd, maxlen);
            revealmanangement(cwd, temp,home,prevdirectory);
        }
        else if (strcmp(words[0], "proclore") == 0)
        {
            proclore(temp);
        }
        else if(strcmp(words[0],"seek")==0)
        {
            seek(temp,home,prevdirectory);
            
        }
        else if(strcmp(words[0],"activities")==0)
        {
            activity(process_list);
        }
        else if(strcmp(words[0],"ping")==0)
        {
            ping(temp);
        }
        else if(strcmp(words[0],"fg")==0)
        {
            fg(temp);
            
        }
        else if(strcmp(words[0],"bg")==0)
        {
            bg(temp);
            
        }
        else if(strcmp(words[0],"neonate")==0)
        {
            neonate(temp);
        }
        else if(strcmp(words[0],"iMan")==0)
        {
            iman(temp);
        }
        else if( aliasp==1)
        {

            function(originalcommand,home,logpath,modeflag,head);
            return ;

        }
        
        else
        {
                if (modeflag == 0)
                {
                int pid = fork();
                fg_pid=pid;
                if (pid < 0)
                {
                    printf("%sCould not create a child process%s\n", RED, RESET);
                    return;
                }
                else if (pid == 0)
                {
                    
                    char *words[100];
                    char *token;
                    int it = 0;
                    while (token = strtok_r(temp, " \t\n", &temp))
                    {
                        words[it] = strdup(token);
                        it++;
                    }
                    words[it] = NULL;
                    if (execvp(words[0], words) == -1)
                    {
                        printf("%scommand not found%s\n", RED, RESET);
                        exit(0);
                    }
                    exit(0);
                }
                else
                {
                    add_process(pid, temp,1);
                    waitpid(pid,NULL,WUNTRACED);
                    if(kill(pid,0)==-1)
                    {
                        remove_process(pid);
                    }


                }
                fg_pid=-1;
            }
            else if (modeflag == 1)
            {

                // i have to execute the command in background


                int pid = fork();
                if (pid < 0)
                {
                    printf("%sCould not create a child process%s\n", RED, RESET);
                    return;
                }
                else if (pid == 0)
                {
                    setpgid(0, 0);
                    char *words[100];
                    char *token;
                    int it = 0;
                    while (token = strtok_r(temp, " \t\n", &temp))
                    {
                        words[it] = token;
                        it++;
                    }
                    words[it] = NULL;
                    if (execvp(words[0], words) == -1)
                    {
                        printf("%sCould not execute system command%s\n", RED, RESET);
                        exit(0);
                    }
                    exit(0);
                }
                else
                {
                    printf("%d\n", pid);
                    add_process(pid, temp,0);

                }
            }
        }
    }
}