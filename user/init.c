// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(void)
{
  char *args[] = {"sh", 0};
  int child_pid = -1;
  int status_pid = -1;

  // Ensure console device is open
  int fd = open("console", O_RDWR);
  if (fd < 0)
  {
    mknod("console", CONSOLE, 0);
    fd = open("console", O_RDWR);
  }

  // Redirect stdout and stderr
  for (int i = 0; i < 2; i++)
  {
    dup(0);
  }

  // Start shell loop
  while (1)
  {
    printf("init: starting sh\n");

    child_pid = fork();
    if (child_pid < 0)
    {
      fprintf(2, "init: fork failed\n");
      exit(1);
    }

    if (child_pid == 0)
    {
      exec("sh", args);
      fprintf(2, "init: exec sh failed\n");
      exit(1);
    }

    // Monitor child process
    while (1)
    {
      status_pid = wait(0);
      if (status_pid == child_pid)
      {
        // shell exited, respawn
        break;
      }
      else if (status_pid < 0)
      {
        fprintf(2, "init: wait error\n");
        exit(1);
      }
      // orphaned child, ignore
    }
  }
}
