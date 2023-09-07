#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#ifndef NARGS
#define NARGS 4
#endif

void
args_free(char *argv[])
{
	for (size_t i = 1; i < NARGS; i++) {
		free(argv[i]);
		argv[i] = NULL;
	}
}

int
xargs(char *args[])
{
	ssize_t nread;
	size_t n = 0, len = 0;
	// Conseguir los argumentos de stdin hasta
	// tener NARGS o hasta que se termine el input.
	while (len < NARGS && (nread = getline(&args[len + 1], &n, stdin)) != -1) {
		args[++len][n] = '\0';
		n = 0;
	}

	// Marcar el fin de los
	// argumentos con NULL.
	args[len + 1] = NULL;

	// Crear un hijo que ejecute
	// el programa con el
	// empaquetado de argumentos.
	pid_t i = fork();
	if (i == 0) {
		// HIJO
		if (execvp(args[0], args) == -1)
			perror("Error en execvp");

	} else if (i > 0) {
		// PADRE
		wait(NULL);
		args_free(args);

		// Correr el programa
		// con los argumentos
		// restantes.
		if (nread != -1)
			xargs(args);

	} else {
		// ERROR
		perror("Error en fork");
		args_free(args);
		return 1;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Uso: xargs <programa>\n");
		return 1;
	}

	char *args[NARGS + 2];
	memset(args, 0, sizeof(args));
	args[0] = argv[1];
	return xargs(args);
}