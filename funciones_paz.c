#include "funciones_paz.h"

int pixelRotarDerecha (char* argv[], int pos)
{
    t_imagen imgOriginal;
    t_imagen imgNueva;

    if( imagenCargar(&imgOriginal, argv[pos]) != 0)
    {
        printf("Error al cargar el arcivo en funcion rotar-derecha.\n");
        return 1;
    }

    if(imagenCrear( &imgOriginal, &imgNueva, imgOriginal.metadata.ancho, imgOriginal.metadata.alto) != 0)
    {
        printf("Error al cargar el arcivo en funcion rotar-derecha.\n");
        imagenLiberar(&imgOriginal);
        return 1;
    }

    memcpy(imgNueva.metadata.encabezado + 38, imgOriginal.metadata.encabezado + 42,sizeof(int)); //asigno resolucion vertical vieja a resolucion horizontal nueva
    memcpy(imgNueva.metadata.encabezado + 42, imgOriginal.metadata.encabezado + 38,sizeof(int)); //asigno resolucion horizontal vieja a resolucion vertical nueva


    for( int i = 0; i < abs(imgOriginal.metadata.alto); i++)
    {
        for( int j = 0; j < abs(imgOriginal.metadata.ancho); j++)
        {
            imgNueva.pixeles[imgOriginal.metadata.ancho - 1 - j][i] = imgOriginal.pixeles[i][j];
        }
    }

    char nombre_archivo_nuevo[256];
    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "rotar-derecha_");
    strcat(nombre_archivo_nuevo, argv[pos]);

    imagenGuardar(&imgNueva, nombre_archivo_nuevo);

    imagenLiberar(&imgNueva);
    imagenLiberar(&imgOriginal);

    return 0;
}

