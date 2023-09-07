# xargs

En su versión más sencilla, la utilidad `xargs(1)` permite:

* Tomar un único argumento (argv[1], un sólo comando).
* Leer nombres de archivos de la entrada estándar (stdin), de a uno por línea.
* Para cada nombre de archivo leído, invocar al comando especificado con el nombre de archivo como argumento.

Por ejemplo, si se invoca:

```sh
$ echo /home | xargs ls
```

Esto sería equivalente a realizar `ls /home`. Pero si se invoca:

```sh
$ printf "/home\n/var\n" | xargs ls
```

Esto sería equivalente a ejecutar `ls /home; ls/var`.

Variantes (soportadas por la utilidad):

* Aceptar más de un argumento. Por ejemplo:

  ```sh
  $ printf "/home\n/var\n" | xargs ls -l
  ```

    En este caso, el comando ejecutado sobre el input es `ls -l`.
    <br>

* Aceptar nombres de archivos separados por espacio, por ejemplo:

    ```sh
    $ echo /home /var | xargs ls
    ```

    (Esto impediría, no obstante, que se puedan pasar a *xargs* nombres de archivos con espacios en sus nombres, como ser: `/home/user/Media Files`)
    <br>

* Aceptar el “empaquetado” de varios nombres de archivos en una sola invocación del comando. Por ejemplo, en el segundo ejemplo de arriba, que se ejecute `ls /home /var` (una sola vez) en lugar de `ls /home; ls /var` (dos veces).

Para ampliar y conocer el comportamiento de una implementación de *xargs* moderna, y por ejemplo las opciones `-r`, `-0`, `-n`, `-I` y `-P`, consultar la página de manual.

Se pide implementar la siguiente versión modificada de *xargs*.

#### Requisitos:

* Leer los nombres de archivo **línea a línea**, **nunca** separados **por espacios** (se recomienda usar la función [getline(3)](https://dashdash.io/3/getline)). También, es necesario eliminar el caracter `'\n'` para obtener el nombre del archivo.

* El “empaquetado” vendrá definido por un valor entero positivo disponible en la macro `NARGS` (la cual tiene efecto durante el proceso de compilación. Para más información se puede consultar la siguiente documentación). El programa debe funcionar de manera tal que siempre se pasen `NARGS` argumentos al comando ejecutado (excepto en su última ejecución, que pueden ser menos). **Si no se encuentra definida `NARGS`, se debe definir a 4**. Se puede hacer algo como:

```c
#ifndef NARGS
#define NARGS 4
#endif
```

* Se debe esperar siempre a que termine la ejecución del comando actual.

    * Mejora o funcionalidad opcional: si el primer argumento a *xargs* es -P, emplear hasta 4 ejecuciones del comando en paralelo (pero nunca más de 4 a la vez).

Llamadas al sistema: `fork(2)`, `execvp(3)`, `wait(2)`.