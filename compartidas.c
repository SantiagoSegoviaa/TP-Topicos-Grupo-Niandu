#include "compartidas.h"

char crearDireccion(char nombre_archiv)
{
    char direccion[256];

    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, nombre_archiv);

    return direccion;
}