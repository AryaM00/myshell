#ifndef ALIAS_H
#define ALIAS_H
void alias(char *command);
typedef struct Alias
{
    char*alias;
    char*originalcommand;
    int valid;
} Alias;
void mapofaliases(Alias *alias_list);
int inalias(Alias * head,char *alias);
void getoriginalcommand(Alias *alias_list,char *alias,char *originalcommand);
#endif
