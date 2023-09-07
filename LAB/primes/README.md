# Primes

La [criba de Eratóstenes](https://es.wikipedia.org/wiki/Criba_de_Erat%C3%B3stenes) ([sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) en inglés) es un algoritmo milenario para calcular todos los primos menores a un determinado número natural, n.

Si bien la visualización del algoritmo suele hacerse “tachando” en una grilla, el concepto de criba, o sieve (literalmente: cedazo, tamiz, colador) debe hacernos pensar más en un filtro. En particular, puede pensarse en n filtros apilados, donde el primero filtra los enteros múltiplos de 2, el segundo los múltiplos de 3, el tercero los múltiplos de 5, y así sucesivamente.

Si modelásemos cada filtro como un proceso, y la transición de un filtro al siguiente mediante tuberías (pipes), se puede implementar el algoritmo con el siguiente pseudo-código (ver [fuente original](https://swtch.com/~rsc/thread/), y en particular la imagen que allí se muestra):

```
p := <leer valor de pipe izquierdo>

imprimir p // asumiendo que es primo

mientras <pipe izquierdo no cerrado>:
    n = <leer siguiente valor de pipe izquierdo>
    si n % p != 0:
        escribir <n> en el pipe derecho
```

(El único proceso que es distinto, es el primero, que tiene que simplemente generar la secuencia de números naturales de 2 a n. No tiene lado izquierdo.)

La interfaz que se pide es:

```sh
$ ./primes <n>
```

donde n será un número natural mayor o igual a 2. El código debe crear una estructura de procesos similar a la mostrada en la imagen, de tal manera que:

* El primer proceso cree un proceso derecho, con el que se comunica mediante un pipe.

* Ese primer proceso, escribe en el pipe la secuencia de números de 2 a n, para a continuación cerrar el pipe y esperar la finalización del proceso derecho.

* Todos los procesos sucesivos aplican el pseudo-código mostrado anteriormente, con la salvedad de que son responsables de crear a su “hermano” derecho, y la tubería (pipe) que los comunica.

* Se debería poder ejecutar correctamente el programa con un N mayor o igual a 10000.

* **No** se debe realizar ninguna clase de optimización que simplifique el algoritmo.

Ejemplo de uso:

```
$ ./primes 35
primo 2
primo 3
primo 5
primo 7
primo 11
primo 13
primo 17
primo 19
primo 23
primo 29
primo 31
```

#### Ayuda:

* Conceptualmente esta tarea es la más difícil de las cuatro del lab, y no es prerrequisito para poder realizar las dos siguientes.

Llamadas al sistema: `fork(2)`, `pipe(2)`, `wait(2)`.