#include "front.c"

int main()
{
    censoADT censo=nuevoCenso();
    int rta=leeInfo(censo);

    if (rta==ERROR_MEMORIA)
        error(ERROR_MEMORIA);
    else
    {
        rta=creaArchivos(censo);
        if (rta!=BIEN)
            error(ERROR_ARCHIVO);
    }
}
