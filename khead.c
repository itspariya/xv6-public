#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_STRING_LENGTH 1024

int
main(int argc, char *argv[])
{
    char data[MAX_STRING_LENGTH];
    int fd;
    int num_lines = 10; // Default number of lines

    // Handle command line arguments
    if (argc == 4 && strcmp(argv[1], "-n") == 0) {
        num_lines = atoi(argv[2]);
        if (num_lines <= 0) {
            printf(1, "Invalid number of lines: %s\n", argv[2]);
            exit();
        }
    } else if (argc != 2) {
        printf(1, "Usage: khead [-n <number>] <input_file>\n");
        exit();
    }

    char *filename = (argc == 4) ? argv[3] : argv[1];

    if((fd = open(filename, O_RDONLY)) < 0){
        printf(1, "Failed to open file %s\n", filename);
        exit();
    }

    int bytesRead = read(fd, data, sizeof(data) - 1);
    if (bytesRead < 0) {
        printf(1, "Error reading file %s\n", filename);
        close(fd);
        exit();
    }
    data[bytesRead] = '\0';  // null terminate the read data

    close(fd);

    // Call the head function
    if(head(data, num_lines) < 0){
        printf(1, "head syscall failed\n");
        exit();
    }

    exit();
}

