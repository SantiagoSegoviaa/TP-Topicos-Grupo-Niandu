#include "funciones_diaz.h"

int pixelMasContrast(char *argv[], int valor, int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //El calculo del aumento del contraste viene de un factor de escalado F que a su vez proviene de la variable de contraste C

    double var_contraste = (valor * 255)/100;
    double factor_esc = (259 * (var_contraste + 255)) / (255 * (259 - var_contraste));

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            img.pixeles[y][x].r = pixelValorContrast(img.pixeles[y][x].r, factor_esc);
            img.pixeles[y][x].g = pixelValorContrast(img.pixeles[y][x].g, factor_esc);
            img.pixeles[y][x].b = pixelValorContrast(img.pixeles[y][x].b, factor_esc);
        }
    }

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "aumentar-contraste_", nombre_archivo_nuevo) != OK)
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

int pixelMenosContrast(char* argv[], int valor, int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //El calculo del aumento del contraste viene de un factor de escalado F que a su vez proviene de la variable de contraste C

    double var_contraste = -(valor * 255)/100;
    double factor_esc = (259 * (var_contraste + 255)) / (255 * (259 - var_contraste));

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            img.pixeles[y][x].r = pixelValorContrast(img.pixeles[y][x].r, factor_esc);
            img.pixeles[y][x].g = pixelValorContrast(img.pixeles[y][x].g, factor_esc);
            img.pixeles[y][x].b = pixelValorContrast(img.pixeles[y][x].b, factor_esc);
        }
    }

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "reducir-contraste_", nombre_archivo_nuevo) != OK)
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

unsigned char pixelValorContrast(unsigned char valorCanal, double factor)
{
    int valorNuevo = factor * (valorCanal - 128) + 128;

    if(valorNuevo > 255)
        return 255;
    if(valorNuevo < 0)
        return 0;

    return (unsigned char)valorNuevo;
}
