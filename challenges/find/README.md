# find

Se pide escribir una versión muy simplificada de la utilidad `find(1)`. Esta herramienta, tal y como se la encuentra en sistemas GNU/Linux, acepta una miríada de opciones (ver su [página de manual](https://dashdash.io/1/find), o un [resumen gráfico](https://twitter.com/b0rk/status/993862211964735488/photo/1)). No obstante, en este lab se implementará sólo una de ellas.

La sinopsis de nuestra implementación será:

```sh
$ ./find [-i] <cadena>
```

Invocado como `./find xyz`, el programa buscará y mostrará por pantalla todos los archivos del directorio actual (y subdirectorios) cuyo nombre contenga (o sea igual a) `xyz`. Si se invoca como `./find -i xyz`, se realizará la misma búsqueda, pero sin distinguir entre mayúsculas y minúsculas.

Por ejemplo, si en el directorio actual se tiene:

```
.
├── Makefile
├── find.c
├── xargs.c
├── antiguo
│   ├── find.c
│   ├── xargs.c
│   ├── pingpong.c
│   ├── basurarghh
│   │   ├── find0.c
│   │   ├── find1.c
│   │   ├── pongg.c
│   │   └── findddddddd.c
│   ├── planes.txt
│   └── pingpong2.c
├── antinoo.jpg
└── GNUmakefile
```

el resultado de las distintas invocaciones debe ser como sigue (no importa el orden en que se impriman los archivos de un mismo directorio):


```sh
$ ./find Makefile
Makefile
GNUmakefile

$ ./find Makefile
Makefile

$ ./find -i Makefile
Makefile
GNUmakefile

$ ./find arg
xargs.c
antiguo/xargs.c
antiguo/basurarghh

$ ./find pong
antiguo/pingpong.c
antiguo/basurarghh/pongg.c
antiguo/pingpong2.c

$ ./find an
antiguo
antiguo/planes.txt
antinoo.jpg

$ ./find d.c
find.c
antiguo/find.c
antiguo/basurarghh/findddddddd.c
```

#### Ayuda:
* Usar recursividad para descender a los distintos directorios.

* Nunca descender los directorios especiales `.` y `..` (ambos son un “alias”; el primero al directorio actual, el segundo a su directorio inmediatamente superior).

* No es necesario preocuparse por ciclos en enlaces simbólicos.

* En el resultado de `readdir(3)`, asumir que el campo d_type siempre está presente, y es válido.

* La implementación case-sensitive vs. case-insensitive (opción `-i`) se puede resolver limpiamente usando un puntero a función como abstracción. (Ver [strstr(3)](https://dashdash.io/3/strstr).)

#### Requisitos:

* Llamar a la función `opendir(3)` una sola vez, al principio del programa (con argumento "."; no es necesario conseguir el nombre del directorio actual, si tenemos su alias).

* Para abrir sub-directorios, usar exclusivamente la función `openat(2)` (con el flag `O_DIRECTORY` como precaución). De esta manera, no es necesario realizar concatenación de cadenas para abrir subdirectorios.

  * Sí será necesario, no obstante, concatenar cadenas para mostrar por pantalla los resultados. No es necesario usar memoria dinámica; es suficiente un único buffer estático de longitud `PATH_MAX` (ver header limits.h).

  * Funciones que resultarán útiles como complemento a `openat()`: `dirfd(3)`, `fdopendir(3)`.

Llamadas al sistema: `openat(2)`, `readdir(3)`.