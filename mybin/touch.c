#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("touch: missing file operand\n");
	exit(-1);
    }

    int  i;
    for(i=1; i<argc; ++i)
    {
         int res = open(argv[i], O_RDWR | O_CREAT, 0664);
	 if(res == -1)
	 {
	     printf("open error\n");
	 }
    }

    for(i=0; i<argc; ++i)
    {
        close(argv[i]);
    }
    return 0;
}
