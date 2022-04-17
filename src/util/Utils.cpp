#include "../../inc/util/Utils.hpp"
#include <fcntl.h>
#include <unistd.h>

void randombytes(char* buffer, int start, int length) {
    int fd = open("/dev/urandom", O_RDONLY);
    ssize_t r = read(fd, buffer + start, length);
    close(fd);
}