#include "funciones_gramajo.h"

static unsigned char limitarCanal(int valor)
{
    if(valor > 255)
        return 255;
    return (unsigned char)valor;
}

static int nombreArchivoComodin(const char *ruta, char *destino)
{
    const char *ultimoSeparador = strrchr(ruta, '/');
    const char *separadorWindows = strrchr(ruta, '\\');
    const char *nombre;
    int largo;

    if(separadorWindows != NULL &&
       (ultimoSeparador == NULL || separadorWindows > ultimoSeparador))
        ultimoSeparador = separadorWindows;

    nombre = ultimoSeparador == NULL ? ruta : ultimoSeparador + 1;
    largo = snprintf(destino, TAM_RUTA, "%scomodin_%s", NOMBRE_GRUPO, nombre);

    if(largo < 0 || largo >= TAM_RUTA)
        return ERR_EJECUCION;

    return OK;
}

int pixelComodin(char *argv[], int pos)
{
    t_imagen img;
    char nombre_archivo_nuevo[TAM_RUTA];

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
        return cod;

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            t_pixel original = img.pixeles[y][x];

            int rojo  = (393 * original.r + 769 * original.g + 189 * original.b + 500) / 1000;
            int verde = (349 * original.r + 686 * original.g + 168 * original.b + 500) / 1000;
            int azul  = (272 * original.r + 534 * original.g + 131 * original.b + 500) / 1000;

            img.pixeles[y][x].r = limitarCanal(rojo);
            img.pixeles[y][x].g = limitarCanal(verde);
            img.pixeles[y][x].b = limitarCanal(azul);
        }
    }

    if(nombreArchivoComodin(argv[pos], nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    if(ImagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    ImagenLiberar(&img);
    return OK;
}

int pixelTonRojo(char *argv[], int valor, int pos)
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
            int nuevo_valor = (int)(img.pixeles[y][x].r * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            img.pixeles[y][x].r = (unsigned char)nuevo_valor;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "tonalidad-roja_", nombre_archivo_nuevo) != OK)
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
