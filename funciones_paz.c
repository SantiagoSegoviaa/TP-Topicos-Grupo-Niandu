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

int pixelEspVert(char* argv[], int pos)
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

    for(int x = 0; x < img.metadata.ancho; x++)
    {
        int y = 0;

        while(y < abs(img.metadata.alto) / 2)
        {
            t_pixel aux = img.pixeles[y][x];
            img.pixeles[y][x] = img.pixeles[img.metadata.alto - 1 - y][x];
            img.pixeles[img.metadata.alto - 1 - y][x] = aux;

            y++;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "espejar-vertical_", nombre_archivo_nuevo) != OK)
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
