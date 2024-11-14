#include "hop.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define maxlength 4096
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
int hop(char *command,char * prev,char * home)
{
    // if more than one argument is given
    char *words[100];
    char * token;
    int it=0;
    while(token=strtok_r(command," \t\n",&command))
    {
        words[it]=token;
        it++;
    }
    if(it==1)
    {
        if(chdir(home))
        {
            printf("%sCould not change directory%s\n",RED,RESET);
            return -1;
        }
        else
        {
            printf("%s\n",home);
        }
    }
    if(it==2)
    {
        char *cwd=(char*)malloc(sizeof(char)*maxlength);
        char *path=(char*)malloc(sizeof(char)*maxlength);
        char* temp=(char*)malloc(sizeof(char)*maxlength);
        char* temp2=(char*)malloc(sizeof(char)*maxlength);
        if(getcwd(temp2,maxlength)==NULL)
        {
            printf("Could not get the current working directory\n");
            return -1;
        }
        if(words[1][0]=='-')
        {
            if(strcmp(prev,"noprev")==0)
            {
                printf("%sNo previous directory%s\n",RED,RESET);
                return -1;
            }
            else
            {
                if(chdir(prev))
                {
                    printf("%sCould not change directory%s\n",RED,RESET);
                    return -1;
                }
                else
                {
                    strcpy(prev,temp2);
                    getcwd(cwd,maxlength);
                    printf("%s\n",cwd);
                }
            }
            return 0;
        }
        if(words[1][0]=='~')
        {
            strcpy(temp,home);
            strcat(temp,words[1]+1);
            strcpy(path,temp);
        }
        else
        {
            if(strcmp("-",words[1])==0)
            {
                strcpy(path,prev);

            }
            else
            {

            strcpy(path,words[1]);
            }
        }
        if(chdir(path))
        {
            printf("%sCould not change directory%s\n",RED,RESET);
            return -1;
        }
        else
        {
            //strcpy can create problem 
            strcpy(prev,temp2);
            getcwd(cwd,maxlength);
            printf("%s\n",cwd);
        }
        free(cwd);
        free(path);
        free(temp);
        free(temp2);
    }
    else if(it>2)
    {

        char *cwd=(char*)malloc(sizeof(char)*maxlength);
        char path[maxlength];
        char temp[maxlength];
        char *temp2=(char*)malloc(sizeof(char)*maxlength);
        getcwd(temp2,maxlength);
        if(words[1][0]=='~')
        {
            strcpy(temp,home);
            strcat(temp,words[1]+1);
            strcpy(path,temp);
        }
        else
        {
            if(strcmp("-",words[1])==0)
            {
                strcpy(path,prev);

            }
            else
            {

            strcpy(path,words[1]);
            }
        }
        if(chdir(path))
        {
            printf("%sCould not change directory%s\n",RED,RESET);
            return -1;
        }
        else
        {
            strcpy(prev,temp2);
            getcwd(cwd,maxlength);
        }
        for(int i=2;i<it;i++)
        {
            if(words[i][0]=='~')
            {
                strcpy(temp,home);
                strcat(temp,words[i]+1);
                strcpy(path,temp);
            }
            else
            {
                if(strcmp("-",words[i])==0)
                {
                    strcpy(path,prev);

                }
                else
                {

                strcpy(path,words[i]);
                }
                
            }
            getcwd(cwd,maxlength);
            strcpy(temp2,cwd);
            if(chdir(path))
            {
                printf("%sCould not change directory%s\n",RED,RESET);
                return -1;
            }
            else
            {
                strcpy(prev,temp2);
            }
            
        }
        getcwd(cwd,maxlength);
        printf("%s\n",cwd);
        free(cwd);
        free(temp2);
    }
    return 0;
}
