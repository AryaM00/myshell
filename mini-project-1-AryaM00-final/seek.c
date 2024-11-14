#include "seek.h"
char *directory[1000];
int directory_count=0;
char *foundfiles[1000];
int foundfiles_count=0;
char *filedirectory[1000];
int filedirectory_count=0;
char target[4096];


#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"

int directory_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    int len=strlen(target);
    char in[4096];
    if (tflag == FTW_D) { // Check if it's a directory and only in the current 
        strcpy(in,fpath+(ftwbuf->base));
        if(strncmp(in,target,len)==0)
        {
        directory[directory_count]=strdup(fpath);
        directory_count++;
        if(directory_count==1000)
            return 0;
        }
    }
    return 0; // Continue the tree walk
}
int file_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    int len=strlen(target);
    char in[4096];

    if (tflag == FTW_F ) { // Check if it's a file and only in the current level
        strcpy(in,fpath+(ftwbuf->base));
        if(strncmp(in,target,len)==0)
        {        
            foundfiles[foundfiles_count]=strdup(fpath); // Store the path of the found file
            foundfiles_count++;
            if(foundfiles_count==1000)
                return 0;
        }
        
    }
    return 0; // Continue the tree walk
}
int file_dir_search(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    // Check if it's a file or directory and only at the current directory level
    int len=strlen(target);
    char in[4096];
    
    if ((tflag == FTW_F || tflag == FTW_D)) {
        strcpy(in,fpath+(ftwbuf->base));

        if(strncmp(in,target,len)==0)
        {   
            filedirectory[filedirectory_count]=strdup(fpath);// Store the path of the found item
            filedirectory_count++;
            if(filedirectory_count==1000)
                return 0;
        }
    }

    return 0; // Continue the tree walk
}

