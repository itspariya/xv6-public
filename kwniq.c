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

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf(2, "Failed to open %s.\n", argv[1]);
        exit();
    }

    char buffer[MAX_STRING_LENGTH];
    int bytesRead;
    char inputString[MAX_STRING_LENGTH] = ""; // Initialize an empty string

    // Read the file and append each line to the input string
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        int i;
        for (i = 0; i < bytesRead; i++) {
            inputString[i] = buffer[i];
        }
    }

    close(fd);

    // Make the sys_uniq system call with the input string
    if (syscall(SYS_uniq, inputString) < 0) {
        printf(2, "sys_uniq system call failed.\n");
        exit();
    }

    exit();
}

