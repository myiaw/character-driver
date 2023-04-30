#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int fd = open("/dev/xpo_gonilnik_kodiranje", O_RDONLY);
    if (fd < 0) {
        perror("Error opening device file");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
        perror("Error reading from device file");
        close(fd);
        return 1;
    }

    buffer[bytesRead] = '\0';
    printf("Read data: %s\n", buffer);

    close(fd);
    return 0;
}