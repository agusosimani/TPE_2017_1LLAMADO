#ifndef CensoBack_h
#define CensoBack_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PROVINCIAS 24
#define VIVIENDAS 9
#define LETRAS 26
#define BIEN 0
#define ERROR_MEMORIA 1
#define ERROR_ARCHIVO 2

typedef struct censoCDT * censoADT;
/* Crea un nuevo censo */
censoADT nuevoCenso();

/* Guarda la informacion recibida de entrada estandar a la estructura del censo */
int guardaInfo(int edad,int es_analf,int viv,char* dep,int prov,censoADT censo);

/* Pasa la información del vector viviendas de censoCDT al archivo Analfabetismo.csv.
Devuelve BIEN si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), ERROR_ARCHIVO en caso contrario */
int Analfabetismo(censoADT censo);

/* Pasa la información del vector viviendas de censoCDT al archivo Provincia.csv.
Devuelve BIEN si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), ERROR_ARCHIVO en caso contrario */
int Provincia(censoADT censo);

/* Pasa la información del vector viviendas de censoCDT al archivo Departamento.csv.
Devuelve BIEN si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), ERROR_ARCHIVO en caso contrario */
int Departamento(censoADT censo);

/* Libera todos los recursos utilizados por el programa */
void freeCenso(censoADT censo);

#endif
