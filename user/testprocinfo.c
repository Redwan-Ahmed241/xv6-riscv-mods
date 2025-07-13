#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    struct pstat psinfo;
    int status = getpinfo(&psinfo);

    if (status != 0)
    {
        fprintf(2, "Error: Failed to get process info\n");
        exit(1);
    }

    printf("\n%-5s %-7s %-19s %-19s %-12s\n", "PID", "InUse", "Original Tickets", "Current Tickets", "Time Slices");

    int index = 0;
    while (index < NPROC)
    {
        if (psinfo.inuse[index])
        {
            int pid = psinfo.pid[index];
            int in = psinfo.inuse[index];
            int ori = psinfo.tickets_original[index];
            int cur = psinfo.tickets_current[index];
            int ts = psinfo.time_slices[index];

            printf("%-5d %-7d %-19d %-19d %-12d\n", pid, in, ori, cur, ts);
        }
        index++;
    }

    exit(0);
}
