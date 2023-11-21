#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"

int getprocinfo(int pid, struct proc_stat *p);
int sys_ps(void)
{
    int pid;
    struct proc_stat p[NPROC];
    struct proc_stat *user_p;
    struct proc *curr_proc = myproc();

    if(argint(0, &pid) < 0)
        return -1;

    if(argptr(1, (void*)&user_p, sizeof(struct proc_stat)*NPROC) < 0)
        return -1;

    if(getprocinfo(pid, p) < 0)
        return -1;

    int bytes_to_copy;
    if(pid == 0) {
        bytes_to_copy = sizeof(struct proc_stat) * NPROC;
    } else {
        bytes_to_copy = sizeof(struct proc_stat);  // only one process information
    }

    if(copyout(curr_proc->pgdir, (uint)user_p, p, bytes_to_copy) < 0)
        return -1;

    return 0;
}


int sys_gettime(int *start_time, int *end_time, int *total_time) {
    struct proc *curproc = myproc();

    if (!curproc) {
        return -1;  // Error: No current process
    }

    // Access timing information directly from the current process's structure
    *start_time = curproc->creation_time; 
    *end_time = curproc->end_time;
    *total_time = curproc->total_time;  // total execution time


    return 0; 
}





// Custom function to compare two strings up to n characters
int custom_strncmp(const char *s1, const char *s2, int n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0) {
        return 0;
    } else {
        return *(unsigned char *)s1 - *(unsigned char *)s2;
    }
}

// Custom function to copy memory
void custom_memcpy(char *dest, const char *src, int n) {
    while (n--) {
        *dest++ = *src++;
    }
}

int sys_uniq(void) {
    char *string;
    char kernel_buffer[MAX_STRING_LENGTH];
    char processed_buffer[MAX_STRING_LENGTH] = "";
    char prev_line[MAX_STRING_LENGTH] = "";
    int processed_len = 0;
    int prev_line_len = 0;
    char *line;
    char *end;
    char *newline;
    int line_len;

    // Use argptr to retrieve the string argument.
    if (argptr(0, &string, 1) < 0) {
        return -1;  // Failed to get the argument
    }

    // Display the initial message
    cprintf("Uniq command is getting executed in kernel mode\n");

    // Copy the user string to the kernel space
    if (copyin((pde_t *)myproc()->pgdir, (uint)string, kernel_buffer, MAX_STRING_LENGTH) < 0) {
        cprintf("Error: copyin failed\n");
        return -1;
    }

    line = kernel_buffer;
    end = kernel_buffer + strlen(kernel_buffer);

    if (!line) {
        cprintf("Error: kernel_buffer is null after copyin\n");
        return -1;
    }

    while (line < end) {
        newline = line;
        while (newline < end && *newline != '\n') {
            newline++;
        }

        // Calculate the length of the current line
        line_len = newline - line;

        if (line_len != prev_line_len || custom_strncmp(line, prev_line, line_len) != 0) {
            if (processed_len + line_len >= MAX_STRING_LENGTH) {
                cprintf("Error: processed_buffer would overflow\n");
                return -1;
            }

            strncpy(processed_buffer + processed_len, line, line_len);
            processed_len += line_len;
            processed_buffer[processed_len] = '\n'; 
            processed_len++;
            custom_memcpy(prev_line, line, line_len);
            prev_line_len = line_len;
        }

        line = newline + 1; 
    }

    processed_buffer[processed_len] = '\0';
    cprintf("Results:\n%s\n", processed_buffer);

    if (copyout((pde_t *)myproc()->pgdir, (uint)string, processed_buffer, strlen(processed_buffer) + 1) < 0) {
        cprintf("Error: copyout failed\n");
        return -1;
    }

    return 0;
}


int
sys_head(void) {
    char *string;
    int num_lines;
    char kernel_buffer[MAX_STRING_LENGTH];
    char processed_buffer[MAX_STRING_LENGTH] = "";
    int linesPrinted = 0;

    // Fetch the arguments from user space using argptr and argint
    if (argptr(0, &string, 1) < 0 || argint(1, &num_lines) < 0) {
        return -1;  // Failed to get the arguments
    }

    // Display the initial message
    cprintf("Head command is getting executed in kernel mode\n");

    // Copy the user string to the kernel space
    if (copyin((pde_t *)myproc()->pgdir, (uint)string, kernel_buffer, sizeof(kernel_buffer)) < 0) {
        cprintf("Error: copyin failed\n");
        return -1;
    }

    char *line = kernel_buffer;
    char *end = kernel_buffer + strlen(kernel_buffer);
    int processed_len = 0;

    while (line < end && linesPrinted < num_lines) {
        char *newline = line;
        while (newline < end && *newline != '\n') {
            newline++;
        }

        int line_len = newline - line + 1;  // +1 to include the newline character

        // Ensure we don't overflow processed_buffer
        if (processed_len + line_len >= sizeof(processed_buffer)) {
            cprintf("Error: processed_buffer would overflow\n");
            return -1;
        }

        memmove(processed_buffer + processed_len, line, line_len);  // Copy the line to processed_buffer
        processed_len += line_len;
        linesPrinted++;

        line = newline + 1;  // Move to the next line
    }

    processed_buffer[processed_len] = '\0';  // Null-terminate the processed string
    cprintf("Results:\n%s\n", processed_buffer);
    
// Copy the processed string back to user space
    if (copyout((pde_t *)myproc()->pgdir, (uint)string, processed_buffer, strlen(processed_buffer) + 1) < 0) {
        cprintf("Error: copyout failed\n");
        return -1;
    }

    return 0;
}


int
sys_fork(void)
{
  return fork();
}


int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

/*int
sys_wait(void)
{
  return wait();
}*/
int
sys_wait(void)
{
    int *wtime, *rtime;

    if(argptr(0, (void*)&wtime, sizeof(int)) < 0 || argptr(1, (void*)&rtime, sizeof(int)) < 0)
        return -1;

    return wait(wtime, rtime);
}


int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
