#include "funciones_lara.h"

int pixelRotarIzquierda (char* argv[], int pos)
{
    t_imagen original;
    t_imagen rotar;
    int alto, ancho;
    char nombre_archivo_nuevo[TAM_RUTA];


    int cod = ImagenCargar(&original, argv[pos]);
    if(cod != OK)
    {
        return cod;
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

int pixelEspHoriz(char* argv[], int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //Invertimos los pixeles.

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        int x = 0;

        while(x < img.metadata.ancho / 2)
        {
            t_pixel aux = img.pixeles[y][x];
            img.pixeles[y][x] = img.pixeles[y][img.metadata.ancho - 1 - x];
            img.pixeles[y][img.metadata.ancho - 1 - x] = aux;

            x++;
        }
    }


    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "espejar-horizontal_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }


    //Creamos el archivo nuevo para cargar la matriz modificada

    if(ImagenGuardar(&img, nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Liberamos la memoria de la matriz
    ImagenLiberar(&img);

    return OK;
}
