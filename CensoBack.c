#include "CensoBack.h"

const char* nombre_provincias[PROVINCIAS]={"Buenos Aires", "Catamarca", "Ciudad Autonoma de Buenos Aires","Cordoba", "Corrientes", "Chaco", "Chubut", "Entre Rios", "Formosa", "Jujuy", "La Pampa", "La Rioja", "Mendoza", "Misiones", "Neuquen", "Rio negro", "Salta", "San Juan", "San Luis", "Santa Cruz", "Santa Fe", "Santiago del Estero", "Tierra del Fuego", "Tucuman"};
const char* nombre_viviendas[VIVIENDAS]={"Casa", "Rancho", "Casilla", "Departamento", "Pieza en inquilinato", "Pieza en hotel familiar o pension", "Local no construido para habitacion", "Vivienda movil", "Persona/s viviendo en la calle"};

typedef struct deps
{
  char* nombre_dep;
  struct deps* next;
  int habitantes_dep;
  int analf_dep;
}Tdep;

typedef struct viviendas
{
  int habitantes_viv;
  int analf_viv;
}Tviv;

typedef struct prov
{
  int habitantes_prov;
  Tdep* dep[LETRAS+1]; //los departamentos que empiecen con un numero (como "9 de Julio") irán a la primer posición
  int analf_prov;
  int suma_edades_prov;
}Tprov;

typedef struct censoCDT
{
  Tprov provincias[PROVINCIAS];
  Tviv viviendas[VIVIENDAS];
}censoCDT;

censoADT nuevoCenso()
{
    censoADT nuevo=(censoADT)calloc(1,sizeof(censoCDT));
    return nuevo;
}

/* Cambia el código de la provincia por el índice que le correspondería por orden alfabético */
static int cambioProv(int prov)
{
    if(prov==0) //es CABA
        return 2;
    if(prov==1) //es BsAs
        return 0;
    if(prov==2) //es Catamarca
        return 1;
    if(prov==22) //es Tucuman
        return 23;
    if(prov==23) //es Tierra del Fuego
        return 22;
    return prov; //ya que el resto de las provincias no cambiaron
}

/* Inserta un nodo del tipo Tdep en orden alfabético en la posición del vector dep correspondiente.
Modifica el flag "lugar" con ERROR_MEMORIA en caso de que no haya memoria para crear un nodo */
static Tdep* insertaDep(char* dep, int es_analf, censoADT censo, Tdep* primero, int* lugar)
{
    int c;
    if(primero==NULL || (c=strcmp(primero->nombre_dep,dep))>0)
    {
        Tdep* aux=(Tdep*)calloc(1,sizeof(Tdep));
        if(aux==NULL)//no hubo lugar
        {
            *lugar=ERROR_MEMORIA;
            return primero;
        }
        Tdep* nuevo=aux;

        char* aux2=(char*)malloc((strlen(dep)+1));
        if(aux2==NULL)
        {
            *lugar=ERROR_MEMORIA;
            return primero;
        }
        strcpy(aux2,dep);
        nuevo->nombre_dep=aux2;
        nuevo->habitantes_dep++;
        if(es_analf==2)
            nuevo->analf_dep++;
        nuevo->next=primero;
        return nuevo;
    }
    if(c==0)
    {
        primero->habitantes_dep++;
        if(es_analf==2)
            primero->analf_dep++;
        return primero;
    }
    primero->next=insertaDep(dep,es_analf,censo,primero->next,lugar);
    return primero;
}

int guardaInfo(int edad, int es_analf, int viv, char* dep, int prov, censoADT censo)
{
    int lugar=BIEN;//corrobora si hubo lugar en insertaDep
    
    prov=cambioProv(prov);

    censo->provincias[prov].habitantes_prov++;
    censo->viviendas[viv].habitantes_viv++;
    censo->provincias[prov].suma_edades_prov+=edad;
    if(es_analf==2)
    {
        censo->provincias[prov].analf_prov++;
        censo->viviendas[viv].analf_viv++;
    }

    int indice;
    if (isalpha(dep[0]))
        indice=tolower(dep[0])-'a'+1; //el +1 es porque la primer posición es para los departamentos que empiezan con número
    else
        indice=0;
    censo->provincias[prov].dep[indice]=insertaDep(dep,es_analf,censo,censo->provincias[prov].dep[indice],&lugar);

    return lugar;
}

int Analfabetismo(censoADT censo)
{
    int i,j;
    double indice=0;
    FILE* analf=fopen("Analfabetismo.csv","wt");
    if (analf==NULL)
        return ERROR_ARCHIVO;

    for (i=0; i<VIVIENDAS; i++)
    {
        j=i+1;
        if (censo->viviendas[i].habitantes_viv!=0)
            indice=(double)(censo->viviendas[i].analf_viv)/censo->viviendas[i].habitantes_viv;
        fprintf(analf,"%d,%s,%d,%.2f\n",j,nombre_viviendas[i],censo->viviendas[i].habitantes_viv,indice);
    }
    return fclose(analf);
}

int Provincia(censoADT censo)
{
    double prom_edad,indice;
    FILE* prov=fopen("Provincia.csv","wt");
    if (prov==NULL)
        return ERROR_ARCHIVO;

    for (int i=0; i<PROVINCIAS; i++)
    {
        prom_edad=(double)(censo->provincias[i].suma_edades_prov)/censo->provincias[i].habitantes_prov;
        indice=(double)(censo->provincias[i].analf_prov)/censo->provincias[i].habitantes_prov;
        fprintf(prov,"%s,%d,%.2f,%.2f\n",nombre_provincias[i],censo->provincias[i].habitantes_prov,prom_edad,indice);
    }
    return fclose(prov);
}

/* Toma la información de cada nodo de la lista dep y la escribe en el archivo Departamento.csv */
static void DepartamentoR(Tdep* first, FILE* depar, int prov)
{
    if (first==NULL)
        return;
    double indice=0;
    if (first->habitantes_dep!=0)
        indice=(double)(first->analf_dep)/first->habitantes_dep;
    fprintf(depar,"%s,%s,%d,%.2f\n", nombre_provincias[prov], first->nombre_dep, first->habitantes_dep, indice);
    DepartamentoR(first->next, depar, prov);
}

int Departamento(censoADT censo)
{
    int i,j;
    FILE* depar=fopen("Departamento.csv","wt");
    if (depar==NULL)
        return ERROR_ARCHIVO;

    for(i=0; i<PROVINCIAS; i++)
    {
        for(j=0; j<LETRAS+1; j++)
        {
            DepartamentoR(censo->provincias[i].dep[j],depar,i);
        }
    }
    return fclose(depar);
}

/* Libera los recursos utilizados en cada nodo de la lista dep de manera recursiva */
static void freeTdep(Tdep* dep)
{
    if(dep==NULL)
        return;
    freeTdep(dep->next);
    free(dep->nombre_dep);
    free(dep);
}

void freeCenso(censoADT censo)
{
    int i,j;
    for(i=0; i<PROVINCIAS; i++)
    {
        for(j=0; j<LETRAS+1; j++)
        {
            freeTdep(censo->provincias[i].dep[j]);
        }
    }
    free(censo);
}
