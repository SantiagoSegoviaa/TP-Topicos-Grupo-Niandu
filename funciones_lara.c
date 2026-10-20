#include "funciones_lara.h"

int pixelRotarIzquierda (char* argv[], int pos)
{
    t_imagen original;
    t_imagen rotar;
    int alto, ancho; 
    char rotar_name[256];


    if( imagenCargar(&original, argv[pos]) != 0)
    {
        imagenLiberar(&original);
        return 1;
    }

    imagenCrear( &original, &rotar, original.metadata.ancho, original.metadata.alto);

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

    strcpy(rotar_name, NOMBRE_GRUPO);
    strcat(rotar_name, "rotar-izquierda_");
    strcat(rotar_name, argv[pos]);

    imagenGuardar(&rotar, rotar_name);
    
    imagenLiberar(&rotar);
    imagenLiberar(&original);

    return 0;

    //bio

}
