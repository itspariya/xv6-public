#include "types.h"
#include "user.h"
#include "fcntl.h"


// Function to execute a CPU-bound process
void cpu_bound_process() {
  int i, j;
  for (i = 0; i < 100; i++) {
    for (j = 0; j < 1000000; j++) {
      // busy-wait to consume CPU time
    }
  }
  exit(); // terminate current process
}

// Function to execute an I/O-bound process
void io_bound_process() {
  char *argv[] = { "head", "somefile", 0 };
  exec("head", argv);
  exit(); // if exec fails
}

// Main function where the test scenario begins
int main(void) {
  int pid;
  int start_time, end_time, total_time; // variables for statistics

  // Start a CPU-bound process
  pid = fork();
  if (pid == 0) {
    cpu_bound_process();
  }

  // Start an I/O-bound process
  pid = fork();
  if (pid == 0) {
    io_bound_process();
  }

  // Wait for the processes to complete and collect statistics
  for (int i = 0; i < 2; i++) { // waiting for 2 child processes
    wait();

    // Call the sys_time system call to get the timing details of the completed process
    if (sys_gettime(&start_time, &end_time, &total_time) < 0) {
      // Successfully retrieved the timing information
      printf(1, "Process %d -> Start: %d, End: %d, Total: %d\n", 
             pid, start_time, end_time, total_time);
    } else {
      printf(2, "Error retrieving timing information for process %d\n", pid);
    }
  }

  exit(); // end of the main process
}
