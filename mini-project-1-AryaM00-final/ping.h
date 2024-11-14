#ifndef PING_H
#define PING_H
extern int fg_pid;
void ping(char *command);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
#endif