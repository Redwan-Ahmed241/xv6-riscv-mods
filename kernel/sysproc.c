#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n = 0;
  if (argint(0, &n) >= 0)
  {
    exit(n);
  }
  return 0; // This line is never reached
}

uint64
sys_getpid(void)
{
  struct proc *p = myproc();
  int pid = p->pid;
  return pid;
}

uint64
sys_fork(void)
{
  uint64 result = fork();
  return result;
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  int status = wait(p);
  return status;
}

uint64
sys_sbrk(void)
{
  struct proc *p = myproc();
  uint64 base = p->sz;
  int n = 0;

  if (argint(0, &n) < 0)
    return -1;

  int growth = growproc(n);
  return (growth < 0) ? -1 : base;
}

uint64
sys_sleep(void)
{
  int n = 0;
  argint(0, &n);
  if (n < 0)
    n = 0;

  uint start_ticks = 0;
  acquire(&tickslock);
  start_ticks = ticks;

  for (;;)
  {
    if ((ticks - start_ticks) >= n)
      break;
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }

  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid = -1;
  if (argint(0, &pid) < 0)
    return -1;

  int res = kill(pid);
  return res;
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint current_ticks = 0;
  acquire(&tickslock);
  current_ticks = ticks;
  release(&tickslock);
  return current_ticks;
}

uint64
sys_settickets(void)
{
  int tickets = 1;
  if (argint(0, &tickets) < 0 || tickets < 1)
  {
    tickets = 1;
  }

  struct proc *p = myproc();
  acquire(&p->lock);
  p->tickets_original = tickets;
  p->tickets_current = tickets;
  release(&p->lock);

  return 0;
}

uint64
sys_getpinfo(void)
{
  struct pstat ps;
  uint64 addr = 0;
  argaddr(0, &addr);

  if (getpinfo(&ps) != 0)
    return -1;

  if (copyout(myproc()->pagetable, addr, (char *)&ps, sizeof(ps)) < 0)
    return -1;

  return 0;
}
