#include "funciones_paz.h"

int pixelRotarDerecha (char* argv[], int pos)
{
    t_imagen imgOriginal;
    t_imagen imgNueva;

    int cod = ImagenCargar(&imgOriginal, argv[pos]);
    if(cod != OK)
    {
        printf("Error al cargar el arcivo en funcion rotar-derecha.\n");
        return cod;
    }

    if(ImagenCrear( &imgOriginal, &imgNueva, imgOriginal.metadata.ancho, imgOriginal.metadata.alto) != 0)
    {
        ImagenLiberar(&imgOriginal);
        return ERR_MEMORIA;
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

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "rotar-derecha_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&imgNueva);
        ImagenLiberar(&imgOriginal);
        return ERR_EJECUCION;
    }

    if(ImagenGuardar(&imgNueva, nombre_archivo_nuevo) != 0)
    {
        ImagenLiberar(&imgNueva);
        ImagenLiberar(&imgOriginal);
        return ERR_EJECUCION;
    }

    ImagenLiberar(&imgNueva);
    ImagenLiberar(&imgOriginal);

    return OK;
}

