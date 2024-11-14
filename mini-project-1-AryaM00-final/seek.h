#ifndef SEEK_H
#define SEEK_H
#define buff 4096
// #define SA_RESTART 0x10000000
#define buff1 100
#define _XOPEN_SOURCE 500 // Required for nftw function
#include <stdio.h>
#include <ftw.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include<signal.h>
#include<time.h>
#include <sys/stat.h>
#include<sys/resource.h>
#include<pwd.h>
int file_dir_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);
int file_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);
int directory_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);
void seek(char *command, char * home, char * prevdirectory);
#endif