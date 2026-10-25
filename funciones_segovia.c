#include "funciones_segovia.h"
#include <stdio.h>

int pixelNegativo(char *argv[], int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]); 
    if(cod != OK)
    {
        return cod;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //Cambiamos los pixeles a sus opuestos para crear el negativo

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            //Para invertir los valores rgb hay que hacer: valor_invertido = 255 - valor

            img.pixeles[y][x].r = 255 - img.pixeles[y][x].r;
            img.pixeles[y][x].g = 255 - img.pixeles[y][x].g;
            img.pixeles[y][x].b = 255 - img.pixeles[y][x].b;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta


    //Creamos un nombre nuevo para el archivo
    char nombre_archivo_nuevo[TAM_RUTA];
    if(GeneraRuta(argv[pos], "negativo_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Cargamos la matriz a un archivo nuevo.
    if(ImagenGuardar(&img,nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Liberamos la memoria de la matriz
    ImagenLiberar(&img);


    return OK;
}

int pixelGrises(char *argv[], int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //Cambiamos los pixeles a su version en gris.

    unsigned char gris;

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            gris = ((img.pixeles[y][x].r) + (img.pixeles[y][x].g) + (img.pixeles[y][x].b)) / 3;

            img.pixeles[y][x].r = gris;
            img.pixeles[y][x].g = gris;
            img.pixeles[y][x].b = gris;

        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "escala-de-grises_", nombre_archivo_nuevo) != OK)
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
