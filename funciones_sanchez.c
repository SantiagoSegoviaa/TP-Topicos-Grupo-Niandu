#include "funciones_sanchez.h"

static const t_pixel relleno = {0, 165, 255};

static int filaReal(const t_imagen *img, int filaVisual)
{
    int alto = abs(img->metadata.alto);

    if(img->metadata.alto > 0)
        return alto - 1 - filaVisual;

    return filaVisual;
}

int pixelConcatenarHoriz(char *argv[], int pos1, int pos2)
{
    t_imagen img1;
    t_imagen img2;
    t_imagen nueva;
    char nombre_archivo_nuevo[TAM_RUTA];

    int cod = ImagenCargar(&img1, argv[pos1]);
    if(cod != OK)
        return cod;

    cod = ImagenCargar(&img2, argv[pos2]);
    if(cod != OK)
    {
        ImagenLiberar(&img1);
        return cod;
    }

    int alto1 = abs(img1.metadata.alto);
    int alto2 = abs(img2.metadata.alto);
    int ancho1 = img1.metadata.ancho;
    int ancho2 = img2.metadata.ancho;

    int altoMax = (alto1 > alto2) ? alto1 : alto2;
    int anchoTotal = ancho1 + ancho2;
    int altoFinal = (img1.metadata.alto > 0) ? altoMax : -altoMax;

    if(ImagenCrear(&img1, &nueva, altoFinal, anchoTotal) != 0)
    {
        ImagenLiberar(&img1);
        ImagenLiberar(&img2);
        return ERR_MEMORIA;
    }

    for(int v = 0; v < altoMax; v++)
    {
        int yNueva = filaReal(&nueva, v);

        for(int x = 0; x < ancho1; x++)
        {
            if(v < alto1)
                nueva.pixeles[yNueva][x] = img1.pixeles[filaReal(&img1, v)][x];
            else
                nueva.pixeles[yNueva][x] = relleno;
        }

        for(int x = 0; x < ancho2; x++)
        {
            if(v < alto2)
                nueva.pixeles[yNueva][ancho1 + x] = img2.pixeles[filaReal(&img2, v)][x];
            else
                nueva.pixeles[yNueva][ancho1 + x] = relleno;
        }
    }

    int resultado = OK;

    if(GeneraRuta(argv[pos1], "concatenar-horizontal_", nombre_archivo_nuevo) != OK)
        resultado = ERR_EJECUCION;
    else if(ImagenGuardar(&nueva, nombre_archivo_nuevo) != 0)
        resultado = ERR_EJECUCION;

    ImagenLiberar(&nueva);
    ImagenLiberar(&img1);
    ImagenLiberar(&img2);

    return resultado;
}

int pixelTonAzul(char *argv[], int valor, int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //Para aumentar la intensidad de un color se aplica un factor M > 1.0 unicamente al canal que deseemos y dejamos los otros intactos.

    double m = 1 + (valor/100.0);

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            int nuevo_valor = (int)(img.pixeles[y][x].b * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            img.pixeles[y][x].b = (unsigned char)nuevo_valor;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "tonalidad-azul_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(ImagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Liberamos la memoria de la matriz
    ImagenLiberar(&img);

    return OK;
}
