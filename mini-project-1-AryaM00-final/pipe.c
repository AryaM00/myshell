#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include "functionmanagement.h"
#include "inputmanagement.h"
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
void pipehandler(char * command,char *home,char *logpath,Alias * head)
{
    char commands[100][maxlen];
    int command_count=0;

    // if & is not at then end then i will output the error message
    int andflag=0;
    int totalpipes=0;

    for(int i=0;i<strlen(command);i++)
    {
        if(command[i]=='|')
        {
            totalpipes++;
        }


    }
   
    for(int i=0;i<strlen(command);i++)
    {
        if(command[i]=='&')
        {
            andflag=1;
            continue;
        }
        if(andflag==1 && command[i]!=' ' && command[i]!='\n') 
        {
            printf("Invalid command\n");
            return;
        }
    }
    char *token=(char*)malloc(sizeof(char)*maxlen);
    while(token=strtok_r(command,"|",&command))
    {
        strcpy(commands[command_count],token);
        command_count++;
    }
    if(command_count!=totalpipes+1)
    {
        printf("Invalid command\n");
        return;
    }


    int tempstdin=dup(0);
    int tempstdout=dup(1);
    char inputfile[1000];
    char outputfile[1000];

    if(command_count==1)
    {   
        if(strstr(commands[0],"<")!=NULL)
        {

        // i want to read name of input file it starts after < and continues till the first space
        int i=0;
        while(commands[0][i]!='<')
        {
            i++;
        }
        i++;
        while(commands[0][i]==' ')
        {
            i++;
        }
        int j=0;
        while(commands[0][i]!=' ' && commands[0][i]!='\0' && commands[0][i]!='\n')
        {
            inputfile[j]=commands[0][i];
            i++;
            j++;
        }
        int inputfd=open(inputfile,O_RDONLY);
        if(inputfd<0)
        {
            perror("No such input file");
            return;
        }
        if(dup2(inputfd,0)<0)
        {
            perror("could not duplicate input file descriptor");
            return;
        }
            close(inputfd);
        }
        if(strstr(commands[0],">")!=NULL)
        {

            // i want to read name of output file it starts after > and continues till the first space
            int i=0;
            while(commands[0][i]!='>')
            {
                i++;
            }
            while(commands[0][i]=='>')
            {
                i++;
            }
 
            while(commands[0][i]==' ' )
            {
                i++;
            }
            int j=0;
            while(commands[0][i]!=' ' && commands[0][i]!='\0' && commands[0][i]!='\n')
            {
                outputfile[j]=commands[0][i];
                i++;
                j++;
            }
            int outputfd;
            if(strstr(commands[0],">>")!=NULL)
            {
                outputfd=open(outputfile,O_WRONLY|O_CREAT|O_APPEND,0644);
            }
            else
            {
                outputfd=open(outputfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
            }
            if(outputfd<0)
            {
                perror("Could not open output file");
                return;
            }
            if(dup2(outputfd,1)<0)
            {
                perror("could not duplicate output file descriptor");
                return;
            }
            close(outputfd);
        }
     
        // printf("command is %s\n",commands[0]);
        int flag=0;
        for(int i=0;i<strlen(commands[0]);i++)
        {
            if(flag)
            {
                commands[0][i]=' ';
            }
            if(commands[0][i]=='<')
            {
                commands[0][i]=' ';
                flag=1;
            }
            if(commands[0][i]=='>')
            {
                commands[0][i]=' ';
                flag=1;
            }

        }


      
        // printf("command is %s\n",commands[0]);


        commandmanagement(commands[0],home,logpath,head);  
        dup2(tempstdin,0);
        dup2(tempstdout,1);

        // replace stdin and stdout with original values

    }
    else
    {
        int tempinput=dup(0);
        int tempoutput=dup(1);
        int fd[2];
        int prevoutcurrinp=0;

        for( int i=0;i<command_count;i++)
        {
            if(pipe(fd)<0)
            {
                perror("Could not create pipe");
                return;
            }
            int lflag=0;
            int gflag=0;
            if(strstr(commands[i],"<")!=NULL)
            {
                lflag=1;

                // i want to read name of input file it starts after < and continues till the first space
                int k=0;
                int j=0;
                while(commands[i][k]!='<')
                {
                    k++;
                }
                k++;
                while(commands[i][k]==' ')
                {
                    k++;
                }
                while(commands[i][k]!=' ' && commands[i][k]!='\0' && commands[i][k]!='\n')
                {
                    inputfile[j]=commands[i][k];
                    k++;
                    j++;
                }
                int inputfd=open(inputfile,O_RDONLY);
                if(inputfd<0)
                {
                    perror("No such input file");
                    return;
                }
                if(dup2(inputfd,0)<0)
                {
                    perror("could not duplicate input file descriptor");
                    return;
                }
                close(inputfd);

            }
            if(strstr(commands[i],">")!=NULL)
            {
                gflag=1;

                // i want to read name of output file it starts after > and continues till the first space
                int k=0;
                int j=0;
                while(commands[i][k]!='>')
                {
                    k++;
                }
                while(commands[i][k]=='>')
                {
                    k++;
                }
                while(commands[i][k]==' ')
                {
                    k++;
                }
                while(commands[i][k]!=' ' && commands[i][k]!='\0' && commands[i][k]!='\n')
                {
                    outputfile[j]=commands[i][k];
                    k++;
                    j++;
                }
                int outputfd;
                if(strstr(commands[i],">>")!=NULL)
                {
                    outputfd=open(outputfile,O_WRONLY|O_CREAT|O_APPEND,0644);
                }
                else
                {
                    outputfd=open(outputfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
                }
                if(outputfd<0)
                {
                    perror("Could not open output file");
                    return;
                }
                if(dup2(outputfd,1)<0)
                {
                    perror("could not duplicate output file descriptor");
                    return;
                }
                close(outputfd);
            }
            if(lflag==0)
            {
            dup2(prevoutcurrinp,0);
            }
            
            if(i==command_count-1)
            {
                if(gflag==0)
                {
                if(dup2(tempoutput,1)<0)
                {
                    perror("could not duplicate output file descriptor");
                    return;
                }
                }
               

            }
            else
            {
                if(gflag==0)
                {
                if(dup2(fd[1],1)<0)
                {
                    perror("could not duplicate output file descriptor");
                    return;
                }
                }
            }
            int flag=0;
            for(int j=0;j<strlen(commands[i]);j++)
            {
                if(flag)
                {
                    commands[i][j]=' ';
                }
                if(commands[i][j]=='<')
                {
                    commands[i][j]=' ';
                    flag=1;
                }
                if(commands[i][j]=='>')
                {
                    commands[i][j]=' ';
                    flag=1;
                }

            }

            commandmanagement(commands[i],home,logpath,head);
            close(fd[1]);
            prevoutcurrinp=fd[0];

        }
        dup2(tempinput,0);
        dup2(tempoutput,1);
        close(fd[0]);
        close(fd[1]);



    

        
    }
}