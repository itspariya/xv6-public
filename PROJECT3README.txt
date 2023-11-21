ENVIRONMENT:
- WINDOWS10
- Ubuntu
- xv6-public

make clean
make
make qemu

_____________________________________________________________________________________________________________________

------------------------------------------------   EXAMPLE FILES     ------------------------------------------------
_____________________________________________________________________________________________________________________

- testexample.txt
- uniqexample.txt
- headexample.txt
_____________________________________________________________________________________________________________________

------------------------------------------------   USER PROGRAMS     ------------------------------------------------
_____________________________________________________________________________________________________________________

-----------------------------------------------       ps.c           ------------------------------------------------

Utitlity program to view the list of processes currently running on the system.
It displays the system's process state, including process ID, state, start time, total execution time, and process name.

******  USAGE  ******
   ps <pid>
   ps 0

******  OUTPUT  ******
PID: Process ID
STATUS: Current state of the process.
START: The start time of the process.
TOTAL: The total execution time of the process.
NAME: The name of the process.
In tabular format.

   - Checks for a specific process ID, if one or 0 is given, it will print all current processes
   - The program uses the ps system call to retrieve details about the procesess
   - The processes information is displayed.

-----------------------------------------------       test.c           ------------------------------------------------

Utility for executing another user-space program from within itself and reports timing details.
Currently is able to execute uniq and head command.

******  USAGE  ******
   test <program> [args...]
   test head 7 example.txt
        uniq uniqexample.txt

   
  - Checks for correct command line arguments, it expects the program to execute along with its arguments.
  - The fork() system call is used to create a child process.
  - The creation time, end time, and total time variables were added to the kernel structure.
    The times are updated throughout fork(), wait(), and exit().
    The timing details are printed in kernel space from with the exit() function prior to exiting.
    - Once first process is completed, user can type a second command execution. 
      The steps above repetes and then the program exits. 

-----------------------------------------------       uniq.c           ------------------------------------------------

Implements a simplied version of uniq command.
Reads the content of the input file and prints out unique lines

******  USAGE  ******
  uniq <inputfile>
  uniq uniqexample.txt

  - The program first checks fir the input file argument. If not provided, it prints a usage message then exits.
  - It then prints the execution message.
  - The program will attempt to open the input file. If unsuccessful, it will print an error message and exit. 
  - The while loops through the to read the file. The defined MAX_STRING_LENGTH buffer reads and processes the lines. 
  - Strcmp() is used to compare previous lines to each new line. 
    If a line is considered unique, it will be printed using “printf” and stored as the new ‘previous line’. 
  - It loops though comparing until the end of the buffer is reached. 
  - Once the entire file is processed, the file is closed using close() and the program exits. 
  
-----------------------------------------------       head.c           ------------------------------------------------


Implements a simplified version of head command.
Prints the first N lines of the input file.

******  USAGE  ******
   head <-n number> <inputfile>
   head -n 7 headexample.txt

 - The program starts by checking the command line argument. It verifies that the -n flag is used.
   The -n flag converts the following argument into an integer, this is the number of lines to print.
   If the number is not specified, it will default to printing 14 lines.
 - An usage message will appear if arguments are incorrect or if lines are less than or equal to 0.
 - It then attempts to open the input file, if it is unable to, an error message appears. 
 - The loop reads the file by line and keeps count of the number of lines printed
 - When a new line character '\n' is encountered, the counter is incremented. 
 - The loop terminates its printed the specified number of lines or when the end of the file is 
   reached.
 - The file is closed and the program exits.


_____________________________________________________________________________________________________________________

------------------------------------------------   SYSTEM CALLS     ------------------------------------------------
_____________________________________________________________________________________________________________________

----------------------------------------------- sys_ps -----------------------------------------------

    Provides user programs with information about processes running on the system.

    - Retrieves PID and user space pointers as the input argument.
    - Calls the getprocinfo() function in proc.c to gather information about the specified PID or for all processes if PID is 0.
    - If a specific PID is provided, only that process's details are filled in the 'p' array.
    - If PID is 0, the 'p' array is filled with details from all processes.
    - The kernel space array 'p' is then copyout() to the user space array 'user_p', copying only the necessary bytes, 
      either for a single process or for all processes.

