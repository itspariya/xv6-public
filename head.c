#include "types.h"
#include "user.h"
#include "fcntl.h"

char line[512];

int main(int argc, char *argv[]) {
    int num_lines = 14; // Default number of lines

    if (argc == 4 && strcmp(argv[1], "-n") == 0) {
        num_lines = atoi(argv[2]);

        if (num_lines <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[2]);
            exit();
        }
    } else if (argc != 3) {
        printf(2, "Usage: %s [-n <number>] <input_file>\n", argv[0]);
        exit();
    }

    printf(1, "Head command is getting executed in user mode.\n");

    int fd = open(argv[argc - 1], O_RDONLY);
    if (fd < 0) {
        printf(2, "Failed to open %s.\n", argv[argc - 1]);
        exit();
    }

    int bytesRead;
    int linesPrinted = 0;

    while ((bytesRead = read(fd, line, sizeof(line))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (line[i] == '\n') {
                linesPrinted++;
            }

            if (linesPrinted >= num_lines) {
                break;
            }

            printf(1, "%c", line[i]);  // Print characters to console
        }

        if (linesPrinted >= num_lines) {
            break;
        }
    }

    close(fd);
    exit();
}

