#include "funciones_paz.h"

int pixelRotarDerecha (char* argv[])
{
    t_imagen imgOriginal;
    t_imagen imgNueva;

    if( imagenCargar(&imgOriginal, argv[POS_ARCHIVO]) != 0)
    {
        printf("Error al cargar el arcivo en funcion rotar-derecha.\n");
        return 1;
    }

    //crear una copia teniendo como base para el encabezado la original
    imagenCrear( &imgOriginal, &imgNueva, imgOriginal.metadata.ancho, imgOriginal.metadata.alto);

    //ajustes de resolucion vertical y horizontal
    memcpy(imgNueva.metadata.encabezado + 38, imgOriginal.metadata.encabezado + 42,sizeof(int));
    memcpy(imgNueva.metadata.encabezado + 42, imgOriginal.metadata.encabezado + 38,sizeof(int));

    //rotar derecha
    for( int i = 0; i < abs(imgOriginal.metadata.alto); i++)
    {
        for( int j = 0; j < abs(imgOriginal.metadata.ancho); j++)
        {
            imgNueva.pixeles[imgOriginal.metadata.ancho - 1 - j][i] = imgOriginal.pixeles[i][j];
        }
    }

    //crear nombre para el nuevo archivo
    char nombre_archivo_nuevo[256];
    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "rotar-derecha_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //crear archivo
    imagenGuardar(&imgNueva, nombre_archivo_nuevo);

    //liberar recursos usados
    imagenLiberar(&imgNueva);
    imagenLiberar(&imgOriginal);

    return 0;
}
