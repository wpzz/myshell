#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("mkdir: missing operand\n");
	exit(-1);
    }

    int i;
    for(i=1;i<argc;++i)
    {
        int res = mkdir(argv[i], 0775);

	if(res == -1)
	{
	    printf("mkdir fail: %s\n", argv[i]);
	}
    }
    return 0;
}

