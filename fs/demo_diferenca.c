#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    // Usando função de biblioteca (stdio)
    printf("=== Usando Library Function (fopen/fwrite) ===\n");
    FILE *fp = fopen("arquivo_lib.txt", "w");
    if (fp) {
        fwrite("Hello from library function!\n", 1, 30, fp);
        fclose(fp);
    }
    
    // Usando system call diretamente
    printf("=== Usando System Call (open/write) ===\n");
    int fd = open("arquivo_syscall.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd != -1) {
        write(fd, "Hello from system call!\n", 24);
        close(fd);
    }
    
    return 0;
}
