#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(2, "Usage: test <program> [args...]\n");
        exit();
    }

    int pid;

    // Call the provided program with its arguments
    pid = fork();
    if (pid == 0) {  // Child process
        printf(1, "Trying to execute %s\n", argv[1]);
        exec(argv[1], &argv[1]);  // Executing the program along with its args
        printf(2, "exec for %s failed\n", argv[1]);
        exit();
    } else if (pid > 0) {
        wait();
    } else {
        printf(2, "fork failed\n");
        exit();
    }

   exit();
}

