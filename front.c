#include "back.h"

void error(int rta)
{
    if (rta==ERROR_MEMORIA)
        printf("No hay espacio para guardar la informacion\n");
    else
        printf("No se pudo crear o cerrar el archivo\n");
}

int leeInfo(censoADT censo)
{
    char line[80];//voy a guardar la linea aca que tiene 80 como maximo caracteres
    int edad, es_analf, viv, prov, rta=BIEN;//rta indica si hubo error al guardar
    char dep[80];

  while(fgets(line,80,stdin)!=NULL && rta==BIEN)//lee mal los saltos de linea
  {
      if(line[0]!='\n')//ver bien si hay lineas blancas(\n) porque no las ignora
        {
            sscanf(line,"%d,%d,%d,%[^,],%d",&edad,&es_analf,&viv,dep,&prov);

            rta=guardaInfo(edad,es_analf,viv-1,dep,prov-1,censo);
        }
  }
  return rta;
}

/*Si retorna distinto de 0 es que no se pudo crear o cerrar el archivo*/
int creaArchivos(censoADT censo)
{
    int crea=BIEN;
    crea+=Analfabetismo(censo);
    crea+=Provincia(censo);
    crea+=Departamento(censo);
    freeCenso(censo);
    return crea;
}
