#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "pstat.h"

int
sys_settickets(void)
{
    int tickets;
    
    // Get the number of tickets from user space
    argint(0, &tickets);
    
    if(tickets < 1) {
        tickets = 1; 
    }

    struct proc *p = myproc();
    acquire(&p->lock);
    p->tickets_original = tickets;
    p->tickets_current = tickets;
    release(&p->lock);
    
    return 0;
}

int
sys_getpinfo(void)
{
    uint64 addr;
    struct pstat ps;
    
    argaddr(0, &addr);
    
    if(getpinfo(&ps) < 0)
        return -1;

    if(copyout(myproc()->pagetable, addr, (char*)&ps, sizeof(ps)) < 0)
        return -1;
    
    return 0;
}

