#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "reveal.h"
#include <dirent.h>
#include <time.h>

#define RESET   "\033[0m"    // Reset to default
#define BLACK   "\033[1;30m" // Bold Black
#define RED     "\033[1;31m" // Bold Red
#define GREEN   "\033[1;32m" // Bold Green
#define YELLOW  "\033[1;33m" // Bold Yellow
#define BLUE    "\033[1;34m" // Bold Blue
#define MAGENTA "\033[1;35m" // Bold Magenta
#define CYAN    "\033[1;36m" // Bold Cyan
#define WHITE   "\033[1;37m" // Bold White
#define maxlen 4096
int compareStrings(const void *a, const void *b) {
    // The parameters are void pointers, so cast them to char pointers
    const char **str1 = (const char **)a;
    const char **str2 = (const char **)b;

    // Use strcmp to compare the two strings
    return strcmp(*str1, *str2);
}
void reveall( char * path)
{
    char permissions[11]="---------";
    struct stat fileStat;
    if (stat(path, &fileStat) < 0) {
        perror("stat");
        return;
    }
    if (S_ISDIR(fileStat.st_mode)) {
        permissions[0] = 'd'; // Directory
    } else {
        permissions[0] = '-'; // Regular file
    }
    // User (owner) permissions
    permissions[1] = (fileStat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (fileStat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (fileStat.st_mode & S_IXUSR) ? 'x' : '-';
    // Group permissions
    permissions[4] = (fileStat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (fileStat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (fileStat.st_mode & S_IXGRP) ? 'x' : '-';
    // Others permissions
    permissions[7] = (fileStat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (fileStat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (fileStat.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0'; // Null-terminate the string
    int hardlinks=(unsigned int)fileStat.st_nlink;
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    long long int size=fileStat.st_size;
    char time[256];
    if(strftime(time, sizeof(time),"%Y-%m-%d %H:%M:%S", localtime(&fileStat.st_mtime)) == 0) {
        printf("strftime returned 0");
        return;
    }
    char * filename=(char *)malloc(maxlen);
    char* words[100];
    int i=0;
    char *token = strtok(path, "/");
    while (token != NULL) {
        words[i]=strdup(token);
        i++;
        token = strtok(NULL, "/");
    }
    strcpy(filename,words[i-1]);
    // if file is an executable then color it green
    // if file is a directory then color it blue
    if(permissions[0]=='d')
    {
        
        printf("%-11s %2d %-15s %-15s %5lld %s %s%s%s\n",permissions,hardlinks,pw->pw_name,gr->gr_name,size,time,BLUE,filename,RESET);
    }
    else if(permissions[3]=='x' || permissions[6]=='x' || permissions[9]=='x')
    {
        printf("%-11s %2d %-15s %-15s %5lld %s %s%s%s\n",permissions,hardlinks,pw->pw_name,gr->gr_name,size,time,GREEN,filename,RESET);
    }
    else
    {
        printf("%-11s %2d %-15s %-15s %5lld %s %s\n",permissions,hardlinks,pw->pw_name,gr->gr_name,size,time,filename);
    }
}
int sum1=0;
int sum2=0;
void filesandallfiles(char *allfiles[],char * allfilesincludinghidden[],char * path,int* filecount,int* filecount2)
{
    
    *filecount2=0;
    *filecount=0;
    struct stat fileStat;
    if (stat(path, &fileStat) < 0) {
        perror("stat");
        return;
    }
    
    if (S_ISDIR(fileStat.st_mode)) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path)) != NULL) {
            
            while ((ent = readdir(dir)) != NULL) {
                if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0) {
                    allfilesincludinghidden[*filecount2]=strdup(ent->d_name);
                    sum2+=fileStat.st_blocks;

                    (*filecount2)++;
                    continue;
                }

                struct stat fileStat;
                char *path2=(char *)malloc(strlen(path)+strlen(ent->d_name)+2);
                strcpy(path2,path);
                strcat(path2,"/");
                strcat(path2,ent->d_name);
                if (stat(path2, &fileStat) < 0) {
                    perror("stat");
                    return;
                }
                sum2+=fileStat.st_blocks;
                if(ent->d_name[0]=='.')
                {
                    allfilesincludinghidden[*filecount2]=strdup(ent->d_name);
                    
                    (*filecount2)++;
                }
                else
                {
                    sum1+=fileStat.st_blocks;
                    allfiles[*filecount]=strdup(ent->d_name);
                    allfilesincludinghidden[*filecount2]=strdup(ent->d_name);
                    (*filecount2)++;
                   ( *filecount)++;
                }
               
            }
            closedir(dir);
    } else {
        printf("%sError opening directory%s\n",RED,RESET);
    }
    } else {
        char * filename=(char *)malloc(maxlen);
        char *words[100];
        int i=0;
        char *token = strtok(path, "/");
        while (token != NULL) {
            words[i]=strdup(token);
            i++;
            token = strtok(NULL, "/");
        }
        strcpy(filename,words[i-1]);
        if(filename[0]=='.')
        {
            allfilesincludinghidden[*filecount2]=strdup(filename);
            (*filecount2)++;
        }
        else
        {
            allfiles[*filecount]=strdup(filename);
            allfilesincludinghidden[*filecount2]=strdup(filename);
            (*filecount)++;
            (*filecount2)++;
        }
    }
   
    qsort(allfiles,*filecount,sizeof(char *),compareStrings);
    qsort(allfilesincludinghidden,*filecount2,sizeof(char *),compareStrings);

}

void revealmanangement(char * path, char * command,char *home,char* prevdirectory)
{
    sum1=0;
    sum2=0;
   
    char * allfiles[1000];
    char *allfilesincludinghidden[1000];
    for (int i = 0; i < 100; i++) {
    allfiles[i] = (char *)malloc(maxlen * sizeof(char));
    allfilesincludinghidden[i] = (char *)malloc(maxlen * sizeof(char));
    }
    int *filecount=(int *)malloc(sizeof(int));
    int *filecount2=(int *)malloc(sizeof(int));
    filesandallfiles(allfiles,allfilesincludinghidden,path,filecount,filecount2);
    
    int fcount=*filecount;
    int fcount2=*filecount2;
    char *words[100];
    int wordcount=0;
    char *token;


    token = strtok(command, " \t");
    
    while (token != NULL) {
        words[wordcount]=token;
        wordcount++;
        token = strtok(NULL, " \t");
    }
    if(wordcount==1)
    {
         sum1=0;
        sum2=0;
        filesandallfiles(allfiles,allfilesincludinghidden,path,filecount,filecount2);
        for(int i=0;i<fcount;i++)
        {   
            struct stat fileStat;
            if (stat(allfiles[i], &fileStat) == 0) // Get the file status
            {
            if (S_ISDIR(fileStat.st_mode)) // Check if it's a directory
            {
                printf("%s%s%s\n", BLUE, allfiles[i], RESET);
            }
            else if (fileStat.st_mode & S_IXUSR) // Check if it's executable
            {
                printf("%s%s%s\n", GREEN, allfiles[i], RESET);
            }
            else
            {
                printf("%s\n", allfiles[i]);
            }
            }
        }
    }
    else
    {
        int lflag=0;
        int aflag=0;
        int pathflag=0;
        int minusflag=0;
        for(int i=1;i<wordcount;i++)
        {
            
            if(words[i][0]=='-')
            {
                if(strstr(words[i],"l")!=NULL)
                {
                    lflag=1;
                }
                 if(strstr(words[i],"a")!=NULL)
                {
                    aflag=1;
                }
                 if(strcmp(words[i],"-")==0)
                {
                    pathflag=1;
                    minusflag=1;
                }
                
            }
            else
            {
                pathflag=1;
            }
        }
        
        if(pathflag==0)
        {
            

            if(lflag && aflag)
            {
                printf("total %d\n",sum2/2);
                for(int i=0;i<fcount2;i++)
                {
                    char *path2=(char *)malloc(strlen(path)+strlen(allfilesincludinghidden[i])+2);
                    strcpy(path2,path);
                    strcat(path2,"/");
                    strcat(path2,allfilesincludinghidden[i]);
                    reveall(path2);
                }
            }
            else if(lflag)
            {
                printf("total %d\n",sum1/2);
                for(int i=0;i<fcount;i++)
                {
                    char *path2=(char *)malloc(strlen(path)+strlen(allfiles[i])+2);
                    strcpy(path2,path);
                    strcat(path2,"/");
                    strcat(path2,allfiles[i]);
                    reveall(path2);
                }
            }
            else if(aflag)
            {
                for(int i=0;i<fcount2;i++)
                {
                    struct stat fileStat;
         
                    if(stat(allfilesincludinghidden[i], &fileStat) == 0) // Get the file status
                    {
                        if(S_ISDIR(fileStat.st_mode)) // Check if it's a directory
                        {
                            printf("%s%s%s\n", BLUE, allfilesincludinghidden[i], RESET);
                        }
                        else if (fileStat.st_mode & S_IXUSR) // Check if it's executable
                        {
                            printf("%s%s%s\n", GREEN, allfilesincludinghidden[i], RESET);
                        }
                        else
                        {
                            printf("%s\n", allfilesincludinghidden[i]);
                        }
                    }
                    else
                    {
                        printf("%s\n", allfilesincludinghidden[i]);
                    }
                   
                }
            }
            else
            {
                for(int i=0;i<fcount;i++)
                {
             
                    struct stat fileStat;
                    if (stat(allfiles[i], &fileStat) == 0) // Get the file status
                    {
                    if (S_ISDIR(fileStat.st_mode)) // Check if it's a directory
                    {
                        printf("%s%s%s\n", BLUE, allfiles[i], RESET);
                    }
                    else if (fileStat.st_mode & S_IXUSR) // Check if it's executable
                    {
                        printf("%s%s%s\n", GREEN, allfiles[i], RESET);
                    }
                    else
                    {
                        printf("%s\n", allfiles[i]);
                    }
                    }
                    else
                    {
                        printf("%s\n", allfiles[i]);
                    }
                }


            }
        }
        else
        {

            char *path2=(char *)malloc(maxlen);
            if(minusflag==1)
            {
                if(strcmp("noprev",prevdirectory)==0)
                {
                    printf("%sNo previously saved directory%s\n",RED,RESET);

                    return ;
                }
                strcpy(path2,prevdirectory);
                
            }
            else
            {
                strcpy(path2,words[wordcount-1]);
                if(path2[0]=='~')
                {
                    char *temp=(char *)malloc(maxlen);
                    strcpy(temp,home);
                    strcat(temp,path2+1);
                    strcpy(path2,temp);
                }
            }
            *filecount=0;
            *filecount2=0;
            // if path2 is a file then print the details of the file
            struct stat fileStat;
            if (stat(path2, &fileStat) == 0) // Get the file status
            {
                if (S_ISDIR(fileStat.st_mode)==0) // Check if it's not a directory
                {
                    if(lflag)
                    {
                        
                        reveall(path2);
                    }
                    else
                    {
                        printf("%s\n", path2);
                        

                    }
                    return ;
                }
               
            }
             sum1=0;
            sum2=0;
            
            filesandallfiles(allfiles,allfilesincludinghidden,path2,filecount,filecount2);
            fcount=*filecount;
            fcount2=*filecount2;
            if(lflag && aflag)
            {
                printf("total %d\n",sum2/2);
                for(int i=0;i<fcount2;i++)
                {
                    char *path3=(char *)malloc(strlen(path2)+strlen(allfilesincludinghidden[i])+2);
                    strcpy(path3,path2);
                    strcat(path3,"/");
                    strcat(path3,allfilesincludinghidden[i]);
                    reveall(path3);
                }
            }
            else if(lflag)
            {
                printf("total %d\n",sum1/2);
                for(int i=0;i<fcount;i++)
                {
                    char *path3=(char *)malloc(strlen(path2)+strlen(allfiles[i])+2);
                    strcpy(path3,path2);
                    strcat(path3,"/");
                    strcat(path3,allfiles[i]);
                    reveall(path3);
                }
            }
            else if(aflag)
            {
                for(int i=0;i<fcount2;i++)
                {
                 
                    struct stat fileStat;
                    if(stat(allfilesincludinghidden[i], &fileStat) == 0) // Get the file status
                    {
                        if(S_ISDIR(fileStat.st_mode)) // Check if it's a directory
                        {
                            printf("%s%s%s\n", BLUE, allfilesincludinghidden[i], RESET);
                        }
                        else if (fileStat.st_mode & S_IXUSR) // Check if it's executable
                        {
                            printf("%s%s%s\n", GREEN, allfilesincludinghidden[i], RESET);
                        }
                        else
                        {
                            printf("%s\n", allfilesincludinghidden[i]);
                        }
                    }
                    else
                    {
                        printf("%s\n", allfilesincludinghidden[i]);
                    }
                }
            }
            else
            {
                for(int i=0;i<fcount;i++)
                {
                    struct stat fileStat;
                    if(stat(allfiles[i], &fileStat) == 0) // Get the file status
                    {
                        if(S_ISDIR(fileStat.st_mode)) // Check if it's a directory
                        {
                            printf("%s%s%s\n", BLUE, allfiles[i], RESET);
                        }
                        else if (fileStat.st_mode & S_IXUSR) // Check if it's executable
                        {
                            printf("%s%s%s\n", GREEN, allfiles[i], RESET);
                        }
                        else
                        {
                            printf("%s\n", allfiles[i]);
                        }
                    }
                    else
                    {
                        printf("%s\n", allfiles[i]);
                    }
            
                }

            }
        }

    }




}


