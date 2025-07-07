#include <stddef.h>
#include <sys/mman.h>
#include <fcntl.h>
int main() {
  int fd = open("arquivo.txt", O_RDWR);
  char *map = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  map[0] = 'X'; // Modifica o arquivo via memória!
  munmap(map, 4096); close(fd);
}
