#include "back.h"

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
  Tdep* dep[LETRAS+1]; //los departamentos que empiecen con un numero como "9 de Julio" irán a la primer posición
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

int cambioProv(int prov)
{
    if(prov==0)//es CABA
        return 2;
    if(prov==1)//es BsAs
        return 0;
    if(prov==2)//es Catamarca
        return 1;
    if(prov==22)
        return 23;
    if(prov==23)
        return 22;
    return prov;//ya que el resto de las provincias no cambiaron
}

/*Inserta un nodo del tipo Tdep en orden alfabetico*/
static Tdep* insertaDep(char* dep, int es_analf, censoADT censo, Tdep* primero, int* lugar)
{
    int c;
    if(primero==NULL || (c=strcmp(primero->nombre_dep,dep))>0)
    {
        Tdep* aux=(Tdep*)calloc(1,sizeof(Tdep));
        if(aux==NULL)//no hubo lugar
        {
            freeCenso(censo);
            *lugar=ERROR_MEMORIA;
            return primero;
        }
        Tdep* nuevo=aux;
        char* aux2=(char*)malloc((strlen(dep)+1));
        if(aux2==NULL)
        {
            freeCenso(censo);
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
    //cambio el valor de prov,para que tenga el valor alfabetico
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
        indice=tolower(dep[0])-'a'+1; //porque la primer posición es para los departamentos que empiezan con número
    else
        indice=0;
    censo->provincias[prov].dep[indice]=insertaDep(dep,es_analf,censo,censo->provincias[prov].dep[indice],&lugar);
    if(lugar==ERROR_MEMORIA)//no hubo lugar para guardar mas departamentos,sali
    {
        freeCenso(censo);
    }
    return lugar;
}

void printDeptos(Tdep* depto)
{
    if(depto==NULL)
    {
        printf("FIN\n");
        return;
    }
    printf("%s,%d,%g-",depto->nombre_dep,depto->habitantes_dep,(double)(depto->analf_dep)/depto->habitantes_dep);
    printDeptos(depto->next);
}

void printCenso(censoADT censo)
{
    int i,j,cant;

    printf("Analfabetismo:\n");
    for(i=0;i<VIVIENDAS;i++)
    {
        printf("%d,%s,%d,%g\n",i+1,nombre_viviendas[i],censo->viviendas[i].habitantes_viv,(double)(censo->viviendas[i].analf_viv)/censo->viviendas[i].habitantes_viv);
    }
    putchar('\n');
    printf("Provincias:\n");
    for(i=0;i<PROVINCIAS;i++)
    {
        cant=censo->provincias[i].habitantes_prov;
        printf("%s,%d,%g,%g\n",nombre_provincias[i],cant,(double)(censo->provincias[i].suma_edades_prov)/cant,(double)(censo->provincias[i].analf_prov)/cant);

    }
    putchar('\n');
    printf("Departamentos:\n");
    for(i=0;i<PROVINCIAS;i++)
    {
        for(j=0;j<LETRAS;j++)
        {
            if(censo->provincias[i].dep[j]!=NULL)
            {
                printf("%s,",nombre_provincias[i]);
                printDeptos(censo->provincias[i].dep[j]);
            }
        }
    }
    putchar('\n');
}

/* Devuelve 0 si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), 1 en caso contrario*/
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

/* Devuelve 0 si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), 1 en caso contrario*/
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

/* Devuelve 0 si se pudo crear el archivo y fclose devolvió 0 (que pudo cerrarlo), 1 en caso contrario*/
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
