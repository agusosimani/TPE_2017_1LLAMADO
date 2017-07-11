Para compilar el programa, en la terminal, ingresar:
gcc -o main main.c -Wall -pedantic -std=c99 CensoBack.c

Luego, para la ejecución, ingresar:
 ./main < censo3.csv

Donde canso3.csv es el archivo de 300.000 líneas provisto por la cátedra, que se encuentra dentro de los archivos subidos.

Para verificar que no haya leaks de memoria, ingresar:
valgrind --leak-check=full --show-leak-kinds=all ./main < censo3.csv