-----------------------------------------------      sys_time        ------------------------------------------------
***********************************************      NOT USED        ***********************************************

Implementation of the uniq command in kernel space
Recieves a string from user space, removes duplicate lines to prints out unique lines


  - Uses argptr to retrieve a string pointer from system call.
  - Prints the kernel space execution message.
  - It then copies the string from user space to kernel space using copyin() function.
    *** copyin() is implemented in vm.c as an inverse to the already implemented copyout() ***
    *** It checks if the user space addresses are valid and the copies data from user space***
  - The function processes the string by eliminating duplicate lines.
  - Once processing is completed, the processed string is printed in the kernel console.
  - copyout() is then used to copy the data back to user space.


-----------------------------------------------      sys_head        ------------------------------------------------
Implementation of the head command in kernel space
Recieves a string from user space, 

User utility program: khead.c

******  USAGE  ******
------
   khead <-n number> <inputfile>
   khead -n 7 headexample.txt

  - Retrieves a string pointer and an integer from system call.
  - Prints the kernel space execution message.
  - It then copies the string from user space to kernel space using copyin() function.
	*** copyin() is implemented in vm.c as an inverse to the already implemented copyout() ***
	*** It checks if the user space addresses are valid and the copies data from user space***
  - The function processes the string by extracting the number of lines provided.
  - Once processing is completed, the processed string is printed in the kernel console.
  - copyout() is then used to copy the data back to user space.


_____________________________________________________________________________________________________________________

----------------------------------------------   PROC.C EXTENSION     ----------------------------------------------
_____________________________________________________________________________________________________________________

----------------------------------------------- scheduler() ---------------------------------------------
    - As the scheduler loops over the process table lookingg for a process to run, it determines which scheduling algorithm to run.
    - If the current scheduler is FCFS, it choose to run the process with the oldest start tick.
    - If it is the priority scheduler, it chooses the process with the highest priority.
    - Once a process is selected, it will be executed and timing details are calculated for the current process.

-----------------------------------------------  procstate_to_str      ---------------------------------------------
Function to convert the enumerated process states into string.
  - Takes in state as the enumerate process statet to convert
  - Returns the string represenation of that state

----------------------------------------------- getprocinfo() ---------------------------------------------

Function to retrieve information about process(es) in the system's process table.
  
    - Acquires the process table lock and iterates over the process table.
    - If a specific PID is given, the proc_stat structure is filled only with details regarding that process. Once found, the function breaks out of the loop.
    - If PID is 0, the function fills the proc_stat structure for each process in the system. The 'p' pointer is incremented for each iteration.
    - The process state, which is an enumerated value, is converted to a string using the procstate_to_str function.
    - Once the desired processes' details are populated (or if the entire process table has been iterated), the function releases the process table lock.


-----------------------------------------------      fork()        ------------------------------------------------
  - added a line that logs a failure to allocate process
  - Creation time process is recorded here when a child process is forked.

-----------------------------------------------      wait()        ------------------------------------------------
  - Calculates the total running time and waiting time of the child process.
  - Added a line to handle cases where the process does not have any children
  - Created a lastwaited pointer to store the last waited process. 

-----------------------------------------------      end()        ------------------------------------------------
  - The end and total running time of the process is recorded here. 
  - The time log for creation, end, and total time is printed here in kernel space.
_____________________________________________________________________________________________________________________

--------------------------------------------------   RESOURCES     --------------------------------------------------
_____________________________________________________________________________________________________________________

-----------------------------------------------   Jake Andrews    ----------------------------------------------
  - For the first part of the project, I had orginally had a sys_time system call that prints the creation, end, and total time of the child process. 
    When running the test program, the child process would exit prior to calling the system call. 
    He suggestest to print the times out in the wait() function prior to exiting.
    Sys_time above is the system call I did not end up using. 

  
-----------------------------------------------   Hitesh Vaidya   ------------------------------------------------

   - I was trying out different Implementations to log the timing details of each process and keep running into incorrect times. 
     I had attempted to increment the ticks in trap.c but decided on a different Implementation. He made me realize that I was still doing both. 

---------------------------------------------   Fhaheem Tadamarry  ----------------------------------------------
  - He gave me the idea to use utility functions in user space to call my system calls for uniq and head.
    I orginally had my user programs call their respective system calls. 
