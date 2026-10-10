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
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    printf("Intentando abrir: [%s]\n", direccion);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Cambiamos los pixeles a su version en gris.

    unsigned char gris;

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            //Como el ojo humano es mas sensible al verde que al rojo o azul, usamos el metodo de luminosidad.
            //Gris = (0.299*R) + (0.587*G) + (0.114*B)

            gris = (0.299*imagen[y][x].r) + (0.587*imagen[y][x].g) + (0.114*imagen[y][x].b);

            imagen[y][x].r = gris;
            imagen[y][x].g = gris;
            imagen[y][x].b = gris;

        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "escala-de-grises_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);

}

void pixelEspHoriz(char* argv[])
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Invertimos los pixeles.

    for(int y = 0; y < alto; y++)
    {
        int x = 0;

        while(x < ancho / 2)
        {
            t_pixel aux = imagen[y][x];
            imagen[y][x] = imagen[y][ancho - 1 - x];
            imagen[y][ancho - 1 - x] = aux;

            x++;
        }
    }


    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "espejar-horizontal_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelEspVert(char* argv[])
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Invertimos los pixeles.

    for(int x = 0; x < ancho; x++)
    {
        int y = 0;

        while(y < alto / 2)
        {
            t_pixel aux = imagen[y][x];
            imagen[y][x] = imagen[alto - 1 - y][x];
            imagen[alto - 1 - y][x] = aux;

            y++;
        }
    }

    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[256];

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "espejar-vertical_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelMasContrast(char *argv[], int valor)
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //El calculo del aumento del contraste viene de un factor de escalado F que a su vez proviene de la variable de contraste C

    double var_contraste = (valor * 255)/100;
    double factor_esc = (259 * (var_contraste + 255)) / (255 * (259 - var_contraste));

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            imagen[y][x].r = pixelValorMasContrast(imagen[y][x].r, factor_esc);
            imagen[y][x].g = pixelValorMasContrast(imagen[y][x].g, factor_esc);
            imagen[y][x].b = pixelValorMasContrast(imagen[y][x].b, factor_esc);
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

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelMenosContrast(char* argv[], int valor)
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //El calculo del aumento del contraste viene de un factor de escalado F que a su vez proviene de la variable de contraste C

    double var_contraste = -(valor * 255)/100;
    double factor_esc = (259 * (var_contraste + 255)) / (255 * (259 - var_contraste));

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            imagen[y][x].r = pixelValorMasContrast(imagen[y][x].r, factor_esc);
            imagen[y][x].g = pixelValorMasContrast(imagen[y][x].g, factor_esc);
            imagen[y][x].b = pixelValorMasContrast(imagen[y][x].b, factor_esc);
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

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

