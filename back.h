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
/*Crea un nuevo censo*/
censoADT nuevoCenso();

/*Guarda la informacion recibida del .cvs a censo*/
int guardaInfo(int edad,int es_analf,int viv,char* dep,int prov,censoADT censo);

/*Cambia el codigo de la vivienda, por el codigo en orden alfabetico*/
int cambioVivienda(int viv);

/*Cambia el codigo de la provincia por el codigo alfabetico*/
int cambioProv(int prov);

/*Pasa la información del vector viviendas de censoCDT al archivo analfabetismo.csv*/
int Analfabetismo(censoADT censo);

int Provincia(censoADT censo);

int Departamento(censoADT censo);

void printCenso(censoADT censo);

/*Libera la memoria reservada por el programa*/
void freeCenso(censoADT censo);
