typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;

#define STRSIZE 16 // maximum size for the string state

// STRUCTURE FOR proc_stat
struct proc_stat {
    int pid;                // Process ID
    char state[STRSIZE];    // Process state as a string
    int start_time;         // Process start time
    int total_time;         // Total time the process has been running
    char name[16];          // Process name
};
