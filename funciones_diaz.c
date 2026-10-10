#include "funciones_diaz.h"

void pixelNegativo(char *argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0) //Devuelve 0 si cargo la imagen correctamente, sino devuelve cualquier otro numero.
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

}

void pixelGrises(char *argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelEspHoriz(char* argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelEspVert(char* argv[])
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelMasContrast(char *argv[], int valor)
{
    t_imagen img;
    
    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelMenosContrast(char* argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }


    //Liberamos la memoria de la matriz

    imagenLiberar(&img);
    
    return;
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

void pixelTonRojo(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelTonAzul(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelTonVerde(char *argv[], int valor)
{
    t_imagen img;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
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
        return;
    }

    //Liberamos la memoria de la matriz
    imagenLiberar(&img);

    return;
}

void pixelRecortar(char *argv[], int valor)
{
    t_imagen img;
    t_imagen imgRecorte;

    if(imagenCargar(&img, argv[POS_ARCHIVO]) != 0)
    {
        return;
    }

    imgRecorte.pixeles = NULL;
    imgRecorte.metadata.encabezado = malloc(img.metadata.tamEncabezado);
    if(imgRecorte.metadata.encabezado == NULL)
    {
        imagenLiberar(&img);
        return;
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
        return;
    }

    //Crear nueva matriz con las nuevas medidas
    imgRecorte.pixeles = calloc(abs(imgRecorte.metadata.alto), sizeof(t_pixel *));
    if(imgRecorte.pixeles == NULL)
    {
        printf("Hubo un error y no se pudo reservar memoria.\n");
        imagenLiberar(&imgRecorte);
        imagenLiberar(&img);

        return;
    }

    for(int y = 0; y < abs(imgRecorte.metadata.alto); y++)
    {
        imgRecorte.pixeles[y] = malloc(imgRecorte.metadata.ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos toda la memoria anterior.
        if(imgRecorte.pixeles[y] == NULL)
        {
            imagenLiberar(&imgRecorte);
            imagenLiberar(&img);

            return;
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
        return;
    }


    //Liberamos la memoria de la matriz
    imagenLiberar(&img);
    imagenLiberar(&imgRecorte);
    
    return;
}

int imagenCargar(t_imagen *img, const char *nombre)
{
    FILE *archivo;

    img->metadata.encabezado = NULL;
    img->pixeles = NULL;
    img->metadata.alto = 0;
    img->metadata.ancho = 0;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, nombre);

    //ABRIMOS EL ARCHIVO.
    archivo = fopen(direccion, "rb");
    if(archivo == NULL)
    {
        perror("Ocurrio un error y no se pudo abrir el archivo.");
        return 1;
    }

    int offset_datos;

    //LOS BYTES 18-25 NOS DICEN EL ANCHO Y EL ALTO DE LA IMAGEN.
    fseek(archivo, 18, SEEK_SET);
    if(fread(&img->metadata.ancho, sizeof(int), 1, archivo) != 1)
        goto error;
    if(fread(&img->metadata.alto, sizeof(int), 1, archivo) != 1)
        goto error;

    int filas = abs(img->metadata.alto);

    int padding = (4 - (img->metadata.ancho * sizeof(t_pixel)) % 4) % 4;

    //EL BYTE 10 NOS DICE DONDE EMPIEZA LA IMAGEN
    fseek(archivo, 10, SEEK_SET);
    if(fread(&offset_datos, sizeof(int), 1, archivo) != 1)
        goto error;

    if(offset_datos < 54)
        goto error;

    if(img->metadata.ancho <= 0 || img->metadata.alto == 0)
        goto error;

    //CLONAMOS EL ENCABEZADO.
    img->metadata.encabezado = malloc(offset_datos);
    if(img->metadata.encabezado == NULL)
    {
        goto error;
    }
    fseek(archivo, 0, SEEK_SET);
    if(fread(img->metadata.encabezado, offset_datos, 1, archivo) != 1)
        goto error;
    img->metadata.tamEncabezado = (unsigned int)offset_datos;

    //NOS PARAMOS DONDE EMPIEZA LA IMAGEN PARA CARGAR LA MATRIZ
    fseek(archivo, offset_datos, SEEK_SET);

    //CARGAMOS EL ARCHIVO A LA MATRIZ.
    img->pixeles = calloc(filas, sizeof(t_pixel*));
    if(img->pixeles == NULL)
    {
        goto error;
    }

    for(int y = 0; y < filas; y++)
    {
        img->pixeles[y] = malloc(img->metadata.ancho * sizeof(t_pixel));
        if(img->pixeles[y] == NULL)
        {
            goto error;
        }

        if(fread(img->pixeles[y], sizeof(t_pixel), img->metadata.ancho, archivo) != (size_t)img->metadata.ancho)
            goto error;

        //TENER EN CUENTA EL PADDING
        if(padding > 0)
        {
            fseek(archivo, padding, SEEK_CUR);
        }
    }

    //CERRAMOS EL ARCHIVO
    fclose(archivo);

    return 0;

    error:
        fclose(archivo);
        imagenLiberar(img);
        return 1;
}

void imagenLiberar(t_imagen *img)
{
    if(img->pixeles != NULL)
    {
        for(int y = 0; y < abs(img->metadata.alto); y++)
        {
            free(img->pixeles[y]);
        }

        free(img->pixeles);
    }

    free(img->metadata.encabezado);

    img->metadata.encabezado = NULL;
    img->pixeles = NULL;
}

int imagenGuardar(t_imagen *img, const char *nombre)
{
    FILE *archivo_nuevo;

    int padding = (4 - (img->metadata.ancho * sizeof(t_pixel)) % 4) % 4;
    int filas = abs(img->metadata.alto);
    int tamano_imagen = (img->metadata.ancho * (int)sizeof(t_pixel) + padding) * filas;
    img->metadata.tamArchivo = tamano_imagen + (int)img->metadata.tamEncabezado;


    archivo_nuevo = fopen(nombre, "wb");
    if(archivo_nuevo == NULL)
    {
        printf("Ocurrio un error y no se pudo crear el archivo %s.\n", nombre);
        return 1;
    }

    memcpy(img->metadata.encabezado + 2, &img->metadata.tamArchivo, sizeof(int));
    memcpy(img->metadata.encabezado + 18, &img->metadata.ancho, sizeof(int));
    memcpy(img->metadata.encabezado + 22, &img->metadata.alto, sizeof(int));
    memcpy(img->metadata.encabezado + 34, &tamano_imagen, sizeof(int));

    if(fwrite(img->metadata.encabezado, 1, img->metadata.tamEncabezado, archivo_nuevo) != img->metadata.tamEncabezado)
    {
        printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
        fclose(archivo_nuevo);
        return 1;
    }

    unsigned char padding_bytes[3] = {0, 0, 0};

    for(int y = 0; y < abs(img->metadata.alto); y++)
    {
        if(fwrite(img->pixeles[y], sizeof(t_pixel), img->metadata.ancho, archivo_nuevo) != (size_t)img->metadata.ancho)
        {
            printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
            fclose(archivo_nuevo);
            return 1;
        }

        if(padding > 0 && fwrite(padding_bytes, 1, padding, archivo_nuevo) != (size_t)padding)
        {
            printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
            fclose(archivo_nuevo);
            return 1;
        }
    }

    fclose(archivo_nuevo);

    printf("Se creo el archivo %s correctamente.\n", nombre);

    return 0;
}
