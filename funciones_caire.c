#include "funciones_caire.h"

int pixelAchicar(char *argv[], int valor, int pos)
{
    t_imagen imgOriginal;
    t_imagen imgNueva;
    char nombre_archivo_nuevo[TAM_RUTA];

    int cod = ImagenCargar(&imgOriginal, argv[pos]);
    if(cod != OK)
        return cod;

    //El alto puede ser negativo (BMP guardado de arriba hacia abajo), por eso se conserva el signo
    int anchoNuevo = imgOriginal.metadata.ancho * valor / 100;
    int altoNuevo = imgOriginal.metadata.alto * valor / 100;
    int filasOriginal = abs(imgOriginal.metadata.alto);
    int filasNuevas = abs(altoNuevo);

    //Validar las nuevas medidas
    if(anchoNuevo < 1 || filasNuevas < 1)
    {
        printf("Achicar resulta en una imagen vacia.\n");
        ImagenLiberar(&imgOriginal);
        return ERR_EJECUCION;
    }

    if(ImagenCrear(&imgOriginal, &imgNueva, altoNuevo, anchoNuevo) != OK)
    {
        ImagenLiberar(&imgOriginal);
        return ERR_MEMORIA;
    }

    //Cada pixel de la imagen nueva toma el valor del pixel que le corresponde en la original
    for(int y = 0; y < filasNuevas; y++)
    {
        int yOrig = y * filasOriginal / filasNuevas;

        for(int x = 0; x < anchoNuevo; x++)
        {
            int xOrig = x * imgOriginal.metadata.ancho / anchoNuevo;

            imgNueva.pixeles[y][x] = imgOriginal.pixeles[yOrig][xOrig];
        }
    }

    if(GeneraRuta(argv[pos], "achicar_", nombre_archivo_nuevo) != OK)
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