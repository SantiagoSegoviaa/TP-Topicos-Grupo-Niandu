#include "funciones_salazar.h"


// t_pixel esta en orden {b, g, r}  ->  rojo = r255, g0, b0
 const t_pixel COLOR_RELLENO = {0, 0, 255};

 int filaMemoria(const t_imagen *img, int filaVisual)
{
    int filas = abs(img->metadata.alto);
    return (img->metadata.alto > 0) ? filas - 1 - filaVisual : filaVisual;
}

int pixelConcatenarVert(char *argv[], int pos1, int pos2)
{
    t_imagen img1, img2, res;
    char rutaSalida[TAM_RUTA];
    int ret;

    int cod = ImagenCargar(&img1, argv[pos1]);
    if(cod != OK)
    {
        printf("No se pueden concatenar: no se pudo cargar '%s'.\n", argv[pos1]);
        return cod;
    }

    cod = ImagenCargar(&img2, argv[pos2]);
    if(cod != OK)
    {
        printf("No se pueden concatenar: no se pudo cargar '%s'.\n", argv[pos2]);
        ImagenLiberar(&img1);
        return cod;
    }

    int filas1    = abs(img1.metadata.alto);
    int filas2    = abs(img2.metadata.alto);
    int altoTotal = filas1 + filas2;
    int altoRes   = (img1.metadata.alto > 0) ? altoTotal : -altoTotal;
    int anchoRes  = (img1.metadata.ancho > img2.metadata.ancho)
                    ? img1.metadata.ancho : img2.metadata.ancho;

    if(ImagenCrear(&img1, &res, altoRes, anchoRes) != OK)
    {
        ImagenLiberar(&img1);  
        ImagenLiberar(&img2);
        return ERR_MEMORIA;
    }

    for(int i = 0; i < altoTotal; i++)
    {
        const t_imagen *origen = (i < filas1) ? &img1 : &img2;
        int filaOrigen = (i < filas1) ? i : i - filas1;

        const t_pixel *src = origen->pixeles[filaMemoria(origen, filaOrigen)];
        t_pixel *dst       = res.pixeles[filaMemoria(&res, i)];
        int anchoOrigen    = origen->metadata.ancho;

        memcpy(dst, src, anchoOrigen * sizeof(t_pixel));

        for(int x = anchoOrigen; x < anchoRes; x++)   // relleno si es mas angosta
            dst[x] = COLOR_RELLENO;
    }
    
    if(GeneraRuta(argv[pos1], "concatenar-vertical_", rutaSalida) != OK)
    {
        printf("No se pudo generar el nombre del archivo de salida.\n");
        ret = ERR_EJECUCION;
    }
    else if(ImagenGuardar(&res, rutaSalida) != OK)
        ret = ERR_EJECUCION;   // ImagenGuardar ya informa el motivo
    else
        ret = OK;

    ImagenLiberar(&img1);
    ImagenLiberar(&img2);
    ImagenLiberar(&res);

    return ret;
}

int pixelTonVerde(char *argv[], int valor, int pos)
{
    t_imagen img;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    //Para aumentar la intensidad de un color se aplica un factor M > 1.0 unicamente al canal que deseemos y dejamos los otros intactos.

    double m = 1 + (valor/100.0);

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            int nuevo_valor = (int)(img.pixeles[y][x].g * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            img.pixeles[y][x].g = (unsigned char)nuevo_valor;
        }
    }

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "tonalidad-verde_", nombre_archivo_nuevo) != OK)
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
