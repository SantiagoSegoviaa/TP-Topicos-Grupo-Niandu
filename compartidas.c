#include "compartidas.h"

int imagenCargar(t_imagen *img, const char *nombre)
{
    FILE *archivo;

    img->metadata.encabezado = NULL;
    img->pixeles = NULL;
    img->metadata.alto = 0;
    img->metadata.ancho = 0;

    char direccion[256];
    //snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, nombre);

    //ABRIMOS EL ARCHIVO.
    archivo = fopen(nombre, "rb");
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
