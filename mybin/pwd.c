#include<stdio.h>
#include<stdlib.h>

int main()
{
    char buff[1024] = {0};
    getcwd(buff, 1023);
    printf("%s\n",buff);

    return 0;
}
