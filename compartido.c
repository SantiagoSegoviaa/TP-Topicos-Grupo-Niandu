#include "compartido.h"

int ImagenCargar(t_imagen *img, const char *nombre)
{
    FILE *archivo;
    
    img->metadata.encabezado = NULL;
    img->pixeles = NULL;
    img->metadata.alto = 0;
    img->metadata.ancho = 0;

    archivo = fopen(nombre, "rb");
    if(archivo == NULL)
    {
        return ERR_NO_EXISTE;
    }

    int offset_datos;

    //LOS BYTES 18-25 NOS DICEN EL ANCHO Y EL ALTO DE LA IMAGEN.
    fseek(archivo, 18, SEEK_SET);
    if(fread(&img->metadata.ancho, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }
    if(fread(&img->metadata.alto, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }

    int filas = abs(img->metadata.alto);

    int padding = (4 - (img->metadata.ancho * sizeof(t_pixel)) % 4) % 4;

    //EL BYTE 10 NOS DICE DONDE EMPIEZA LA IMAGEN
    fseek(archivo, 10, SEEK_SET);
    if(fread(&offset_datos, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }

    if(offset_datos < 54)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }

    if(img->metadata.ancho <= 0 || img->metadata.alto == 0)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }

    //CLONAMOS EL ENCABEZADO.
    img->metadata.encabezado = malloc(offset_datos);
    if(img->metadata.encabezado == NULL)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_MEMORIA;
    }
    fseek(archivo, 0, SEEK_SET);
    if(fread(img->metadata.encabezado, offset_datos, 1, archivo) != 1)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_EJECUCION;
    }
    img->metadata.tamEncabezado = (unsigned int)offset_datos;

    //NOS PARAMOS DONDE EMPIEZA LA IMAGEN PARA CARGAR LA MATRIZ
    fseek(archivo, offset_datos, SEEK_SET);

    //CARGAMOS EL ARCHIVO A LA MATRIZ.
    img->pixeles = calloc(filas, sizeof(t_pixel*));
    if(img->pixeles == NULL)
    {
        fclose(archivo);
        ImagenLiberar(img);
        return ERR_MEMORIA;
    }

    for(int y = 0; y < filas; y++)
    {
        img->pixeles[y] = malloc(img->metadata.ancho * sizeof(t_pixel));
        if(img->pixeles[y] == NULL)
        {
            fclose(archivo);
            ImagenLiberar(img);
            return ERR_MEMORIA;
        }

        if(fread(img->pixeles[y], sizeof(t_pixel), img->metadata.ancho, archivo) != (size_t)img->metadata.ancho)
        {
            fclose(archivo);
            ImagenLiberar(img);
            return ERR_EJECUCION;
        }

        //TENER EN CUENTA EL PADDING
        if(padding > 0)
        {
            fseek(archivo, padding, SEEK_CUR);
        }
    }

    //si el alto es negativo se da vuelta la imagen para que las funciones puedan trabajar con la matriz de manera mas simple
    if(img->metadata.alto < 0)
    {
        for(int y = 0; y < filas / 2; y++)
        {
            t_pixel *aux = img->pixeles[y];
            img->pixeles[y] = img->pixeles[filas - 1 - y];
            img->pixeles[filas - 1 - y] = aux;
        }
        img->metadata.alto = filas;
    }
    fclose(archivo);

    return OK;
}

void ImagenLiberar(t_imagen *img)
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

