#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int
primes(const int izq_read)
{
	int primo;
	// Lee el primer primo del pipe
	// izquierdo. En caso de que no haya
	// más primos termina el proceso.
	if (read(izq_read, &primo, sizeof(primo)) <= 0) {
		close(izq_read);
		return 0;
	}

	printf("primo %i\n", primo);

	int der_fds[2];
	if (pipe(der_fds) == -1) {
		perror("Error creando pipe");
		close(izq_read);
		return 1;
	}

	// Crear un hijo que filtre
	// los múltiplos del primo.
	pid_t i = fork();
	if (i == 0) {
		// HIJO
		close(izq_read);
		close(der_fds[1]);

		// Volver a correr primes
		// con el pipe derecho.
		if (primes(der_fds[0]) == 1) {
			close(der_fds[0]);
			return 1;
		}

		close(der_fds[0]);

	} else if (i > 0) {
		// PADRE
		close(der_fds[0]);

		int num;
		// Filtra los múltiplos del primo y
		// escribe los "primos" en el pipe derecho.
		while (read(izq_read, &num, sizeof(num)) > 0)
			if (num % primo != 0)
				if (write(der_fds[1], &num, sizeof(num)) == -1) {
					perror("Error escribiendo en pipe");
					close(der_fds[1]);
					close(izq_read);
					wait(NULL);
					return 1;
				}

		close(der_fds[1]);
		close(izq_read);
		wait(NULL);

	} else {
		// ERROR
		perror("Error en fork");
		close(der_fds[0]);
		close(der_fds[1]);
		close(izq_read);
		return 1;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Uso: %s <n>\n", argv[0]);
		return 1;
	}

	const int N = atoi(argv[1]);
	if (N < 2) {
		printf("Uso: %s <n> (n >= 2)\n", argv[0]);
		return 1;
	}

	int fds[2];
	if (pipe(fds) == -1) {
		perror("Error creando pipe");
		return 1;
	}

	// Escribir el rango de
	// números en el pipe.
	for (int i = 2; i <= N; i++)
		if (write(fds[1], &i, sizeof(i)) == -1) {
			perror("Error escribiendo en pipe");
			close(fds[0]);
			close(fds[1]);
			return 1;
		}

	close(fds[1]);
	return primes(fds[0]);
}
