#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<sys/stat.h>

void show_info(char *filename, struct stat *info_p);
void mode_str(int mode, char str[]);
char *uid_str(uid_t uid);
char *gid_str(gid_t gid);

void my_ls(char *pathname, int mode)
{
    DIR *dir = opendir(pathname);
    struct dirent *pdirent;
    char full_path[128] = {0};
    struct stat buff;

    if(dir == NULL)
    {
        printf("can not open this dir:%s\n", pathname);
	exit(-1);
    }

    if(mode == -1)
    {
        printf("error cmd\n");
	exit(-1);
    }
    if(mode == 0)
    {
        while((pdirent = readdir(dir)) != NULL)
	{
	    printf("%s  ", pdirent->d_name);
	}
	printf("\n");
    }
    else  if(mode == 1)
    {
        while((pdirent = readdir(dir)) != NULL)
	{
	    strcpy(full_path, pathname);
	    int dir_len = strlen(full_path);
	
	    if(full_path[dir_len-1] != '/')
	    {
	        full_path[dir_len] = '/';
		strcat(full_path, pdirent->d_name);
       	    }
	    else
	        strcat(full_path, pdirent->d_name);
	
            stat(full_path, &buff);
	    show_info(pdirent->d_name, &buff);
	    memset(full_path, 0, 128);
        }
    }
}

void show_info(char *filename, struct stat *info_p)
{
    char modestr[11];
    mode_str(info_p->st_mode, modestr);
    printf("%s", modestr);
    printf("%3d ", (int)info_p->st_nlink);
    printf("%-5s", uid_str(info_p->st_uid));
    printf("%-5s", gid_str(info_p->st_gid));
    printf("%4ld ", (long)info_p->st_size);
    printf("%.12s ", 4 + ctime(&info_p->st_mtime));
    printf("%s\n", filename);
 }


void mode_str(int mode, char str[]) 
{
     strcpy(str, "----------"); 
     if(S_ISDIR(mode)) str[0] = 'd';
     if(S_ISCHR(mode)) str[0] = 'c';
     if(S_ISBLK(mode)) str[0] = 'b'; 
     if(S_ISLNK(mode)) str[0] = 'l';

     if(mode & S_IRUSR) str[1] = 'r';
     if(mode & S_IWUSR) str[2] = 'w';
     if(mode & S_IXUSR) str[3] = 'x';

     if(mode & S_IRGRP) str[4] = 'r'; 
     if(mode & S_IWGRP) str[5] = 'w';
     if(mode & S_IXGRP) str[6] = 'x';

     if(mode & S_IROTH) str[7] = 'r';
     if(mode & S_IWOTH) str[8] = 'w';
     if(mode & S_IXOTH) str[9] = 'x';

}

char *uid_str(uid_t uid)
{
	static char numstr[10];
	struct passwd *pw_ptr;

	if((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char *gid_str(gid_t gid)
{
     static char numstr[10];
     struct group *grp_ptr;
     if((grp_ptr = getgrgid(gid)) == NULL)
     {	 
         sprintf(numstr, "%d", gid);
	 return numstr;
     }
     else
	 return grp_ptr->gr_name;

}


int main(int argc, char *argv[])
{
    int i;
    int mode = 0;
    if(argc == 1)
    {
        my_ls("./", mode);
	return 0;
    }

    if(strcmp(argv[1], "-l") == 0)
    {
        mode = 1;
    }
    else if(strcmp(argv[1], "-a") == 0)
    {
        mode = 2;
    }
    else
    {
        my_ls(argv[1], mode);
	return 0;
    }
    
    if(argc >2)
    {
        my_ls(argv[2], mode);
    }
    else
    {
        my_ls("./", mode);
    }

    return 0;
}

