#ifndef ACTIVITTIES_H
#define ACTIVITIES_H
#define maxlen 4096
typedef struct ProcessInfo
{
    int pid;
    char command[maxlen];
    struct ProcessInfo *next;
    int foreground;
} ProcessInfo;
 
void activity(ProcessInfo *process_list);
#endif