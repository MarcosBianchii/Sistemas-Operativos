// Esta macro es para que se
// definan otras macros.
#define _BSD_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <features.h>
#include <dirent.h>
// Esta macro es para que se
// defina strcasestr.
#define __USE_GNU
#include <string.h>

typedef char *(*substr_t)(const char *en, const char *esto);

int
dir_iter(char *substr, substr_t func, DIR *cur_dir, char *path)
{
    struct dirent *de = NULL;
    // Iterar sobre el dir actual.
    while (de = readdir(cur_dir)) {
        char *nombre = de->d_name;
        unsigned int tipo = de->d_type;

        if (tipo == DT_DIR) {
            // Saltear el dir "." y "..".
            if (strcmp(nombre, ".") == 0 || strcmp(nombre, "..") == 0)
                continue;

            // Abrir el subdir y correr la función en el.
            int subdirfd = openat(dirfd(cur_dir), nombre, O_DIRECTORY);
            if (subdirfd > 0) {
                // Ampliar el path.
                char nuevo_path[PATH_MAX];
                snprintf(nuevo_path, sizeof(nuevo_path), "%s/%s", path, nombre);

                // Abrir el dir.
                DIR *subdir = fdopendir(subdirfd);
                dir_iter(substr, func, subdir, nuevo_path);
                close(subdirfd);

            } else fprintf(stderr, "Error abriendo: %s/%s", path, nombre);
        }

        // Si no es un dir fijarse
        // si existe el substr en su nombre.
        else if (func(nombre, substr))
            printf("%s/%s\n", path, nombre);
    }

    return closedir(cur_dir);
}

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s [-i] <cadena>\n", argv[0]);
        return 1;
    }

    DIR *dir = opendir(".");
    if (!dir) {
        printf("No se pudo abrir el directorio actual");
        return 1;
    }

    substr_t func = !strcmp(argv[1], "-i") ? strcasestr : strstr;
    return dir_iter(argv[argc - 1], func, dir, "");
}