unsigned char pixelValorMasContrast(unsigned char valorCanal, double factor)
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
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Para aumentar la intensidad de un color se aplica un factor M > 1.0 unicamente al canal que deseemos y dejamos los otros intactos.

    double m = 1 + (valor/100.0);

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            int nuevo_valor = (int)(imagen[y][x].r * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            imagen[y][x].r = (unsigned char)nuevo_valor;
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

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelTonAzul(char *argv[], int valor)
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Para aumentar la intensidad de un color se aplica un factor M > 1.0 unicamente al canal que deseemos y dejamos los otros intactos.

    double m = 1 + (valor/100.0);

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            int nuevo_valor = (int)(imagen[y][x].b * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            imagen[y][x].b = (unsigned char)nuevo_valor;
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

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelTonVerde(char *argv[], int valor)
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Para aumentar la intensidad de un color se aplica un factor M > 1.0 unicamente al canal que deseemos y dejamos los otros intactos.

    double m = 1 + (valor/100.0);

    for(int y = 0; y < alto; y++)
    {
        for(int x = 0; x < ancho; x++)
        {
            int nuevo_valor = (int)(imagen[y][x].g * m);

            if(nuevo_valor > 255)
                nuevo_valor = 255;

            imagen[y][x].g = (unsigned char)nuevo_valor;
        }
    }



    //Como tuvimos en cuenta el padding en la carga ahora no hace falta

    char nombre_archivo_nuevo[256];
    char num_buff[12];

    sprintf(num_buff, "%d", valor);

    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    strcpy(nombre_archivo_nuevo, NOMBRE_GRUPO);
    strcat(nombre_archivo_nuevo, "tonalidad-verde=");
    strcat(nombre_archivo_nuevo, num_buff);
    strcat(nombre_archivo_nuevo, "_");
    strcat(nombre_archivo_nuevo, argv[POS_ARCHIVO]);


    //Creamos el archivo nuevo para cargar la matriz modificada

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");
        return;
    }

    //Pasamos el mismo encabezado al nuevo archivo
    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < alto; y++)
    {
        fwrite(imagen[y], sizeof(t_pixel), ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding > 0)
        {
            fwrite(padding_bytes, 1, padding, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen);
    free(header_buffer);
}

void pixelRecortar(char *argv[], int valor)
{
    FILE *archivo_viejo;
    FILE *archivo_nuevo;

    char direccion[256];
    snprintf(direccion, sizeof(direccion), "%s/%s", SUBCARPETA, argv[POS_ARCHIVO]);

    archivo_viejo = fopen(direccion, "rb");
    if(archivo_viejo == NULL)
    {
        perror("No se pudo abrir el archivo correctamente.\n");
        return;
    }

    int ancho, alto, offset_datos;

    //Los bytes 18-21 tienen el ancho y los bytes 22-25 tienen el alto
    fseek(archivo_viejo, 18, SEEK_SET);
    fread(&ancho, sizeof(int), 1, archivo_viejo);
    fread(&alto, sizeof(int), 1, archivo_viejo);

    //Se calcula el padding del archivo.
    int padding = (4 - (ancho * sizeof(t_pixel)) % 4) % 4;

    //Nos paramos en el byte 10 para saber cuando empieza la imagen
    fseek(archivo_viejo, 10, SEEK_SET);
    fread(&offset_datos, sizeof(int), 1, archivo_viejo);

    //De paso clonamos el encabezado
    unsigned char *header_buffer = malloc(offset_datos);
    if(header_buffer == NULL)
    {
        perror("Hubo un problema y no se pudo guardar en memoria.\n");
        fclose(archivo_viejo);
        return;
    }
    fseek(archivo_viejo, 0, SEEK_SET);
    fread(header_buffer, offset_datos, 1, archivo_viejo);
    size_t tamano_header = (size_t)offset_datos;

    //Nos paramos donde empieza la imagen para pasarla a la matriz
    fseek(archivo_viejo, offset_datos, SEEK_SET);

    //Cargamos la imagen a la matriz de pixeles
    t_pixel** imagen = malloc(alto * sizeof(t_pixel*));
    if(imagen == NULL)
    {
        perror("Hubo un error y no se pudo guardar en memoria.\n");
        free(header_buffer);
        return;
    }

    for(int y = 0; y < alto; y++)
    {
        imagen[y] = malloc(ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos la memoria anterior
        if(imagen[y] == NULL)
        {
            perror("Hubo un error y no se pudo guardar en memoria.\n");
            for(int k = 0; k < y; k++)
            {
                free(imagen[k]);
            }

            free(imagen);
            free(header_buffer);
            fclose(archivo_viejo);

            return;
        }

        fread(imagen[y], sizeof(t_pixel), ancho, archivo_viejo);

        //Tener en cuenta el padding
        if(padding > 0)
        {
            fseek(archivo_viejo, padding, SEEK_CUR);
        }
    }

    //ACCESO SIMPLE A CUALQUIER PIXEL: imagen[y][x]
    //DONDE Y ES LA FILA Y X ES LA COLUMNA

    fclose(archivo_viejo);

    //Calcular nuevo alto y ancho de la foto recortada
    int nuevo_ancho = ancho * valor / 100;
    int nuevo_alto = alto* valor / 100 ;

    //Crear nueva matriz con las nuevas medidas
    t_pixel** imagen_recort = malloc(nuevo_alto * sizeof(t_pixel*));
    if(imagen_recort == NULL)
    {
        printf("Hubo un error y no se pudo reservar memoria.\n");
        free(header_buffer);
        for(int y = 0; y < alto; y++)
        {
            free(imagen[y]);
        }
        free(imagen);

        return;
    }

    for(int y = 0; y < nuevo_alto; y++)
    {
        imagen_recort[y] = malloc(nuevo_ancho * sizeof(t_pixel));

        //Si hay un error lo informamos y liberamos toda la memoria anterior.
        if(imagen_recort[y] == NULL)
        {
            for(int k = 0; k < y; k++)
            {
                free(imagen_recort[k]);
            }

            free(header_buffer);
            for(int k = 0; k < alto; k++)
            {
                free(imagen[k]);
            }
            free(imagen);

            return;
        }
    }

    //Pasamos los datos de la matriz vieja a la matriz nueva pero solo el rango que queremos

    int desplazamiento = alto - nuevo_alto;

    for(int y = 0; y < nuevo_alto; y++)
    {
        for(int x = 0; x < nuevo_ancho; x++)
        {
            imagen_recort[y][x] = imagen[y + desplazamiento][x];
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

    archivo_nuevo = fopen(nombre_archivo_nuevo, "wb");
    if(archivo_nuevo == NULL)
    {
        perror("Ocurrio un error y no se pudo crear el archivo.\n");

        free(header_buffer);

        for(int y = 0; y < alto; y++)
        {
            free(imagen[y]);
        }
        free(imagen);

        for(int y = 0; y < nuevo_alto; y++)
        {
            free(imagen_recort[y]);
        }
        free(imagen_recort);

        return;
    }

    //Como cambiaron las medidas calculamos el nuevo padding.
    int padding_nuevo = (4 - (nuevo_ancho * sizeof(t_pixel)) % 4) % 4;

    //Cambiamos los datos del encabezado con el nuevo ancho y el nuevo alto (y lo que eso implica)

    int tamano_datos_nuevo = (nuevo_ancho * 3 + padding_nuevo) * nuevo_alto;
    int tamano_archivo_nuevo = offset_datos + tamano_datos_nuevo;

    memcpy(header_buffer + 2, &tamano_archivo_nuevo, sizeof(int));
    memcpy(header_buffer + 18, &nuevo_ancho, sizeof(int));
    memcpy(header_buffer + 22, &nuevo_alto, sizeof(int));
    memcpy(header_buffer + 34, &tamano_datos_nuevo, sizeof(int));

    fwrite(header_buffer, 1, tamano_header, archivo_nuevo);

    unsigned char padding_bytes[3] = {0, 0, 0};

    //Cargamos la matriz modificada al archivo nuevo
    for(int y = 0; y < nuevo_alto; y++)
    {
        fwrite(imagen_recort[y], sizeof(t_pixel), nuevo_ancho, archivo_nuevo);

        //Si hay padding lo agregamos
        if(padding_nuevo > 0)
        {
            fwrite(padding_bytes, 1, padding_nuevo, archivo_nuevo);
        }

    }

    //Cerramos el archivo nuevo.
    fclose(archivo_nuevo);

    printf("Se creo el archivo %s corrertamente.\n", nombre_archivo_nuevo);

    //Liberamos la memoria de la matriz
    for(int y = 0; y < nuevo_alto; y++)
    {
        free(imagen_recort[y]);
    }
    for(int y = 0; y < alto; y++)
    {
        free(imagen[y]);
    }

    free(imagen_recort);
    free(imagen);
    free(header_buffer);
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
        fclose(archivo_nuevo);
        return 1;
    }

    unsigned char padding_bytes[3] = {0, 0, 0};

    for(int y = 0; y < abs(img->metadata.alto); y++)
    {
        if(fwrite(img->pixeles[y], sizeof(t_pixel), img->metadata.ancho, archivo_nuevo) != (size_t)img->metadata.ancho)
        {
            fclose(archivo_nuevo);
            return 1;
        }

        if(padding > 0 && fwrite(padding_bytes, 1, padding, archivo_nuevo) != (size_t)padding)
        {
            fclose(archivo_nuevo);
            return 1;
        }
    }

    fclose(archivo_nuevo);

    printf("Se creo el archivo %s correctamente.\n", nombre);

    return 0;
}
