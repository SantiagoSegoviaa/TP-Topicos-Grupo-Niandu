#include "funciones_segovia.h"

int pixelNegativo(char *argv[], int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }


    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {

            img.pixeles[y][x].r = 255 - img.pixeles[y][x].r;
            img.pixeles[y][x].g = 255 - img.pixeles[y][x].g;
            img.pixeles[y][x].b = 255 - img.pixeles[y][x].b;
        }
    }

    char nombre_archivo_nuevo[TAM_RUTA];
    if(GeneraRuta(argv[pos], "negativo_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }


    if(ImagenGuardar(&img,nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }


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

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "escala-de-grises_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    if(ImagenGuardar(&img, nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    ImagenLiberar(&img);

    return OK;
}
