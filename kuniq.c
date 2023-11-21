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

    if(argc != 2){
        printf(1, "Usage: kuniq <input_file>\n");
        exit();
    }

    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf(1, "Failed to open file %s\n", argv[1]);
        exit();
    }

    int bytesRead = read(fd, data, sizeof(data) - 1);
    if (bytesRead < 0) {
        printf(1, "Error reading file %s\n", argv[1]);
        close(fd);
        exit();
    }
    data[bytesRead] = '\0';  // null terminate the read data

    close(fd);

    // Call the uniq function
    if(uniq(data) < 0){
        printf(1, "uniq failed\n");
        exit();
    }

    // Display the result
    printf(1, "Result:\n%s\n", data);

    exit();
}

