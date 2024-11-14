#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "alias.h"
void mapofaliases(Alias *aliasarray)
{
    for(int i=0;i<10;i++)
    {
        aliasarray[i].alias=(char*)malloc(1000);
        aliasarray[i].originalcommand=(char*)malloc(4096);
        aliasarray[i].valid=0;
    }
    FILE *file=fopen(".myshrc","r");
    if(file==NULL)
    {
        return;
    }
    char line[4096];
    int count=0;
    char *tok=(char*)malloc(4096);

    while(fgets(line,4096,file)!=NULL)
    {
        


        tok=strtok(line,"=");

        char alias[1000];
        strcpy(alias,tok);
        tok=strtok(NULL,"=");
        char originalcommand[4096];
        strcpy(originalcommand,tok);
        strcpy(aliasarray[count].alias,alias);
        strcpy(aliasarray[count].originalcommand,originalcommand);
        aliasarray[count].valid=1;
        count++;
        if(count==10)
        {
            break;
        }

    }
    fclose(file);
}

