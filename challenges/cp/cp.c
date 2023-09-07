#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Uso: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src == -1) {
        printf("El archivo src: <%s> no existe\n", argv[1]);
        return 1;
    }

    // Conseguir el largo del file.
    off_t filesize = lseek(src, 0, SEEK_END);
    if (filesize == -1) {
        perror("No se pudo conseguir el largo de src");
        close(src);
        return 1;
    }

    // Abrir el mapping del file src.
    void *srcmapp = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, src, 0);
    if (srcmapp == MAP_FAILED) {
        close(src);
        return 1;
    }

    // Crear dst solo si no existe.
    int dst = open(argv[2], O_RDONLY);
    if (dst == -1) {
        dst = open(argv[2], O_RDWR | O_CREAT, 0666);
        if (dst == -1) {
            printf("El archivo dst: <%s> no se pudo crear\n", argv[2]);
            munmap(srcmapp, filesize);
            close(src);
            return 1;
        }

    } else {
        printf("El archivo dst: <%s> ya existe\n", argv[2]);
        munmap(srcmapp, filesize);
        close(src);
        close(dst);
        return 1;
    }

    // Truncar el file dst.
    if (ftruncate(dst, filesize) == -1) {
        perror("No se pudo truncar dst");
        munmap(srcmapp, filesize);
        close(src);
        close(dst);
        return 1;
    }

    // Abrir el mapping del file dst.
    void *dstmapp = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dst, 0);
    if (dstmapp == MAP_FAILED) {
        munmap(srcmapp, filesize);
        close(src);
        close(dst);
        return 1;
    }

    // Copiar el archivo.
    memcpy(dstmapp, srcmapp, filesize);

    munmap(srcmapp, filesize);
    munmap(dstmapp, filesize);
    close(src);
    close(dst);
    return 0;
}