void seek(char *command,char * home,char * prevdirectory)
{

    char *words[100];
    for(int i=0;i<100;i++)
    {
        words[i]=(char*)malloc(sizeof(char)*4096);
    }
    char *token;
    int it=0;
    char *temp=(char*)malloc(sizeof(char)*4096);
    strcpy(temp,command);
    while(token=strtok_r(command," \t\n",&command))
    {
        words[it]=token;
        it++;
    }
    int dflag=0;
    int fflag=0;
    int eflag=0;
    int pathflag=0;
    
    char *path=(char*)malloc(sizeof(char)*4096);
    int tempcount=0;
    for(int i=1;i<it;i++)
    {
        
        if(words[i][0]=='-')
        {
            if(strstr(words[i],"d")!=NULL)
            {
                dflag=1;
            }
            if(strstr(words[i],"f")!=NULL)
            {
                fflag=1;
            }
            if(strstr(words[i],"e")!=NULL)
            {
                eflag=1;
            }
            if(strcmp(words[i],"-")==0 && tempcount==1)
            {
                pathflag=1;
                path=strdup(words[i]);
                break;

                
            }
        }
        else if(tempcount==0)
        {

            strcpy(target,words[i]);
           tempcount++;
        }
        else
        {
            
            path=strdup(words[i]);
            pathflag=1;
            break;
        }
    }
    if(pathflag==0)
    {
        //search in current directory for tagrget file
        
        if(dflag==1)
        {
            //search for directories
            if(fflag==1)
            {
                //search for files
                printf("%sInvalid flags%s\n",RED,RESET);
            }
            else
            {
                if (nftw(".", directory_search, 20, FTW_PHYS) == -1) {
                    printf("%sError in nftw%s\n",RED,RESET);
                    return ;
                }
                for(int i=0;i<directory_count;i++)
                {
                    
                    printf("%s%s%s \n",BLUE,directory[i],RESET);
                    
                }
                if(directory_count==1 && eflag==1)
                {
                    char *cwd=(char*)malloc(4096);
                    getcwd(cwd,4096);
                    if(chdir(directory[0])!=0)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        strcpy(prevdirectory,cwd);

                    }
                }
                if(directory_count==0)
                {
                    printf("%sNo match found%s\n",RED,RESET);
                }
            }
        }
        else if(fflag==1)
        {
            //search for files
            if(dflag==1)
            {
                //search for directories
                printf("%sInvalid flags%s\n",RED,RESET);
            }
            else
            {
                if (nftw(".", file_search, 20, FTW_PHYS) == -1) {
                    printf("%sError in nftw%s\n",RED,RESET);
                    return ;
                }
                for(int i=0;i<foundfiles_count;i++)
                {
                    
                    printf("%s%s%s\n",GREEN,foundfiles[i],RESET);
                    
                }
                if(foundfiles_count==1 && eflag==1)
                {
                    FILE *fp=fopen(foundfiles[0],"r");
                    if(fp==NULL)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        // i need to print contents of the file
                        char ch;
                        while((ch=fgetc(fp))!=EOF)
                        {
                            printf("%c",ch);
                        }


                        
                       
                    }
                }
                if(foundfiles_count==0)
                {
                    printf("%sNo match found%s\n",RED,RESET);
                }
                
                
            }
        }
        else
        {
            //search for both files and directories
        
            if (nftw(".", file_dir_search, 20, FTW_PHYS) == -1) {
                printf("%sError in nftw%s\n",RED,RESET);
                return ;
            }

            for(int i=0;i<filedirectory_count;i++)
            {
                // if found item is a file then print it in green else print it in blue
                // check if current path is a file or directory
                struct stat statbuf;
                stat(filedirectory[i],&statbuf);
                if(S_ISDIR(statbuf.st_mode))
                {
                    printf("%s%s%s\n",BLUE,filedirectory[i],RESET);
                }
                else
                {
                    printf("%s%s%s\n",GREEN,filedirectory[i],RESET);
                }
                
                
                
            }
            if(filedirectory_count==1 && eflag==1)
            {
                // if only one item is found then check if it is a file or directory
                
                struct stat statbuf;
                stat(filedirectory[0],&statbuf);
                if(S_ISDIR(statbuf.st_mode))
                {
                    char *cwd=(char*)malloc(4096);
                    getcwd(cwd,4096);
                    if(chdir(filedirectory[0])!=0)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        
                        strcpy(prevdirectory,cwd);
                    }
                }
                else
                {
                    FILE *fp=fopen(filedirectory[0],"r");
                    if(fp==NULL)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        // i need to print contents of the file
                        char ch;
                        while((ch=fgetc(fp))!=EOF)
                        {
                            printf("%c",ch);
                        }
                    }
                }
            }
            if(filedirectory_count==0)
            {
                printf("%sNo match found%s\n",RED,RESET);
            }
        }
    }
    else
    {
        //search in the specified directory for target file

        if(path[0]=='~')
        {
            char *temp=(char *)malloc(4096);
            strcpy(temp,home);
            strcat(temp,path+1);
            path=strdup(temp);
         
        }
        if(path[0]=='-')
        {
            char *temp=(char *)malloc(4096);
            strcpy(temp,prevdirectory);
            strcat(temp,path+1);
            path=strdup(temp);

        }
    
        
        
        
        if(dflag==1)
        {
            //search for directories
            if(fflag==1)
            {
                //search for files
                printf("%sInvalid flags%s\n",RED,RESET);
            }
            else
            {
                if (nftw(path, directory_search, 20, FTW_PHYS) == -1) {
                    printf("%sError in nftw%s\n",RED,RESET);
                    return ;
                }
                for(int i=0;i<directory_count;i++)
                {
                        if (strncmp(directory[i], path, strlen(path)) == 0) {
                            printf("%s.%s%s\n",BLUE, directory[i] + strlen(path),RESET);
                        } else {
                            printf("%s%s%s\n",BLUE,directory[i],RESET);
                              // If start is not a prefix, print full path
                        }
                    
                  
                    
                }
                if(directory_count==1 && eflag==1)
                {
                    char cwd[4096];
                    getcwd(cwd,4096);
                    if(chdir(directory[0])!=0)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        strcpy(prevdirectory,cwd);
                    }
                }
                if(directory_count==0)
                {
                    printf("%sNo match found%s\n",RED,RESET);
                }
            }
        }
        else if(fflag==1)
        {
            //search for files
            if(dflag==1)
            {
                //search for directories
                printf("%sInvalid flags%s\n",RED,RESET);
            }
            else
            {
                if (nftw(path, file_search, 20, FTW_PHYS) == -1) {
                    printf("%sError in nftw%s\n",RED,RESET);
                    return ;
                }
                for(int i=0;i<foundfiles_count;i++)
                {
                        if (strncmp(foundfiles[i], path, strlen(path)) == 0) {
                        printf("%s.%s%s\n",GREEN, foundfiles[i] + strlen(path),RESET);
                        } else {
                             printf("%s%s%s\n",GREEN,foundfiles[i],RESET);
                             // If start is not a prefix, print full path
                        }
                    
                   
                    
                }
                if(foundfiles_count==1 && eflag==1)
                {
                    FILE *fp=fopen(foundfiles[0],"r");
                    if(fp==NULL)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        // i need to print contents of the file
                        char ch;
                        while((ch=fgetc(fp))!=EOF)
                        {
                            printf("%c",ch);
                        }
                    }
          

                }
                if(foundfiles_count==0)
                {
                    printf("%sNo match found%s\n",RED,RESET);
                }
            }
        }
        else
        {
            //search for both files and directories
        
            if (nftw(path, file_dir_search, 20, FTW_PHYS) == -1) {
                printf("%sError in nftw%s\n",RED,RESET);
                return ;
            }
            // printf("%s%s%s\n",BLUE,filedirectory[0],RESET);
            

            for(int i=0;i<filedirectory_count;i++)
            {
                // if found item is a file then print it in green else print it in blue
                // check if current path is a file or directory
                struct stat statbuf;
                stat(filedirectory[i],&statbuf);
                if(S_ISDIR(statbuf.st_mode))
                {
                   
                        if (strncmp(filedirectory[i], path, strlen(path)) == 0) {
                            printf("%s.%s%s\n",BLUE, filedirectory[i] + strlen(path),RESET);
                        } else {
                            printf("%s%s%s\n",BLUE,filedirectory[i],RESET);
                              // If start is not a prefix, print full path
                        }
                }
                else
                {

                        if (strncmp(filedirectory[i], path, strlen(path)) == 0) {
                        printf("%s.%s%s\n",GREEN, filedirectory[i] + strlen(path),RESET);
                        } else {
                             printf("%s%s%s\n",GREEN,filedirectory[i],RESET);
                             // If start is not a prefix, print full path
                        }
                   
                }
                
                
                
            }
            if(filedirectory_count==1 && eflag==1)
            {
                 
                // if only one item is found then check if it is a file or directory
                
                struct stat statbuf;
                
                stat(filedirectory[0],&statbuf);
                if(S_ISDIR(statbuf.st_mode))
                {
                    char cwd[4096];
                    getcwd(cwd,4096);
                   
                    if(chdir(filedirectory[0])!=0)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        strcpy(prevdirectory,cwd);
                    }
                }
                else
                {
                    FILE *fp=fopen(filedirectory[0],"r");
                    if(fp==NULL)
                    {
                        printf("%sMissing permissions for task!%s\n",RED,RESET);
                    }
                    else
                    {
                        // i need to print contents of the file
                        char ch;
                        while((ch=fgetc(fp))!=EOF)
                        {
                            printf("%c",ch);
                        }
                    }
                }
            }
            if(filedirectory_count==0)
            {
                printf("%sNo match found%s\n",RED,RESET);
            }
        }

        
    }
     directory_count=0;
     foundfiles_count=0;
     filedirectory_count=0;
}

