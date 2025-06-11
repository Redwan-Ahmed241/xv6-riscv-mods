#include "kernel/types.h"
#include "kernel/riscv.h"
#include "user/user.h"


int main(int argc, char * argv[])
{
    printf("setting ticket for parent process\n");

    int number = atoi(argv[1]);
    int r = settickets(number);
    if(r < 0)
    {
        printf("setticket unsuccessful!\n");
    }else
    {
        printf("setticket successful!\n");
    }

    int val = fork();

    if(val == 0) printf("\nFork successful\n");
    else if (val < 0) printf("\nFork unsuccessful\n");

    while (1)
    {
        
    }

    return 0;
}