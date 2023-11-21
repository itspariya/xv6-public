#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

#define MAX_STRING_LENGTH 512

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Usage: %s <input_file>\n", argv[0]);
        exit();
    }

    // Print the message
    printf(1, "Uniq command is getting executed in user mode.\n");


    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf(2, "Failed to open %s.\n", argv[1]);
        exit();
    }

    char buffer[MAX_STRING_LENGTH];
    int bytesRead;
    char prev_line[MAX_STRING_LENGTH] = "";

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        char *line = buffer;
        char *end = buffer + bytesRead;

        while (line < end) {
            char *newline = line;
            while (newline < end && *newline != '\n') {
                newline++;
            }

            *newline = '\0'; // Null terminate the line

	    /* int ret = syscall(SYS_uniq, line); // attempt to call uniq system call
            if (ret < 0) {
                printf(2, "Error: syscall failed\n");
                close(fd);
                exit();
            } */

            if (strcmp(line, prev_line) != 0) {
                printf(1, "%s\n", line);  // Print unique lines to console
                strcpy(prev_line, line);
            }

            line = newline + 1; // Move to the next line
        }
    }

    close(fd);
    exit();
} 
