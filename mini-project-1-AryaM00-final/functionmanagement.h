#ifndef FUNTIONMANAGEMENT_H
#define FUNTIONMANAGEMENT_H
#include "alias.h"
void function(char * command,char * home, char * logpath, int modeflag,Alias *head);
void add_process(int pid, char *command,int foreground);
void handle_sigchld(int sig);
void remove_process(int pid);
void setforegroundtozero(int pid);
void setforegroundtoone(int pid);
const char *get_command_from_pid(int pid,char *command);
void handle_ctrl_d();


#endif