int ImagenGuardar(t_imagen *img, const char *nombre)
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
        return ERR_EJECUCION;
    }

    memcpy(img->metadata.encabezado + 2, &img->metadata.tamArchivo, sizeof(int));
    memcpy(img->metadata.encabezado + 18, &img->metadata.ancho, sizeof(int));
    memcpy(img->metadata.encabezado + 22, &img->metadata.alto, sizeof(int));
    memcpy(img->metadata.encabezado + 34, &tamano_imagen, sizeof(int));

    if(fwrite(img->metadata.encabezado, 1, img->metadata.tamEncabezado, archivo_nuevo) != img->metadata.tamEncabezado)
    {
        printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
        fclose(archivo_nuevo);
        return ERR_EJECUCION;
    }

    unsigned char padding_bytes[3] = {0, 0, 0};

    for(int y = 0; y < abs(img->metadata.alto); y++)
    {
        if(fwrite(img->pixeles[y], sizeof(t_pixel), img->metadata.ancho, archivo_nuevo) != (size_t)img->metadata.ancho)
        {
            printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
            fclose(archivo_nuevo);
            return ERR_EJECUCION;
        }

        if(padding > 0 && fwrite(padding_bytes, 1, padding, archivo_nuevo) != (size_t)padding)
        {
            printf("Ocurrio un error y no se pudo escribir sobre el archivo.\n");
            fclose(archivo_nuevo);
            return ERR_EJECUCION;
        }
    }

    fclose(archivo_nuevo);

    printf("Se creo el archivo %s correctamente.\n", nombre);

    return OK;
}

int ImagenCrear(const t_imagen* imgOrigen, t_imagen* imgDestino, int alto, int ancho)
{
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;
    unsigned int bytesFila = ancho * sizeof(t_pixel) + padding;
    int filas = abs(alto);

    //copiar encabezado teniendo como base imgOrigen
    imgDestino->metadata.alto = alto;
    imgDestino->metadata.ancho = ancho;
    imgDestino->metadata.profundidad = imgOrigen->metadata.profundidad;
    imgDestino->metadata.tamEncabezado = imgOrigen->metadata.tamEncabezado;
    imgDestino->metadata.tamArchivo = imgDestino->metadata.tamEncabezado + bytesFila * alto;
    imgDestino->metadata.encabezado = malloc(imgOrigen->metadata.tamEncabezado);
    if(imgDestino->metadata.encabezado == NULL)
    {
        printf("Error al duplicar el encabezado.\n");
        return ERR_NO_EXISTE;
    }
    memcpy(imgDestino->metadata.encabezado, imgOrigen->metadata.encabezado, imgOrigen->metadata.tamEncabezado);

    //declarar matriz de pixeles
    imgDestino->pixeles = calloc(filas, sizeof(t_pixel*));
    if(imgDestino->pixeles == NULL)
    {
        ImagenLiberar(imgDestino);
        printf("Error al duplicar el encabezado.\n");
        return ERR_NO_EXISTE;
    }
    for(int i = 0; i < filas; i++)
    {
        imgDestino->pixeles[i] = malloc(ancho * sizeof(t_pixel));
        if( imgDestino->pixeles[i] == NULL)
        {
            ImagenLiberar(imgDestino);
            printf("Error al duplicar el encabezado.\n");
            return ERR_MEMORIA;
        }
    }
    return OK;
}

int GeneraRuta(const char *ruta, const char* operacion, char *destino){

    char nombre[TAM_RUTA];
    char *p;
    char *sep=NULL;


    if(ruta == NULL || operacion == NULL || destino == NULL)
        return ERR_EJECUCION;

    if(strlen(NOMBRE_GRUPO) + strlen(operacion) + strlen(ruta) + 1 > TAM_RUTA)
        return ERR_EJECUCION;
        
    if((p = strrchr(ruta,'\\')) != NULL) // C:\Dev\imagenes\unlam.bmp
        sep = p;
    if((p = strrchr(ruta,'/')) != NULL && (sep == NULL || p > sep))
        sep = p;
    if((p = strrchr(ruta,':')) != NULL && (sep == NULL || p > sep))
        sep = p;

    if(sep == NULL)
    {
        strcpy(destino,NOMBRE_GRUPO);
        strcat(destino,operacion);
        strcat(destino,ruta);
        return OK;
    }
    else
    {
        strcpy(nombre,NOMBRE_GRUPO);
        strcat(nombre,operacion);
        strcat(nombre,(sep+1));

        strncpy(destino,ruta,(sep-ruta+1));
        *(destino+(sep - ruta + 1)) = '\0';
        strcat(destino,nombre);
        return OK;
    }


}
