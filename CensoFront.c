#include "CensoBack.h"

/* Le avisa al usuario si hubo un error y libera los recursos utilizados */
void error(int rta, censoADT censo);

/* Lee la información desde entrada estandar y la guarda en las estructuras.
Devuelve que hubo ERROR_MEMORIA si no se logró hacer algun alloc debido a falta de memoria */
int leeInfo(censoADT censo);

/* Si retorna distinto de 0 es que no se pudo crear o cerrar el archivo.
Pasa toda la información leída a los archivos Analfabetismo.csv, Provincia.csv y Departamento.csv
Luego, libera todos los recursos utilizados por el programa */
int creaArchivos(censoADT censo);


void error(int rta, censoADT censo)
{
	freeCenso(censo);
    if (rta==ERROR_MEMORIA)
        printf("No hay espacio para guardar la informacion\n");
    else
        printf("No se pudo crear o cerrar el archivo\n");
}

int leeInfo(censoADT censo)
{
    char line[80]; //string donde guardo cada línea de texto (80 caracteres máximo)
    int edad, es_analf, viv, prov, rta=BIEN; //rta indica si hubo error o no al guardar
    char dep[80];

  while(fgets(line,80,stdin)!=NULL && rta==BIEN)
  {
      if(line[0]!='\n')//ignora lineas en blanco
        {
            sscanf(line,"%d,%d,%d,%[^,],%d",&edad,&es_analf,&viv,dep,&prov);

            rta=guardaInfo(edad,es_analf,viv-1,dep,prov-1,censo);
        }
  }
  return rta;
}

int creaArchivos(censoADT censo)
{
    int crea=BIEN; //crea indica si hubo error o no al crear los archivos
    crea+=Analfabetismo(censo);
    crea+=Provincia(censo);
    crea+=Departamento(censo);
    freeCenso(censo);
    return crea;
}
