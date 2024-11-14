#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "activity.h"
#define maxlen 4096

char state[100];
#define MAX_ACTIVITIES 100

int compareStrings1(const void *a, const void *b) {
    // The parameters are void pointers, so cast them to char pointers
    const char **str1 = (const char **)a;
    const char **str2 = (const char **)b;

    // Use strcmp to compare the two strings
    return strcmp(*str1, *str2);
}
int cmp(const void *a, const void *b)
{
    return *(int*)a-*(int*)b;
}

void get_process_state(int pid)
{
    char path[maxlen * 2];
    snprintf(path,maxlen * 2,"/proc/%d/stat",pid);
    FILE* file = fopen(path,"r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    char s;
    fscanf(file,"%*d %*s %c",&s);
    fclose(file);
    if (s == 'T') {
       strcpy(state,"Stopped");
    } else {
        strcpy(state,"Running");
    }
    return;
}
char arrayofactivities[100][4096];
int activity_count = 0;

void activity(ProcessInfo *process_list)
{

    ProcessInfo *current = process_list;
    int arr[100];
    for(int i=0;i<100;i++)
    {
        arr[i]=__INT32_MAX__;
    }
    int validcount=0;

    while (current)
    {
        if(current->foreground==1)
        {
            current = current->next;
            continue;
        }
        get_process_state(current->pid);
        // snprintf(arrayofactivities[activity_count], maxlen, "[%d] : [%s] - [%s]", current->pid, current->command, state);
        // printf("[%d] : [%s] - [%s]\n", current->pid, current->command, state);
        arr[validcount]=current->pid;
        validcount++;
        current = current->next;
    }
    qsort(arr,100,sizeof(int),cmp);
    for(int i=0;i<validcount;i++)
    {
        int j=arr[i];
        current = process_list;
        while (current)
        {
            if(current->pid!=j)
            {
                current = current->next;
                continue;   
                
            }
            printf("pid: %d\n",current->pid);
            
            get_process_state(current->pid);
            // snprintf(arrayofactivities[activity_count], maxlen, "[%d] : [%s] - [%s]", current->pid, current->command, state);
            printf("[%d] : [%s] - [%s]\n", current->pid, current->command, state);
            break;
        }

    }
   
}
