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

    if(ImagenGuardar(&imgNueva, nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&imgNueva);
        ImagenLiberar(&imgOriginal);
        return ERR_EJECUCION;
    }

    ImagenLiberar(&imgNueva);
    ImagenLiberar(&imgOriginal);

    return OK;
}

int pixelRecortar(char *argv[], int valor, int pos)
{
    t_imagen img;
    t_imagen imgRecorte;

    int cod = ImagenCargar(&img, argv[pos]);
    if(cod != OK)
    {
        return cod;
    }

    imgRecorte.pixeles = NULL;
    imgRecorte.metadata.encabezado = malloc(img.metadata.tamEncabezado);
    if(imgRecorte.metadata.encabezado == NULL)
    {
        ImagenLiberar(&img);
        return ERR_MEMORIA;
    }
    memcpy(imgRecorte.metadata.encabezado, img.metadata.encabezado, img.metadata.tamEncabezado);
    imgRecorte.metadata.tamEncabezado = img.metadata.tamEncabezado;

    //Calcular nuevo alto y ancho de la foto recortada
    imgRecorte.metadata.ancho = img.metadata.ancho * valor / 100;
    imgRecorte.metadata.alto = img.metadata.alto * valor / 100 ;

    //Validar los nuevos anchos y altos
    if(imgRecorte.metadata.ancho < 1 || abs(imgRecorte.metadata.alto) < 1)
    {
        printf("El recorte resulta en una imagen vacia.\n");
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Crear nueva matriz con las nuevas medidas
    imgRecorte.pixeles = calloc(abs(imgRecorte.metadata.alto), sizeof(t_pixel *));
    if(imgRecorte.pixeles == NULL)
    {
        printf("Hubo un error y no se pudo reservar memoria.\n");
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);

        return ERR_MEMORIA;
    }

    for(int y = 0; y < abs(imgRecorte.metadata.alto); y++)
    {
        imgRecorte.pixeles[y] = malloc(imgRecorte.metadata.ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos toda la memoria anterior.
        if(imgRecorte.pixeles[y] == NULL)
        {
            ImagenLiberar(&imgRecorte);
            ImagenLiberar(&img);

            return ERR_MEMORIA;
        }
    }

    //Pasamos los datos de la matriz vieja a la matriz nueva pero solo el rango que queremos

    int desplazamiento = abs(img.metadata.alto) - abs(imgRecorte.metadata.alto);

    for(int y = 0; y < abs(imgRecorte.metadata.alto); y++)
    {
        for(int x = 0; x < imgRecorte.metadata.ancho; x++)
        {
            imgRecorte.pixeles[y][x] = img.pixeles[y + desplazamiento][x];
        }
    }

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "recortar_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    if(ImagenGuardar(&imgRecorte, nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }


    ImagenLiberar(&img);
    ImagenLiberar(&imgRecorte);

    return OK;
}
