#ifndef LOG_H
#define LOG_H
#include "alias.h"
void storeinlog(char * command,char * prevcommand,char *logpath);
void printlog(char * logpath);
void logpurge(char * logpath);
void logexecute(int k,char *home,char *logpath, Alias *head);
void lastcommand(char * logpath,char *prevcommand);
void logmanagement(char * command,char * home,char * logpath,Alias *head);
#endif