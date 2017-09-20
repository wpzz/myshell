#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("rmdir: missing operand\n");
	exit(-1);
    }

    int i;
    for(i=1;i<argc;++i)
    {
        int res = rmdir(argv[i]);

	if(res == -1)
	{
	    printf("rmdir fail: %s\n", argv[i]);
	}
    }
    return 0;
}

