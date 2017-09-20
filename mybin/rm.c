#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("rm: missing operand\n");
	exit(-1);
    }

    int i;
    for(i=1; i<argc; i++)
    {
        int res = unlink(argv[i]);
	if(res == -1)
	{
	    printf("rm error :%s\n", argv[i]);
	}
    }

    return 0;
}
