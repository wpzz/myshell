#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#include<unistd.h>
#include<termios.h>

char *RCmd[128] = {0};
int count = 0;
char hostname[128] = {0};
char user[128] = {0};
char curdir[128] = {0};
char oldpwd[128] = {0};

enum Cmdtype
{
    CD,
    TOUCH,
    RM,
    MKDIR,
    RMDIR,
    CP,
    LS,
    DEFAULT,
    EXIT,
    ENPTY,
    ERROR,
};

void Markedwords()
{
    int uid = getuid();
    struct passwd *pwd;
    pwd = getpwuid(uid);
    strcpy(user, pwd->pw_name);
    
    char flag = '$';
    if(strcmp(user, "root") == 0)
    {
       flag = '#';
    }

    gethostname(hostname, 127);
    char *shostname = strtok(hostname, "."); 

    char buff[128] = {0};
    getcwd(buff, 127);
    char *p = strtok(buff, "/");
    while(p != NULL)
    {
       memset(curdir, 0, 128);
       strcpy(curdir, p);
       p = strtok(NULL, "/");
    }   
    if(curdir[0] == 0)
    {
       curdir[0] = '/';
    }
    if(strcmp(curdir, user) == 0)
    {
       strcpy(curdir, "~\0");
    }

    printf("REALLY[%s@%s %s]%c ",user,shostname,curdir,flag);
}

void ResolveCmd(char *cmd)
{
    char *p = strtok(cmd, " ");
    
    while(p != NULL)
    {
        RCmd[count++] = p;
        p = strtok(NULL, " ");
    }
    
}

int AnalyzeCmd()
{
    if(strcmp(RCmd[0], "cd") == 0)
    {
        return CD;
    }
    if(strcmp(RCmd[0], "exit") == 0)
    {
        return EXIT;
    }
    if(strcmp(RCmd[0],"pwd") == 0 \
    || strcmp(RCmd[0], "ls") == 0 \
    || strcmp(RCmd[0],"touch") == 0 \
    || strcmp(RCmd[0],"rm") == 0 \
    || strcmp(RCmd[0],"mkdir") == 0 \
    || strcmp(RCmd[0],"rmdir") == 0 \
    || strcmp(RCmd[0],"cp") == 0)
    {
        return DEFAULT;
    }
    if(strcmp(RCmd[0], "\n") == 0)
    {
        return ENPTY;
    }

    return ERROR;
}

void ClearCmd()
{
    int i = 0;
    char ch;

    for(i=0;i<count;i++)
    {
       RCmd[count] = NULL;
    }

    count = 0;
}

void myfork()
{
    if(fork() == 0)
    {
        char buff[128] = "/home/wpzz/Desktop/myshell/mybin/";

        strcat(buff, RCmd[0]);
        RCmd[0] = buff;
        execv(buff, RCmd);
        printf("exec fail\n");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void mycd()
{
   char buff[128] = {0};
   getcwd(buff, 127);
   
   if(RCmd[1] == NULL || strcmp(RCmd[1], "~") == 0)
   { 
      if(strcmp(user, "root") == 0)
      {
         chdir("/root");
      }
      else
      {
         char temp[128] = "/home/";
         strcat(temp, user);
         chdir(temp);
      }
   }

   else if(strcmp(RCmd[1], "-") == 0)
   {
      if(oldpwd[0] == 0)
      {
         printf("OLDPWD not set\n");
         return ;
      }
      chdir(oldpwd);
   }
 
   else
   {
      if(chdir(RCmd[1]) == -1)
      {
         printf("bash: cd: %s: No such file or directory\n",RCmd[1]);
         return ;
      }
   }

   strcpy(oldpwd, buff);
}

void GetCmd(char* command)
{
/*   struct termios old_ter, new_ter;
    
   tcgetattr(0, &old_ter);
   new_ter = old_ter;
   new_ter.c_lflag &= ~ECHO;
   new_ter.c_lflag &= ~ICANON;

   tcsetattr(0, TCSANOW, &new_ter);*/

   char buff[128] = {0}; 
   int count = 0;
//   int flag = 0;
   while(1)
   {
      char c = getchar();
      if(c == 27)
      {
          c = getchar();
          if(c == 91)
          {
              switch(c = getchar())
              {
                 case'A': 
                   printf("up\n");
                   break;
                 case'B': 
                   printf("down\n");
                   break;
                 case'C': 
                   printf("right\n");
                   break;
                 case'D': 
                   printf("left\n");
                   break;
              }
        //      tcsetattr(0, TCSANOW, &old_ter);
	      getchar();
	      return;
          }
          
      }
      else
      {
          if(c == 10)
          {
	      if(count == 0)
	      {
	          strcpy(command, "\n");
	      }
	      else
	          strcpy(command, buff);
              break;
          }
          buff[count++] = c;
      }
      //tcsetattr(0, TCSANOW, &old_ter);
   }
}

int main(char argc, char* argv[], char* envp[])
{
   char command[128] = {0};
   /*
   int i = 0;
   for(i=0; envp[i] != NULL; ++i)
   {
       printf("%s\n",envp[i]);
   }
   */
   while(1)
   {
      Markedwords();
      GetCmd(command);
      ResolveCmd(command);      
      int res = AnalyzeCmd();
      switch(res)
      {
          case CD:
            mycd();
            break;
          case EXIT:
            exit(0);
            break;
          case DEFAULT:
            myfork();
            break;
	  case ENPTY:
	    break;
          default:
            printf("command input error\n");
      }
      ClearCmd();
      memset(command, 0, 128);
   }
   exit(0);
}
