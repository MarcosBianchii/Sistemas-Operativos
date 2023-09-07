// Esta macro es para que se
// definan otras macros.
#define _BSD_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>

bool
str_es_numero(char *str)
{
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

int
proc_iter(DIR *proc)
{
    struct dirent *de = NULL;
    // Iterar sobre el
    // directorio de procesos.
    while (de = readdir(proc)) {
        char *nombre = de->d_name;
        unsigned int tipo = de->d_type;
        // Filtrar para quedarme solo con
        // los directorios que son procesos.
        if (tipo == DT_DIR && str_es_numero(nombre)) {
            // Formatear el string para leer
            // el archivo comm del proceso.
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "/proc/%s/comm", nombre);
            int commfd = open(path, O_RDONLY);

            size_t len = 0;
            char binario[128], c;
            // Leer el nombre del binario.
            while (read(commfd, &c, sizeof(c)) > 0)
                binario[len++] = c;
            binario[len-1] = '\0';

            if (close(commfd) == -1) {
                closedir(proc);
                return 1;
            }

            // Calcular el padding
            // e imprimir por consola.
            int pad = 5 - strlen(nombre);
            printf("%*s%s %s\n", pad, "", nombre, binario);
        }
    }

    return closedir(proc);
}

int
main()
{
    DIR *proc = opendir("/proc");
    return proc_iter(proc);
}