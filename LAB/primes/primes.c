#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Escribe en fd1 los números no
// divisibles por p leidos de fd0.
int
escribir_no_divisibles(int p, int fd0, int fd1)
{
	int num;
	while (read(fd0, &num, sizeof(num)) > 0) {
		if (num % p != 0) {
			if (write(fd1, &num, sizeof(num)) == -1) {
				perror("Error escribiendo en pipe");
				return 1;
			}
		}
	}

	return 0;
}

int
primes(const int izq_read)
{
	int primo;
	// Lee el primer primo del pipe
	// izquierdo. En caso de que no haya
	// más primos termina el proceso.
	if (read(izq_read, &primo, sizeof(primo)) <= 0) {
		return 0;
	}

	printf("primo %i\n", primo);

	int der_fds[2];
	if (pipe(der_fds) == -1) {
		perror("Error creando pipe");
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
		int res = primes(der_fds[0]);
		close(der_fds[0]);
		return res;

	} else if (i > 0) {
		// PADRE
		close(der_fds[0]);

		// Pasar los números no divisibles
		// por primo al pipe derecho.
		int res = escribir_no_divisibles(primo, izq_read, der_fds[1]);
		close(der_fds[1]);

		int status;
		// Conseguir el estado
		// de finalización del hijo.
		waitpid(-1, &status, 0);
		return res == 1 ? 1 : WEXITSTATUS(status);

	} else {
		// ERROR
		perror("Error en fork");
		close(der_fds[0]);
		close(der_fds[1]);
		return 1;
	}
}

// Escribe el rango de
// números en fd1.
int
escribir_rango(int N, int fd1)
{
	for (int i = 2; i <= N; i++) {
		if (write(fd1, &i, sizeof(i)) == -1) {
			perror("Error escribiendo en pipe");
			return 1;
		}
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

	int i = fork();
	if (i == 0) {
		// HIJO
		close(fds[1]);
		int res = primes(fds[0]);
		close(fds[0]);
		return res;

	} else if (i > 0) {
		// PADRE
		int res = escribir_rango(N, fds[1]);
		close(fds[0]);
		close(fds[1]);

		int status;
		// Conseguir el estado
		// de finalización del hijo.
		waitpid(-1, &status, 0);
		return res == 1 ? 1 : WEXITSTATUS(status);

	} else {
		// ERROR
		perror("Error en fork");
		close(fds[0]);
		close(fds[1]);
		return 1;
	}
}
