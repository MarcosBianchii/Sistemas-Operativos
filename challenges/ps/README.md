# ps

*ps* (proccess status) es un comando unix que permite obtener todo tipo de información acerca de los procesos que están corriendo actualmente, disponiendo de muchos flags que alteran la cantidad de información a mostrar. Ver `ps(1)` para la lista completa de flags.

Toda esta información se obtiene del pseudo-filesystem /proc, que mantiene acceso de sólo lectura a muchas estructuras de control del kernel relacionadas con procesos. En particular, los datos de cada proceso se encuentran en el subdirectorio `/proc/[pid]`, siendo pid el proccess ID del proceso.

Dentro de `/proc/[pid]` hay información exhaustiva sobre cada proceso. Para este ejercicio nos interesa en particular `/proc/[pid]/comm`, que guarda el nombre del programa que se usó para lanzar el proceso. Para tener una descripción exacta de qué guarda cada archivo en /proc y cómo está codificado, referirse a `proc(5)`.

La implementación de ps0 (mucho más humilde), sólo listará para cada proceso su pid y el nombre del binario ejecutable que está corriendo. Para lograrlo hay que recorrer el directorio /proc y recaudar la información importante.

La salida de ps0 equivale a ejecutar `ps -eo pid,comm`, que lista en dos columnas el process id y el comando de todos los procesos. Un ejemplo de esta salida sería:

```
$ ps -eo pid,comm
    1 systemd
    2 kthreadd
    3 ksoftirqd/0
    5 kworker/0:0H
    7 rcu_sched
    8 rcu_bh
[...]
 7531 bash
 7625 kworker/1:0
 8046 ps
```

Implementar ps0 que debe mostrar la misma información que `ps -eo pid,comm`.

Syscalls recomendadas: `opendir(3)`, `readdir(3)`

Ayudas: `proc(5)`, `isdigit(3)`, para corroborar que se esté accediendo al directorio de un proceso y no a algún otro archivo de `/proc`.