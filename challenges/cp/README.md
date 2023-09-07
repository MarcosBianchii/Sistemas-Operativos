# cp

La syscall mmap permite mapear una región de los contenidos de un archivo a memoria, y acceder a los mismos directamente como si fuera un array de bytes. Si se utilizan los flags apropiados (`MAP_SHARED`, ver `mmap(2)`) los cambios en la memoria correspondiente al archivo se verán reflejados en el mismo.

Si bien el uso de las syscall de entrada y salida básicas es la implementación más común para cp, también es posible utilizar mmap para copiar archivos. La idea es crear un archivo nuevo, y mapear tanto el archivo origen como el destino a regiones de memoria distintas, luego copiar los datos en memoria de una región a la otra (por ejemplo, utilizando `memcpy`).

Implementar cp1, que debe tener la misma funcionalidad que cp0 pero implementada mediante `mmap` y `memcpy`.

Syscalls recomendadas: `mmap(2)`, `memcpy(2)`, `open(2)`