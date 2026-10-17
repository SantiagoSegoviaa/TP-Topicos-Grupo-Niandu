#include "funciones_diaz.h"

int pixelNegativo(char *argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0) //Devuelve 0 si cargo la imagen correctamente, sino devuelve cualquier otro numero.
    {
        return 1;
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
    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "negativo_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Cargamos la matriz a un archivo nuevo.
    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)//Devuelve 0 si guarda correctamente la imagen, sino devuelve otro valor
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);


    return 0;
}

int pixelGrises(char *argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    //Cambiamos los pixeles a su version en gris.

    unsigned char gris;

    for(int y = 0; y < abs(img.metadata.alto); y++)
    {
        for(int x = 0; x < img.metadata.ancho; x++)
        {
            //Como el ojo humano es mas sensible al verde que al rojo o azul, usamos el metodo de luminosidad.
            //Gris = (0.299*R) + (0.587*G) + (0.114*B)

            gris = (0.299*img.pixeles[y][x].r) + (0.587*img.pixeles[y][x].g) + (0.114*img.pixeles[y][x].b);

            img.pixeles[y][x].r = gris;
            img.pixeles[y][x].g = gris;
            img.pixeles[y][x].b = gris;

        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "escala-de-grises_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelEspHoriz(char* argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "espejar-horizontal_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelEspVert(char* argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "espejar-vertical_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelMasContrast(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "aumentar-contraste=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelMenosContrast(char* argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "reducir-contraste=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }


    //Liberamos la memoria de la matriz

    imagenLiberar(&img);

    return 0;
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

int pixelTonRojo(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "tonalidad-roja=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelTonAzul(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "tonalidad-azul=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelTonVerde(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "tonalidad-verde=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&img, nombre_archivo_nuevo) != 0)
    {
        return 1;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return 0;
}

int pixelRecortar(char *argv[], int valor)
{
    t_imagen img;
    t_imagen imgRecorte;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return 1;
    }

    imgRecorte.pixeles = NULL;
    imgRecorte.metadata.encabezado = malloc(img.metadata.tamEncabezado);
    if(imgRecorte.metadata.encabezado == NULL)
    {
        imagenLiberar(&img);
        return 1;
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
        imagenLiberar(&imgRecorte);
        imagenLiberar(&img);
        return 1;
    }

    //Crear nueva matriz con las nuevas medidas
    imgRecorte.pixeles = calloc(abs(imgRecorte.metadata.alto), sizeof(t_pixel *));
    if(imgRecorte.pixeles == NULL)
    {
        printf("Hubo un error y no se pudo reservar memoria.\n");
        imagenLiberar(&imgRecorte);
        imagenLiberar(&img);

        return 1;
    }

    for(int y = 0; y < abs(imgRecorte.metadata.alto); y++)
    {
        imgRecorte.pixeles[y] = malloc(imgRecorte.metadata.ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos toda la memoria anterior.
        if(imgRecorte.pixeles[y] == NULL)
        {
            imagenLiberar(&imgRecorte);
            imagenLiberar(&img);

            return 1;
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

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "recortar=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);

    //Creamos el archivo nuevo para cargar la matriz modificada

    if(imagenGuardar(&imgRecorte, nombre_archivo_nuevo) != 0)
    {
        imagenLiberar(&imgRecorte);
        imagenLiberar(&img);
        return 1;
    }


    //Liberamos la memoria de la matriz
    imagenLiberar(&img);
    imagenLiberar(&imgRecorte);

    return 0;
}
