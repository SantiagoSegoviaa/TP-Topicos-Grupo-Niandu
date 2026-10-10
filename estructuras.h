#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#define NOMBRE_GRUPO "PALETA_"

// Estructura sugerida, su uso es opcional.
typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}t_pixel;

typedef struct
{
    int tamArchivo;
    unsigned int tamEncabezado;
    unsigned char *encabezado;
    int ancho;
    int alto;
    unsigned short profundidad;
}t_metadata;

typedef struct
{
    t_metadata metadata;
    t_pixel **pixeles;
}t_imagen;


#endif // ESTRUCTURAS_H_INCLUDED
