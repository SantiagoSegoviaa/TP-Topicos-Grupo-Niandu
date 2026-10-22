#include "funciones_lara.h"

int pixelRotarIzquierda (char* argv[], int pos)
{
    t_imagen original;
    t_imagen rotar;
    int alto, ancho;
    char nombre_archivo_nuevo[TAM_RUTA];


    if( ImagenCargar(&original, argv[pos]) != 0)
    {
        return ERR_EJECUCION;
    }

    if(ImagenCrear( &original, &rotar, original.metadata.ancho, original.metadata.alto) != 0)
    {
        ImagenLiberar(&original);
        return ERR_MEMORIA;
    }

    memcpy(rotar.metadata.encabezado + 38, original.metadata.encabezado + 42,sizeof(int));
    memcpy(rotar.metadata.encabezado + 42, original.metadata.encabezado + 38,sizeof(int));

    alto = original.metadata.alto;
    ancho = original.metadata.ancho;

    for( int i = 0; i < alto; i++)
    {
        for( int j = 0; j < ancho; j++)
        {
            rotar.pixeles[j][alto - 1 - i] = original.pixeles[i][j];
        }
    }

    if(GeneraRuta(argv[pos], "rotar-izquierda_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&rotar);
        ImagenLiberar(&original);
        return ERR_EJECUCION;
    }

    if(ImagenGuardar(&rotar, nombre_archivo_nuevo) != 0)
    {
        ImagenLiberar(&rotar);
        ImagenLiberar(&original);
        return ERR_EJECUCION;
    }

    ImagenLiberar(&rotar);
    ImagenLiberar(&original);

    return OK;
}
