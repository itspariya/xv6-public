#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"


int
main(int argc, char *argv[])
{
    struct proc_stat p[NPROC];
    int pid = 0;

    if(argc == 2) {
        pid = atoi(argv[1]);  // specific pid provided
    }

    if(ps(pid, p) < 0) {
        printf(2, "ps failed\n");
        exit();
    }

    printf(1, "PID\tSTATUS\t\tSTART\tTOTAL\tNAME\n");
    for(int i = 0; i < NPROC && p[i].pid != 0; i++) {
        printf(1, "%d\t%s \t%d\t%d\t%s\n", p[i].pid, p[i].state, p[i].start_time, p[i].total_time, p[i].name);
    }

    exit();
}

