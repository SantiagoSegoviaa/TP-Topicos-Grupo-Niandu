#include "funciones_diaz.h"

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

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "aumentar-contraste_", nombre_archivo_nuevo) != OK)
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

int pixelMenosContrast(char* argv[], int valor, int pos)
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

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "reducir-contraste_", nombre_archivo_nuevo) != OK)
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

unsigned char pixelValorContrast(unsigned char valorCanal, double factor)
{
    int valorNuevo = factor * (valorCanal - 128) + 128;

    if(valorNuevo > 255)
        return 255;
    if(valorNuevo < 0)
        return 0;

    return (unsigned char)valorNuevo;
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

int pixelTonVerde(char *argv[], int valor, int pos)
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
            int nuevo_valor = (int)(img.pixeles[y][x].g * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            img.pixeles[y][x].g = (unsigned char)nuevo_valor;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "tonalidad-verde_", nombre_archivo_nuevo) != OK)
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

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

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

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[TAM_RUTA];

    if(GeneraRuta(argv[pos], "recortar_", nombre_archivo_nuevo) != OK)
    {
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(ImagenGuardar(&imgRecorte, nombre_archivo_nuevo) != 0)
    {
        ImagenLiberar(&imgRecorte);
        ImagenLiberar(&img);
        return ERR_EJECUCION;
    }


    //Liberamos la memoria de la matriz
    ImagenLiberar(&img);
    ImagenLiberar(&imgRecorte);

    return OK;
}
